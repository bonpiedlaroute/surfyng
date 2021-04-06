# author(s): Stephane Fedim
# All rights reserved

import sys

sys.path.append('thrift_generated/depositservice')
sys.path.append('../../bot')
sys.path.append('../../bot/thrift_generated/dynamodb_access')

from thrift_generated.depositservice.deposit_service import Processor
from thrift_generated.depositservice.deposit_service import Iface
from thrift_generated.depositservice.ttypes import DepositResult

from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

from firebase_admin import credentials, auth

from thrift_generated.dynamodb_access.ttypes import Type, ValueType, KeyValue

from hash_id import *

import json
import logging
import datetime
import configparser
import firebase_admin

class UserDeposit:
	self.user_display_name = ""
	self.email = ""
	self.ads_list = []
	self.forbidden_ads = set()


class DepositServiceHandler(Iface):
	def __init__(self, config):
		logging.info('Initialize DepositService...')

		self.deposit_table = config['DEFAULT']['deposit_tablename']
		self.works = Queue.Queue()
		self.condition = threading.Condition()

		self.sock = TSocket.TSocket(config['DEFAULT']['dynamodb_host'], int(config['DEFAULT']['dynamodb_port']))
		self.transport = TTransport.TBufferedTransfport(self.sock)

		protocol = TBinaryProtocol.TBinaryProtocol(self.transport)

		self.client = dynamodb_access.client(protocol)

		self.transport.open()

	def annouce_deposit(self, user_id, data):
		logging.info('Deposit announce of user {}'.format(user_id))

		user = auth.get_user(user_id)
		deposit_id = user_id + user.email + datetime.datetime.utcnow().isoformat()

		data = json.loads(data)
		values = {}

		# ID
		ID = hash_id(deposit_id)

		id_value = ttypes.ValueType()
		id_value.field = str(ID)
		id_value.fieldtype = ttypes.Type.NUMBER
		values['ID'] = id_value

		# TITLE
		title_value = ttypes.ValueType()
		title_value.field = data['title']
		title_value.fieldtype = ttypes.Type.STRING
		values['TITLE'] = title_value

		# CITY
		city_value = ttypes.ValueType()
		city_value.field = "puteaux"
		city_value.fieldtype = ttypes.Type.STRING
		values['CITY'] = city_value

		USERID
		userid_value = ttypes.ValueType()
		userid_value.field = user_id
		userid_value.fieldtype = ttypes.Type.STRING
		values['USERID'] = userid_value

		# USER_DISPLAY_NAME
		if user.display_name:
			userdisplayname_value = ttypes.ValueType()
			userdisplayname_value.field = user.display_name
			userdisplayname_value.fieldtype = ttypes.Type.STRING
			values["USER_DISPLAY_NAME"] = userdisplayname_value

		# EMAIL
		email_value = ttypes.ValueType()
		email_value.field = user.email
		email_value.fieldtype = ttypes.Type.STRING
		values["EMAIL"] = email_value

		# PROPERTY_TYPE
		property_type_value = ttypes.ValueType()
		property_type_value.field = "Maison" if data['property_type'] == 1 else "Appartement"
		property_type_value.fieldtype = ttypes.Type.STRING
		values['PROPERTY_TYPE'] = property_type_value

		# SEARCH_TYPE
		search_type_value = ttypes.ValueType()
		search_type_value.field = "For sale" if data['search_type'] == 1 else "For rent"
		search_type_value.fieldtype = ttypes.Type.STRING
		values['SEARCH_TYPE'] = search_type_value

		# ROOMS
		rooms_value = ttypes.ValueType()
		rooms_value.field = data['rooms']
		rooms_value.fieldtype = ttypes.Type.NUMBER
		values['ROOMS'] = rooms_value

		# PRICE
		price_value = ttypes.ValueType()
		price_value.field = data['price']
		price_value.fieldtype = ttypes.Type.NUMBER
		values['PRICE'] = price_value

		# AREA
		area_value = ttypes.ValueType()
		area_value.field = data['area']
		area_value.fieldtype = ttypes.Type.NUMBER
		values['AREA'] = area_value

		# DELETED STATUS
		deleted_value = ttypes.ValueType()
		deleted_value.field = 'OFF'
		deleted_value.fieldtype = ttypes.Type.STRING
		values['DELETED_STATUS'] = deleted_value

		# IMAGE
		images = {
			'image1': data['image1'],
			'image2': data['image2'],
			'image3': data['image3'],
			'image4': data['image4']
		}
		image = str(images)

		image_value = ttypes.ValueType()
		image_value.field = image
		image_value.fieldtype = ttypes.Type.STRING
		values['IMAGE'] = image_value

		result = self.client.put(self.deposit_tablename, values)
		return_value = DepositResult()

		if result.success:
			return_value.success = True
			logging.info('Successful put announce {} in table {}'.format(ID, self.deposit_tablename))
		else:
			return_value.success = False
			return_value.error = result.error
			logging.error('Fail to put announce {} in table {}'.format(ID, self.deposit_tablename))

		return return_value

if __name__ == '__main__':
	app = credentials.Certificate('surfyn-firebase-adminsdk.json')
	firebase_admin.initialize_app(app)

	now = datetime.datetime.now()
	log_filename = "deposit_{}.log".format(now.strftime('%Y-%m-%d-%H-%M-%S'))
	logging.basicConfig(filename=log_filename, level=logging.INFO, format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')

	logging.info('Starting deposit server')
	config = configparser.ConfigParser()
	config.read('config.ini')
	host = config['DEFAULT']['host']
	port = config['DEFAULT']['port']

	handler = DepositServiceHandler(config)
	processor = Processor(handler)
	transport = TSocket.TServerSocket(host, port)
	tfactory = TTransport.TBufferedTransfport()
	pfactory = TBinaryProtocol.TBinaryProtocol()

	server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

	logging.info('Waiting for request...')
	server.serve()
	logging.info('Done.')
