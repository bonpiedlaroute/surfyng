#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2021 
# author(s): Noel Tchidjo
# All rights reserved


import glob
import sys

sys.path.append('thrift_generated/emailalertservice')
sys.path.append('../../bot')
sys.path.append('../../bot/thrift_generated/dynamodb_access')

from thrift_generated.emailalertservice.emailalertservice import Processor
from thrift_generated.emailalertservice.emailalertservice import Iface
from thrift_generated.emailalertservice.ttypes import EmailAlertResult


from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
import logging
import datetime
import configparser
import os
import smtplib
import firebase_admin
from firebase_admin import credentials
from firebase_admin import auth

from thrift_generated.dynamodb_access.ttypes import Type
from thrift_generated.dynamodb_access.ttypes import ValueType
from thrift_generated.dynamodb_access.ttypes import KeyValue
from thrift_generated.dynamodb_access import *
from hash_id import *
from email.mime.text import MIMEText
import threading
import Queue
from email.header import Header
from email.utils import formataddr



#EmailAlertResult registerEmailAlert(1: string userid, 2: map<string, string> parameters),
#EmailAlertResult notifyNewAnnounces(1: string city)

class UserAlert:
   def __init__(self):
      self.user_display_name = ""
      self.email = ""
      self.ads_list = []
      self.forbidden_ads = set()

class EmailAlertServiceHandler(Iface):
   def __init__(self, config):
      logging.info("Initialize EmailAlertService ...")
      self.smtp_host = config['DEFAULT']['smtp_host']
      self.smtp_port = int(config['DEFAULT']['smtp_port'])

      self.from_addr = config['DEFAULT']['from_addr']
      self.password = config['DEFAULT']['password']
      
      self.alert_tablename = config['DEFAULT']['alert_tablename']
      self.ad_tablename = config['DEFAULT']['ad_tablename']
      self.works = Queue.Queue()
      self.condition = threading.Condition()

      
      self.sock = TSocket.TSocket(config['DEFAULT']['dynamodb_host'], int(config['DEFAULT']['dynamodb_port']))

      self.transport = TTransport.TBufferedTransport(self.sock)

      protocol = TBinaryProtocol.TBinaryProtocol(self.transport)

      self.client = dynamodb_access.Client(protocol)

      self.transport.open()

   def registerEmailAlert(self, userid, parameters):
      logging.info("registering email alert for user [{}]".format(userid))

      user = auth.get_user(userid)
      alert_id_string = userid
      alert_id_string += user.email
      alert_id_string += datetime.datetime.utcnow().isoformat()
      #preparing all data to save
      values = dict()

     
      #ID 
      ID=hash_id(alert_id_string)

      idvalue = ttypes.ValueType()
      idvalue.field = str(ID)
      idvalue.fieldtype = ttypes.Type.NUMBER
      values["ID"] = idvalue

      #CITY
      city_value = ttypes.ValueType()
      city_value.field = parameters["search_city"].lower()
      city_value.fieldtype = ttypes.Type.STRING
      values["CITY"] = city_value
      
      
      #USERID
      userid_value = ttypes.ValueType()
      userid_value.field = userid
      userid_value.fieldtype = ttypes.Type.STRING
      values["USERID"] = userid_value

      #USER_DISPLAY_NAME
      if user.display_name:
         userdisplayname_value = ttypes.ValueType()
         userdisplayname_value.field = user.display_name
         userdisplayname_value.fieldtype = ttypes.Type.STRING
         values["USER_DISPLAY_NAME"] = userdisplayname_value
  
      #EMAIL
      email_value = ttypes.ValueType()
      email_value.field = user.email
      email_value.fieldtype = ttypes.Type.STRING
      values["EMAIL"] = email_value

      #PROPERTY_TYPE
      property_type_value = ttypes.ValueType()
      if parameters["prop_type"] == "1":
         property_type_value.field = "Appartement"
      else:
         property_type_value.field = "Maison"

      property_type_value.fieldtype = ttypes.Type.STRING
      values["PROPERTY_TYPE"] = property_type_value


      #SEARCH_TYPE
      search_type_value = ttypes.ValueType()
      if parameters["search_type"] == "1":
         search_type_value.field = "For sale"
      else:
         search_type_value.field = "For rent"

      search_type_value.fieldtype = ttypes.Type.STRING
      values["SEARCH_TYPE"] = search_type_value

      #ROOMS
      if "rooms" in parameters:
         rooms_value = ttypes.ValueType()
         rooms_value.field = parameters["rooms"]
         rooms_value.fieldtype = ttypes.Type.NUMBER
         values["ROOMS"] = rooms_value

      #PRICE_MIN and PRICE_MAX
      price_min_value = ttypes.ValueType()
      price_min_value.field = str(int(parameters["price_min"])*1000) if parameters["search_type"] == "1" else str(int(parameters["price_min"]))      
      price_min_value.fieldtype = ttypes.Type.NUMBER
      values["PRICE_MIN"] = price_min_value

      price_max_value = ttypes.ValueType()
      price_max_value.field = str(int(parameters["price_max"])*1000) if parameters["search_type"] == "1" else str(int(parameters["price_max"]))      
      price_max_value.fieldtype = ttypes.Type.NUMBER
      values["PRICE_MAX"] = price_max_value

 
      #AREA_MIN and AREA_MAX
      area_min_value = ttypes.ValueType()
      area_min_value.field = str(int(parameters["area_min"]))      
      area_min_value.fieldtype = ttypes.Type.NUMBER
      values["AREA_MIN"] = area_min_value

      area_max_value = ttypes.ValueType()
      area_max_value.field = str(int(parameters["area_max"]))      
      area_max_value.fieldtype = ttypes.Type.NUMBER
      values["AREA_MAX"] = area_max_value


      #DATE_OF_CREATION
      dateofcreation_value = ttypes.ValueType()
      dateofcreation_value.field = datetime.datetime.utcnow().replace(microsecond=0).isoformat()
      dateofcreation_value.fieldtype = ttypes.Type.STRING
      values["DATE_OF_CREATION"] = dateofcreation_value


      #ALERT_STATUS
      alertstatus_value = ttypes.ValueType()
      alertstatus_value.field = "ON"
      alertstatus_value.fieldtype = ttypes.Type.STRING
      values["ALERT_STATUS"] = alertstatus_value

      #URL
      url = "/liste_annonces.html?"
      url += "search_city="
      url += parameters["search_city"]
      url += "&search_type="
      url += parameters["search_type"]
      url += "&prop_type="
      url += parameters["prop_type"]
      if "rooms" in parameters:
         url += "&rooms="
         url += parameters["rooms"]

      url += "&price_min="
      url += parameters["price_min"]
      url += "&price_max="
      url += parameters["price_max"]
      
      url += "&area_min="
      url += parameters["area_min"]
      url += "&area_max="
      url += parameters["area_max"]


      url_value = ttypes.ValueType()
      url_value.field = url
      url_value.fieldtype = ttypes.Type.STRING
      values["ADS_URL"] = url_value
   
      ret = self.client.put(self.alert_tablename, values) 
      retval = EmailAlertResult()
      if ret.success:
         retval.success = True
         logging.info("Successfully put alert [{}] in table [{}]".format(str(ID),self.alert_tablename))
      else:
         retval.success = False
         retval.error = ret.error
         logging.error("Fail to put alert [{}] in table [{}]".format(str(ID), self.alert_tablename))

      return retval
      
   def my_realestate_search(self, userid):
      my_search = []
      filterexpression_search = "USERID = :ui and ALERT_STATUS <> :als"
      
      while True:
         attribute_to_get = {}

         id_value = ttypes.ValueType()
         id_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["ID"] = id_value


         city_value = ttypes.ValueType()
         city_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["CITY"] = city_value

         status_value = ttypes.ValueType()
         status_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["ALERT_STATUS"] = status_value


         property_type_value = ttypes.ValueType()
         property_type_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["PROPERTY_TYPE"] = property_type_value

         search_type_value = ttypes.ValueType()
         search_type_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["SEARCH_TYPE"] = search_type_value

         rooms_value = ttypes.ValueType()
         rooms_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["ROOMS"] = rooms_value

         #PRICE_MIN and PRICE_MAX
         price_min_value = ttypes.ValueType()
         price_min_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["PRICE_MIN"] = price_min_value

         price_max_value = ttypes.ValueType()
         price_max_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["PRICE_MAX"] = price_max_value


         #AREA_MIN and AREA_MAX
         area_min_value = ttypes.ValueType()
         area_min_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["AREA_MIN"] = area_min_value

         area_max_value = ttypes.ValueType()
         area_max_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["AREA_MAX"] = area_max_value

         url_value = ttypes.ValueType()
         url_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["ADS_URL"] = url_value

         #USER_DISPLAY_NAME
         userdisplayname_value = ttypes.ValueType()
         userdisplayname_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["USER_DISPLAY_NAME"] = userdisplayname_value
  
         expression_value = {}
         userid_value = ttypes.ValueType()
         userid_value.field =  userid
         userid_value.fieldtype = ttypes.Type.STRING
         expression_value [":ui"] = userid_value

         alertstatus_value = ttypes.ValueType()
         alertstatus_value.field =  "DELETED"
         alertstatus_value.fieldtype = ttypes.Type.STRING
         expression_value [":als"] = alertstatus_value
         query_result = self.client.scan(self.alert_tablename, attribute_to_get, filterexpression_search, expression_value)


         if query_result.result.success and query_result.values:
            logging.info('receiving {} alerts'.format(len(query_result.values)))
            my_search += query_result.values
         if query_result.scanend:
            break

      return my_search

   def ads_match(self, ads, alert):
      if ads["PROPERTY_TYPE"] != alert["PROPERTY_TYPE"]:
         logging.info('ads [{}] is not a match since property type differ {} : {}'.format(ads["ID"], ads["PROPERTY_TYPE"], alert["PROPERTY_TYPE"]))
         return False

      if ads["SEARCH_TYPE"] != alert["SEARCH_TYPE"]:
         logging.info('ads [{}] is not a match since search type differ {} : {}'.format(ads["ID"], ads["SEARCH_TYPE"], alert["SEARCH_TYPE"]))
         return False

      if "ROOMS" in ads and "ROOMS" in alert:
         if ads["ROOMS"] != alert["ROOMS"]:
            logging.info('ads [{}] is not a match since ROOMS differ {} : {}'.format(ads["ID"], ads["ROOMS"], alert["ROOMS"]))
            return False

      price = int(ads["PRICE"])
      if price < int(alert["PRICE_MIN"]) or price > int(alert["PRICE_MAX"]):
         logging.info('ads [{}] is not a match since PRICE is out of bound:price {}, min {} : max {}'.format(ads["ID"], ads["PRICE"], alert["PRICE_MIN"], alert["PRICE_MAX"]))
         return False

      area = float(ads["SURFACE"])
      if area < float(alert["AREA_MIN"]) or area > float(alert["AREA_MAX"]):
         logging.info('ads [{}] is not a match since SURFACE is out of bound:SURFACE {}, min {} : max {}'.format(ads["ID"], ads["SURFACE"], alert["AREA_MIN"], alert["AREA_MAX"]))
         return False

      logging.info('match on {}'.format(ads['ID']))
      return True

   def notifyNewAnnounces(self, city):
      with self.condition:
         self.works.put(city)
         self.condition.notifyAll()
      
     
         retval = EmailAlertResult()
         retval.success = True
         retval.error = ""
         logging.info('notification sent for city [{}]'.format(city))
         return retval

      retval = EmailAlertResult()
      retval.success = False
      retval.error = "notification not sent! for city [" + city + "]"


   def changeAlertStatus(self, alert_id, alert_status):
      id_value = ttypes.ValueType()
      id_value.field = alert_id
      id_value.fieldtype = Type.NUMBER
      item_key = KeyValue('ID', id_value)

      values = {}
      alertstatus = ttypes.ValueType()
      alertstatus.field = alert_status
      alertstatus.fieldtype = Type.STRING
      values["ALERT_STATUS"] = alertstatus
 
      logging.info("update alert status to {} for item {}".format(alert_status, alert_id))
      ret = self.client.update(self.alert_tablename, item_key, values)
      retval = EmailAlertResult()
      if ret.success:
         logging.info("successfully {} alert {}".format(alert_status, alert_id))
         retval.success = True
      else:
         logging.error(ret.error)
         retval.success = False
         retval.error = ret.error

      return retval
 
   def checkAndNotifyUsers(self, city):
      active_alert_list = []
      filterexpression_alert = "CITY = :ct and ALERT_STATUS = :als"
      while True:
         attribute_to_get = {}
         city_value = ttypes.ValueType()
         city_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["CITY"] = city_value

         #USERID
         userid_value = ttypes.ValueType()
         userid_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["USERID"] = userid_value


         property_type_value = ttypes.ValueType()
         property_type_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["PROPERTY_TYPE"] = property_type_value

         search_type_value = ttypes.ValueType()
         search_type_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["SEARCH_TYPE"] = search_type_value

         rooms_value = ttypes.ValueType()
         rooms_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["ROOMS"] = rooms_value

         #PRICE_MIN and PRICE_MAX
         price_min_value = ttypes.ValueType()
         price_min_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["PRICE_MIN"] = price_min_value

         price_max_value = ttypes.ValueType()
         price_max_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["PRICE_MAX"] = price_max_value


         #AREA_MIN and AREA_MAX
         area_min_value = ttypes.ValueType()
         area_min_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["AREA_MIN"] = area_min_value

         area_max_value = ttypes.ValueType()
         area_max_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["AREA_MAX"] = area_max_value

         #USER_DISPLAY_NAME
         userdisplayname_value = ttypes.ValueType()
         userdisplayname_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["USER_DISPLAY_NAME"] = userdisplayname_value

         #EMAIL
         email_value = ttypes.ValueType()
         email_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["EMAIL"] = email_value

         expression_value = {}
         city_value = ttypes.ValueType()
         city_value.field =  city
         city_value.fieldtype = ttypes.Type.STRING
         expression_value [":ct"] = city_value

         alertstatus_value = ttypes.ValueType()
         alertstatus_value.field =  "ON"
         alertstatus_value.fieldtype = ttypes.Type.STRING
         expression_value [":als"] = alertstatus_value
         query_result = self.client.scan(self.alert_tablename, attribute_to_get, filterexpression_alert, expression_value)


         if query_result.result.success and query_result.values:
            logging.info(' recieved {} alert'.format(len(query_result.values)))
            active_alert_list += query_result.values
         if query_result.scanend:
            break

      new_ads = []
      filter_expression_ads  = "CITY = :ct and begins_with(FIRST_TIMESTAMP, :ft)"
      datenow = datetime.datetime.utcnow().replace(microsecond=0).isoformat()
      datenow = datenow[:10]

      while True:
         attribute_to_get = {}
         idvalue =  ttypes.ValueType()
         idvalue.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["ID"] = idvalue
   
         property_type_value = ttypes.ValueType()
         property_type_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["PROPERTY_TYPE"] = property_type_value

         search_type_value = ttypes.ValueType()
         search_type_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["SEARCH_TYPE"] = search_type_value

         rooms_value = ttypes.ValueType()
         rooms_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["ROOMS"] = rooms_value

         #PRICE
         price_value = ttypes.ValueType()
         price_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["PRICE"] = price_value


         #AREA
         area_value = ttypes.ValueType()
         area_value.fieldtype = ttypes.Type.NUMBER
         attribute_to_get["SURFACE"] = area_value

         #DUPLICATES
         duplicates_value = ttypes.ValueType()
         duplicates_value.fieldtype = ttypes.Type.STRING
         attribute_to_get["DUPLICATES"] = duplicates_value


         expression_value = {}
         city_value = ttypes.ValueType()
         city_value.field =  city
         city_value.fieldtype = ttypes.Type.STRING
         expression_value [":ct"] = city_value

         first_timestamp_value = ttypes.ValueType()
         first_timestamp_value.field = datenow
         first_timestamp_value.fieldtype = ttypes.Type.STRING
         expression_value [":ft"] = first_timestamp_value
         query_result = self.client.scan(self.ad_tablename, attribute_to_get, filter_expression_ads, expression_value)

         if query_result.result.success and query_result.values:
            logging.info('found {} new announces'.format(len(query_result.values)))
            new_ads += query_result.values
         else:
            if query_result.result.success:
               logging.info("no new announces found today")
            else:
               logging.error(query_result.result.error)

         if query_result.scanend:
            break

      #prepare user data to notify with new ads
      userToNotify = dict()
      for alert in active_alert_list:
         useralert = UserAlert()
         useralert.email = alert["EMAIL"]
         useralert.user_display_name = alert["USER_DISPLAY_NAME"] if "USER_DISPLAY_NAME" in alert else ""
         userToNotify[alert["USERID"]]= useralert

      
      for ads in new_ads:
         for alert in active_alert_list:
            if self.ads_match(ads, alert) and ads["ID"] not in userToNotify[alert["USERID"]].forbidden_ads:
               userToNotify[alert["USERID"]].ads_list.append(ads["ID"])
               if "DUPLICATES" in ads:
                  duplicates_ad=ads["DUPLICATES"].split(",")
                  for announce in duplicates_ad:
                     userToNotify[alert["USERID"]].forbidden_ads.add(announce)

      smtp_server = smtplib.SMTP(self.smtp_host, self.smtp_port)  
      logging.info("log in to smtp server")
      smtp_server.login(self.from_addr, self.password)


      for userid in userToNotify:
         if userToNotify[userid].ads_list:

            recipients = [userToNotify[userid].email]
            isplural = True if len(userToNotify[userid].ads_list) > 1 else False

            subject_msg = '[Surfyn] '
            subject_msg += str(len(userToNotify[userid].ads_list))
            if isplural:
               subject_msg += u' nouvelles annonces immobilières'
            else:
               subject_msg += u' nouvelle annonce immobilière'
            
            body_msg = 'Bonjour '
            if userToNotify[userid].user_display_name:
               pos = userToNotify[userid].user_display_name.find(' ')
               name = userToNotify[userid].user_display_name[:pos] if pos != -1 else userToNotify[userid].user_display_name 
               body_msg += name
            body_msg += '\n'
            body_msg += str(len(userToNotify[userid].ads_list))
            if isplural:
               body_msg += u' nouvelles annonces sur https://surfyn.fr'
               body_msg += u' correspondent à vos critères de recherche immobilière\n'
            else:
               body_msg += u' nouvelle annonce sur https://surfyn.fr'
               body_msg += u' correspond à vos critères de recherche immobilière\n'

            for ad in userToNotify[userid].ads_list:
               url = 'https://surfyn.fr/annonce_detaille.html?'+ str(ad)
               body_msg += url
               body_msg += '\n'

            body_msg += u"\n\n A votre service\nL' équipe Surfyn"
            msg = MIMEText(body_msg, _charset='utf-8')
            msg['Subject'] = subject_msg
            msg['From'] = formataddr((str(Header('Surfyn', 'utf-8')), self.from_addr))
            msg['To'] = userToNotify[userid].email

            logging.info(u'sending msg:\n{} to user [{}] '.format(body_msg, userToNotify[userid].email))
            smtp_server.sendmail(self.from_addr, recipients, msg.as_string())
         else:
            logging.info('No alert for user [{}]'.format(userid)) 

      smtp_server.quit()

def do_work(handler):
   while True:
      with handler.condition:
         try:
            city = handler.works.get_nowait()
            if city:
               handler.checkAndNotifyUsers(city)
               handler.works.task_done()
         except Queue.Empty:
            pass
         handler.condition.wait()

if __name__ == '__main__':
   cred = credentials.Certificate("surfyn-firebase-adminsdk.json")
   firebase_admin.initialize_app(cred)

   now = datetime.datetime.now()
   log_filename = "emailalertservice_" + now.strftime("%Y-%m-%d-%H-%M-%S") + ".log"
   logging.basicConfig(filename=log_filename,level=logging.INFO, format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')

   logging.info("Starting Email Alert service")
   config = configparser.ConfigParser()
   config.read('config.ini')
   host = config['DEFAULT']['host']
   port = int(config['DEFAULT']['port'])
 
   handler = EmailAlertServiceHandler(config)

   notifier = threading.Thread(name='Notifier', target=do_work, args=(handler,))
   notifier.start()
   processor = Processor(handler)
   transport = TSocket.TServerSocket(host, port)
   tfactory = TTransport.TBufferedTransportFactory()
   pfactory = TBinaryProtocol.TBinaryProtocolFactory()

   server = TServer.TThreadedServer(processor, transport, tfactory, pfactory)

   logging.info('Waiting for request ...')
   server.serve()
   logging.info('done.')

