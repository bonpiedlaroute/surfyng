# author(s): Stephane Fedim
# All rights reserved

import sys
sys.path.append('thrift_generated/depositservice')
sys.path.append('../../bot')
sys.path.append('../../bot/thrift_generated')

import json
import pytz
import Queue
import logging
import datetime
import threading
import unidecode
import configparser
import firebase_admin
from thrift_generated.depositservice.deposit_service import Processor
from thrift_generated.depositservice.deposit_service import Iface
from thrift_generated.depositservice.ttypes import DepositResult
from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
from firebase_admin import credentials, auth
from hash_id import *
from dynamodb_access import dynamodb_access, ttypes
from dynamodb_access.ttypes import Type, ValueType, KeyValue


class UserDeposit:
    def __init__(self):
        self.user_display_name = ""
        self.email = ""
        self.ads_list = []
        self.forbidden_ads = set()


class DepositServiceHandler(Iface):
    def __init__(self, config):
        logging.info('Initialize DepositService...')

        self.deposit_tablename = config['DEFAULT']['deposit_tablename']
        self.summary_tablename = config['DEFAULT']['summary_tablename']
        
        self.works = Queue.Queue()
        self.condition = threading.Condition()

        self.sock = TSocket.TSocket(config['DEFAULT']['dynamodb_host'], int(
            config['DEFAULT']['dynamodb_port']))
        self.transport = TTransport.TBufferedTransport(self.sock)

        protocol = TBinaryProtocol.TBinaryProtocol(self.transport)

        self.client = dynamodb_access.Client(protocol)

        self.transport.open()

    def announce_deposit(self, user_id, data):
        """
            Function used to deposit an announce on dynamoDB server
            params:
                @user_id: User who places the announce
                @data: Dictionnary containing data of announce
        """

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

        # ANNOUNCE LINK
        announce_link_value = ttypes.ValueType()
        announce_link_value.field = "https://surfyn.fr"
        announce_link_value.fieldtype = ttypes.Type.STRING
        values['ANNOUNCE_LINK'] = announce_link_value
        
        # ANNOUNCE SOURCE
        announce_source_value = ttypes.ValueType()
        announce_source_value.field = "Surfyn"
        announce_source_value.fieldtype = ttypes.Type.STRING
        values['ANNOUNCE_SOURCE'] = announce_source_value

        # ANNOUNCE TITLE
        title_value = ttypes.ValueType()
        title_value.field = data['title']
        title_value.fieldtype = ttypes.Type.STRING
        values['ANNOUNCE_TITLE'] = title_value
        
        # CITY
        city_value = ttypes.ValueType()
        city_value.field = data['city']
        city_value.fieldtype = ttypes.Type.STRING
        values['CITY'] = city_value

        # PROPERTY_TYPE
        property_type_value = ttypes.ValueType()
        property_type_value.field = "Maison" if data['prop_type'] == 1 else "Appartement"
        property_type_value.fieldtype = ttypes.Type.STRING
        values['PROPERTY_TYPE'] = property_type_value

        # SEARCH_TYPE
        search_type_value = ttypes.ValueType()
        search_type_value.field = "For sale" if data['search_type'] == 1 else "For rent"
        search_type_value.fieldtype = ttypes.Type.STRING
        values['SEARCH_TYPE'] = search_type_value

        # TIMESTAMP
        utc_now = datetime.datetime.now(pytz.utc)
        utc_now = utc_now.strftime("%Y-%m-%dT%H:%M:%S")
        timestamp_value = ttypes.ValueType()
        timestamp_value.field = utc_now
        timestamp_value.fieldtype = ttypes.Type.STRING
        values['TIMESTAMP'] = timestamp_value

        # FIRST_TIMESTAMP
        first_timestamp_value = ttypes.ValueType()
        first_timestamp_value.field = utc_now
        first_timestamp_value.fieldtype = ttypes.Type.STRING
        values['FIRST_TIMESTAMP'] = first_timestamp_value

        # ANNOUNCE IMAGE
        announce_image_value = ttypes.ValueType()
        announce_link_value.field = data['image1']
        announce_link_value.fieldtype = ttypes.Type.STRING

        # PRICE
        price_value = ttypes.ValueType()
        price_value.field = data['price']
        price_value.fieldtype = ttypes.Type.NUMBER
        values['PRICE'] = price_value

        # SURFACE
        area_value = ttypes.ValueType()
        area_value.field = data['surface']
        area_value.fieldtype = ttypes.Type.NUMBER
        values['SURFACE'] = area_value

        # CONSTRUCTION_YEAR
        if data.has_key('construction_year'):
            construction_year_value = ttypes.ValueType()
            construction_year_value.field = str(data['construction_year'])
            construction_year_value.fieldtype = ttypes.Type.STRING
            values['CONSTRUCTION_YEAR'] = construction_year_value

        # ROOMS
        rooms_value = ttypes.ValueType()
        rooms_value.field = str(data['rooms'])
        rooms_value.fieldtype = ttypes.Type.NUMBER
        values['ROOMS'] = rooms_value

        # BEDROOMS
        bedrooms_value = ttypes.ValueType()
        bedrooms_value.field = str(data['bedrooms'])
        bedrooms_value.fieldtype = ttypes.Type.STRING
        values['BEDROOMS'] = bedrooms_value

        # TYPE OF HEATING
        type_heating_value = ttypes.ValueType()
        type_heating_value.field = data['type_of_heating']
        type_heating_value.fieldtype = ttypes.Type.STRING
        values['TYPE_OF_HEATING'] = type_heating_value

        # CELLAR
        cellar_value = ttypes.ValueType()
        cellar_value.field = str(data['cellar'])
        cellar_value.fieldtype = ttypes.Type.STRING
        values['CELLAR'] = cellar_value

        # FLOOR
        if data.has_key('floor'):
            floor_value = ttypes.ValueType()
            floor_value.field = str(data['floor'])
            floor_value.fieldtype = ttypes.Type.STRING
            values['FLOOR'] = floor_value

        # PARKING
        parking_value = ttypes.ValueType()
        parking_value.field = data['parking']
        parking_value.fieldtype = ttypes.Type.STRING
        values['PARKING'] = parking_value

        # LAND_SURFACE
        if data.has_key('land_surface'):
            land_surface_value = ttypes.ValueType()
            land_surface_value.field = str(data['land_surface'])
            land_surface_value.fieldtype = ttypes.Type.STRING
            values['LAND_SURFACE'] = land_surface_value

        # LIFT
        lift_value = ttypes.ValueType()
        lift_value.field = data['lift']
        lift_value.fieldtype = ttypes.Type.STRING
        values['LIFT'] = lift_value

        # BALCONY
        balcony_value = ttypes.ValueType()
        balcony_value.field = data['balcony']
        balcony_value.fieldtype = ttypes.Type.STRING
        values['BALCONY'] = balcony_value

        # DESCRIPTION
        description_value = ttypes.ValueType()
        description_value.field = data['description']
        description_value.fieldtype = ttypes.Type.STRING
        values['AD_TEXT_DESCRIPTION'] = description_value

        # IMAGE
        images = {
            'image1': data['image1'],
            'image2': data['image2'],
            'image3': data['image3'],
            'image4': data['image4'],
            'image5': data['image5']
        }
        image = str(images)

        image_value = ttypes.ValueType()
        image_value.field = image
        image_value.fieldtype = ttypes.Type.STRING
        values['IMAGE'] = image_value

        # IMAGE COUNT
        image_count = ttypes.ValueType()
        image_count.field = str(len(filter(lambda item: 'image' in item, list(data.keys()))))
        image_count.fieldtype = ttypes.Type.STRING
        values['IMAGE_COUNT'] = image_count

        # ADDRESS
        address_value = ttypes.ValueType()
        address_value.field = unidecode.unidecode(data['address'])
        address_value.fieldtype = ttypes.Type.STRING
        values['ADDRESS'] = address_value

        # SOURCE LOGO
        source_logo_value = ttypes.ValueType()
        source_logo_value.field = "data/surfyn.svg"
        source_logo_value.fieldtype = ttypes.Type.STRING
        values['SOURCE_LOGO'] = source_logo_value
        
        # DELETED STATUS
        deleted_value = ttypes.ValueType()
        deleted_value.field = 'OFF'
        deleted_value.fieldtype = ttypes.Type.STRING
        values['AD_STATUS'] = deleted_value             # Can be ON, OFF
        
        result_summary = self.client.put(self.summary_tablename, values)
        
        # USERID
        userid_value = ttypes.ValueType()
        userid_value.field = user_id
        userid_value.fieldtype = ttypes.Type.STRING
        values['USERID'] = userid_value


        # USER_DISPLAY_NAME
        if user.display_name:
            userdisplayname_value = ttypes.ValueType()
            userdisplayname_value.field = unidecode.unidecode(user.display_name)
            userdisplayname_value.fieldtype = ttypes.Type.STRING
            values["USER_DISPLAY_NAME"] = userdisplayname_value

        # EMAIL
        email_value = ttypes.ValueType()
        email_value.field = user.email
        email_value.fieldtype = ttypes.Type.STRING
        values["EMAIL"] = email_value

        # PHONE NUMBER
        if data.has_key('phone'):
            announce_phone_number = ttypes.ValueType()
            announce_phone_number.field = str(data['phone'])
            announce_phone_number.fieldtype = ttypes.Type.STRING
            values['PHONE_NUMBER'] = announce_phone_number

        result = self.client.put(self.deposit_tablename, values)

        logging.info('Data send to dynamodb')

        return_value = DepositResult()

        if result.success and result_summary.success:
            return_value.success = True
            logging.info('Successful put announce {} in table {} and table {}'.format(
                ID, self.deposit_tablename, self.summary_tablename))
        elif not result.success:
            return_value.success = False
            return_value.error = result.error
            logging.error('Fail to put announce {} in table {}'.format(
                ID, self.deposit_tablename))
            logging.error(result.error)
        elif not result_summary.success:
            return_value.success = False
            return_value.error = result.error
            logging.error('Fail to put announce {} in table {}'.format(
                ID, self.summary_tablename))
            logging.error(result.error)
                
        return return_value

    def delete_announce(self, user_id, announce_id):
        """
            Function used to delete an announce. The only thing to do is to change AD_STATUS of announce.
            params:
                @user_id: User ID of the owner of ad
                @announce_id: ad ID
        """
        
        id_value = ttypes.ValueType()
        id_value.field = announce_id
        id_value.fieldtype = Type.STRING
        item_key = KeyValue('ID', id_value)

        values = {}
        announce_status = ttypes.ValueType()
        announce_status.field = 'OFF'
        announce_status.fieldtype = Type.STRING
        values['AD_STATUS'] = announce_status

        logging.info('Delete announce with ID: {}'.format(announce_id))
        
        result = self.client.update(self.tablename, item_key, values)
        return_value = DepositResult()
        if result.success:
            logging.info('Successfully deleted announce {} of user {}'.format(announce_id, user_id))
            return_value.success = TBufferedTransport
        else:
            logging.error(result.error)
            return_value.success = False
            return_value.error = result.error
        
        return return_value
    
    def fetch_user_announces(self, user_id):
        """
            Function used to fetch all announces of a user
            @user: User ID
        """
        active_announces = []
        filter_expression = 'USERD_ID = :uid and AD_STATUS = :ads'
        
        attributes_to_get = {}
        
        while True:
            # ANNOUNCE TITLE
            announce_title = ttypes.ValueType()
            announce_title.fieldtype = ttypes.Type.STRING
            attributes_to_get['ANNOUNCE_TITLE'] = announce_title

            # PROPERTY TYPE
            property_type = ttypes.ValueType()
            property_type.fieldtype = ttypes.Type.STRING
            attributes_to_get['PROPERTY_TYPE'] = property_type

            # ROOMS
            rooms_value = ttypes.ValueType()
            rooms_value.fieldtype = ttypes.Type.NUMBER
            attributes_to_get['ROOMS'] = rooms_value

            # BEDROOMS
            bedrooms_value = ttypes.ValueType()
            bedrooms_value.fieldtype = ttypes.Type.STRING
            attributes_to_get['BEDROOMS'] = bedrooms_value

            # SURFACE
            surface_value = ttypes.ValueType()
            surface_value.fieldtype = ttypes.Type.NUMBER
            attributes_to_get['SURFACE'] = surface_value

            # USER ID
            userid_value = ttypes.ValueType()
            userid_value.fieldtype = ttypes.Type.STRING
            attributes_to_get['USERID'] = userid_value

            # PRICE
            price_value = ttypes.ValueType()
            price_value.fieldtype = ttypes.Type.NUMBER
            attributes_to_get['PRICE'] = price_value

            expression_value = {}
            userid_value.field = user_id
            expression_value[':uid'] = userid_value

            ad_status_value = ttypes.ValueType()
            ad_status_value.field = 'ON'
            ad_status_value.fieldtype = ttypes.Type.STRING
            expression_value[':ads'] = ad_status_value

            query_result = self.client.scan(self.deposit_tablename, attributes_to_get, filter_expression, expression_value)

            if query_result.result.success and query_result.values:
                logging.info('Received {} alert'.format(len(query_result.values)))
                active_announces == query_result.values
            if query_result.scanend:
                break
        print(active_announces)
        
        return active_announces


if __name__ == '__main__':
    app = credentials.Certificate('surfyn-firebase-adminsdk.json')
    firebase_admin.initialize_app(app)

    now = datetime.datetime.now()
    log_filename = "deposit_{}.log".format(now.strftime('%Y-%m-%d-%H-%M-%S'))
    logging.basicConfig(filename=log_filename, level=logging.INFO,
                        format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')

    logging.info('Starting deposit server')
    config = configparser.ConfigParser()
    config.read('config.ini')
    host = str(config['DEFAULT']['host'])
    port = int(config['DEFAULT']['port'])

    handler = DepositServiceHandler(config)
    processor = Processor(handler)
    transport = TSocket.TServerSocket(host, port)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TServer.TThreadedServer(processor, transport, tfactory, pfactory)

    logging.info('Waiting for request...')
    logging.info('Server listening on port {}'.format(port))
    server.serve()
