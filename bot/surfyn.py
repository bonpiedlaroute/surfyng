from annoy import AnnoyIndex
from scipy import spatial
from nltk import ngrams
import random, json, glob, os, codecs, random
import numpy as np

from thrift_generated.dynamodb_access.ttypes import Type
from thrift_generated.dynamodb_access.ttypes import ValueType
from thrift_generated.dynamodb_access.ttypes import KeyValue
from Serializer import *

tablename ="FR_PROPERTIES"
similar_announce_column = "TF_SIMILAR_ANNOUNCE"

# data structures
file_index_to_file_name = {}
file_index_to_file_vector = {}
chart_image_positions = {}

# config
dims = 2048
n_nearest_neighbors = 5
similarity_threshold=0.9
trees = 10000
infiles = glob.glob('image_vectors/*.npz')

# build ann index
t = AnnoyIndex(dims)
for file_index, i in enumerate(infiles):
   file_vector = np.loadtxt(i)
   file_name = os.path.basename(i).split('.')[0]
   file_index_to_file_name[file_index] = file_name
   file_index_to_file_vector[file_index] = file_vector
   t.add_item(file_index, file_vector)
t.build(trees)

# create a nearest neighbors json file for each input
if not os.path.exists('seloger_nearest_neighbors'):
   os.makedirs('seloger_nearest_neighbors')

serializer = Serializer('localhost', 5050)

for i in file_index_to_file_name.keys():
   master_file_name = file_index_to_file_name[i]
   master_vector = file_index_to_file_vector[i]
   master_announce_id, master_photo_index = master_file_name.split('_')

   print (master_announce_id)
   master_announce_id_value = ValueType()
   master_announce_id_value.field = master_announce_id
   master_announce_id_value.fieldtype = Type.NUMBER
   item_key = KeyValue('ID', master_announce_id_value)

   attributes_to_get = {}
   similar_announce_value = ValueType()
   attributes_to_get['CITY'] = similar_announce_value

   query_result = serializer.client.get(tablename, item_key, attributes_to_get)
   similar_announce_set = set()
   if query_result.result.success and similar_announce_column in query_result.values.keys():
      similar_announce_set = set(query_result.values[similar_announce_column].split(','))

   values = {}

   named_nearest_neighbors = []
   nearest_neighbors = t.get_nns_by_item(i, n_nearest_neighbors)
   for j in nearest_neighbors:
      neighbor_file_name = file_index_to_file_name[j]
      if master_file_name != neighbor_file_name:
         neighbor_file_vector = file_index_to_file_vector[j]

         similarity = 1 - spatial.distance.cosine(master_vector, neighbor_file_vector)
         rounded_similarity = int((similarity * 10000)) / 10000.0

         if rounded_similarity >= similarity_threshold:
            neighbor_announce_id, neighbor_photo_index = neighbor_file_name.split('_')
            similar_announce_set.add(neighbor_announce_id)
            named_nearest_neighbors.append({
               'filename': neighbor_file_name,
               'similarity': rounded_similarity
            })

   if len(named_nearest_neighbors) > 0:
      similar_announce_set_value = ValueType()
      similar_announce_set_value.field = ','.join(similar_announce_set)
      similar_announce_set_value.fieldtype = Type.STRING

      values[similar_announce_column] = similar_announce_set_value

      print ("update item {}".format(master_announce_id))
      ret = serializer.client.update(tablename, item_key, values)
      print (ret)

   with open('seloger_nearest_neighbors/' + master_file_name + '.json', 'w') as out:
      json.dump(named_nearest_neighbors, out)

serializer.close()
