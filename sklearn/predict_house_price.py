#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2019 
# author(s): Noel Tchidjo
# All rights reserved


from sklearn.pipeline import Pipeline
from sklearn.externals import joblib
import pandas as pd
from sklearn.base import BaseEstimator, TransformerMixin
import glob
import sys
sys.path.append('thrift_generated/house_prediction')

from thrift_generated.house_prediction.house_prediction import Processor
from thrift_generated.house_prediction.house_prediction import Iface
from thrift_generated.house_prediction.ttypes import PredictionResult


from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
import logging
from datetime import datetime

class DataFrameSelector(BaseEstimator, TransformerMixin):
    def __init__(self, attribute_names):
        self.attribute_names = attribute_names
    def fit(self, X, y=None):
        return self
    def transform(self, X):
        return X[self.attribute_names].values


class PredictorHandler(Iface):
   def __init__(self):
      logging.info("Loading pipeline ...")
      self.full_pipeline = joblib.load("model_pipeline_geo_16062019.pkl")
      logging.info("Loading pipeline finished.")
      logging.info("Loading model ...")
      self.model_loaded = joblib.load("model_paris_all_geo_05062019.pkl")
      logging.info("Loading model finished.")

      self.df = pd.DataFrame(columns=[["date_mutation","code_postal","lot1_surface_carrez","lot2_surface_carrez","lot3_surface_carrez","lot4_surface_carrez","lot5_surface_carrez","nombre_lots","code_type_local","surface_reelle_bati","nombre_pieces_principales","surface_terrain","longitude","latitude"]])
   

   def getprice(self, house_desc):
      house_crit = []
      house_crit.append(pd.to_datetime(house_desc["date_mutation"]).toordinal()) if "date_mutation" in house_desc else  house_crit.append(0)
      house_crit.append(float(house_desc["code_postal"])) if "code_postal" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["lot1_surface_carrez"])) if "lot1_surface_carrez" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["lot2_surface_carrez"])) if "lot2_surface_carrez" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["lot3_surface_carrez"])) if "lot3_surface_carrez" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["lot4_surface_carrez"])) if "lot4_surface_carrez" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["lot5_surface_carrez"])) if "lot5_surface_carrez" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["nombre_lots"])) if "nombre_lots" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["code_type_local"])) if "code_type_local" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["surface_reelle_bati"])) if "surface_reelle_bati" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["nombre_pieces_principales"])) if "nombre_pieces_principales" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["surface_terrain"])) if "surface_terrain" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["longitude"])) if "longitude" in house_desc else  house_crit.append(0.0)
      house_crit.append(float(house_desc["latitude"])) if "latitude" in house_desc else  house_crit.append(0.0)  

      logging.info(str(house_crit))
 
      self.df.loc[0] = house_crit

      house_prepared = self.full_pipeline.transform(self.df)
      price = self.model_loaded.predict(house_prepared)

      logging.info("value computed:%f" %price[0])
      val = PredictionResult()
      val.predictedvalue = price[0]
      val.error = ""
      return val


if __name__ == '__main__':
   now = datetime.now()
   log_filename ="predict_house_price_" + now.strftime("%Y-%m-%d-%H-%M-%S") + ".log"
   logging.basicConfig(filename=log_filename,level=logging.INFO, format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')
   logging.info("Starting House predictor ...")
   handler = PredictorHandler()
   processor = Processor(handler)
   transport = TSocket.TServerSocket(host='127.0.0.1', port=6060)
   tfactory = TTransport.TBufferedTransportFactory()
   pfactory = TBinaryProtocol.TBinaryProtocolFactory()

   server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

   logging.info('Waiting for request ...')
   server.serve()
   logging.info('done.')

