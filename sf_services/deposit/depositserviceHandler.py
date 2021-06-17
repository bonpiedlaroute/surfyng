# author(s): Stephane Fedim
# All rights reserved

import sys
sys.path.append('thrift_generated/depositservice')
sys.path.append('../../bot')
sys.path.append('../../bot/thrift_generated')

from hash_id import *
from email.header import Header
from thrift.server import TServer
from email.utils import formataddr
from email.mime.text import MIMEText
from thrift.protocol import TBinaryProtocol
from firebase_admin import credentials, auth
from thrift.transport import TSocket, TTransport
from dynamodb_access import dynamodb_access, ttypes
from dynamodb_access.ttypes import Type, ValueType, KeyValue
from thrift_generated.depositservice.ttypes import DepositResult
from thrift_generated.depositservice.deposit_service import Iface
from thrift_generated.depositservice.deposit_service import Processor
from inseecode_postalcode import *


import firebase_admin
import configparser
import unidecode
import threading
import datetime
import smtplib
import logging
import Queue
import pytz
import json


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

        self.smtp_host = config['EMAIL']['smtp_host']
        self.smtp_port = config['EMAIL']['smtp_port']

        self.from_addr = config['EMAIL']['from_addr']
        self.to_addrs = config['EMAIL']['to_addr']
        self.password = config['EMAIL']['password']
        
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
        prop_type = "maison" if data['prop_type'] == 1 else "appartement"
        search_type = "achat" if data['search_type'] == 1 else "location"
        announce_link_value = ttypes.ValueType()
        announce_link_value.field = "/annonce/{}/{}-{}-pieces/{}-{}?{}".format(search_type, prop_type, data['rooms'], data['city'].lower(), postalcodeByCity[data['city'].lower()], ID)
        announce_link_value.fieldtype = ttypes.Type.STRING
        values['ANNOUNCE_LINK'] = announce_link_value

        # ANNOUNCE SOURCE
        announce_source_value = ttypes.ValueType()
        announce_source_value.field = "surfyn"
        announce_source_value.fieldtype = ttypes.Type.STRING
        values['ANNOUNCE_SOURCE'] = announce_source_value

        # ANNOUNCE TITLE
        title = data['title']
        title = title.replace('\n', ' ')
        title = title.replace('\t', ' ')
        title = title.replace('\\', ' ')
        title = title.replace('\r', ' ')
        title_value = ttypes.ValueType()
        title_value.field = title
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
        image_for_announce = None
        if data.has_key('image1'):
            image_for_announce = data['image1']
        elif data.has_key('image2'):
            image_for_announce = data['image2']
        elif data.has_key('image3'):
            image_for_announce = data['image3']

        announce_image_value.field = data['video'] if data.has_key('video') else image_for_announce
        announce_image_value.fieldtype = ttypes.Type.STRING
        values['IMAGE'] = announce_image_value

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
        description = data['description']
        description = description.replace('\n', ' ')
        description = description.replace('\t', ' ')
        description = description.replace('\\', ' ')
        description = description.replace('\r', ' ')
        description_value = ttypes.ValueType()
        description_value.field = description
        description_value.fieldtype = ttypes.Type.STRING
        values['AD_TEXT_DESCRIPTION'] = description_value

        # IMAGE OR VIDEO
        if data.has_key('video'):
            video = data['video']
            video_value = ttypes.ValueType()
            video_value.field = video
            video_value.fieldtype = ttypes.Type.STRING
            values['VIDEO'] = video_value
        else:
            images = {
                'image1': data['image1'] if data.has_key('image1') else '',
                'image2': data['image2'] if data.has_key('image2') else '',
                'image3': data['image3'] if data.has_key('image3') else '',
                'image4': data['image4'] if data.has_key('image4') else '',
                'image5': data['image5'] if data.has_key('image5') else ''
            }
            image = str(images)

            image_value = ttypes.ValueType()
            image_value.field = image
            image_value.fieldtype = ttypes.Type.STRING
            values['ANNOUNCE_IMAGE'] = image_value

            # IMAGE COUNT
            image_count = ttypes.ValueType()
            image_count.field = str(
                len(filter(lambda item: 'image' in item, list(data.keys()))))
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

        # SOURCES
        sources_value = ttypes.ValueType()
        sources_value.field = "surfyn"
        sources_value.fieldtype = ttypes.Type.STRING
        values['SOURCES'] = sources_value

        # DELETED STATUS
        deleted_value = ttypes.ValueType()
        deleted_value.field = 'ON'
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
            userdisplayname_value.field = unidecode.unidecode(
                user.display_name)
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
            
            logging.info('Start sending confirmation mail')
            smtp_host_port = '{}:{}'.format(self.smtp_host, self.smtp_port)
            smtp_server = smtplib.SMTP(smtp_host_port)
            logging.info('Logging into SMTP Server')
            smtp_server.login(self.from_addr, self.password)

            msg_to_send = 'Votre annonce est en ligne sur Surfyn'
            
            recipients = []
            recipients.append(user.email)
            message = MIMEText(msg_to_send, _charset='utf-8')
            message['Subject'] = 'Confirmation: Annonce deposee avec succes'
            message['From'] = formataddr((str(Header('Surfyn', 'utf-8')), self.from_addr))
            message['To'] = user.email

            smtp_server.sendmail(self.from_addr, recipients, message.as_string())

            smtp_server.quit()
            
            logging.info('Email sent successfully')

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
        id_value.fieldtype = ttypes.Type.NUMBER
        item_key = KeyValue('ID', id_value)

        values = {}
        announce_status = ttypes.ValueType()
        announce_status.field = 'OFF'
        announce_status.fieldtype = Type.STRING
        values['AD_STATUS'] = announce_status

        logging.info('Delete announce with ID: {}'.format(announce_id))

        result = self.client.remove(self.summary_tablename, item_key)
        result_deposit = self.client.update(self.deposit_tablename, item_key, values)

        return_value = DepositResult()
        if result.success and result_deposit.success:
            logging.info('Successfully deleted announce {} of user {}'.format(
                announce_id, user_id))
            return_value.success = True
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
        filter_expression = 'USERID = :uid and AD_STATUS = :ads'

        attributes_to_get = {}

        while True:
            # ID
            id_value = ttypes.ValueType()
            id_value.fieldtype = ttypes.Type.NUMBER
            attributes_to_get['ID'] = id_value

            # ANNOUNCE TITLE
            announce_title = ttypes.ValueType()
            announce_title.fieldtype = ttypes.Type.STRING
            attributes_to_get['ANNOUNCE_TITLE'] = announce_title

            # ANNOUNCE_LINK
            announce_link = ttypes.ValueType()
            announce_link.fieldtype = ttypes.Type.STRING
            attributes_to_get['ANNOUNCE_LINK'] = announce_link

            # CITY
            city_value = ttypes.ValueType()
            city_value.fieldtype = ttypes.Type.STRING
            attributes_to_get['CITY'] = city_value

            # PROPERTY TYPE
            property_type = ttypes.ValueType()
            property_type.fieldtype = ttypes.Type.STRING
            attributes_to_get['PROPERTY_TYPE'] = property_type

            # SEARCH TYPE
            search_type = ttypes.ValueType()
            search_type.fieldtype = ttypes.Type.STRING
            attributes_to_get['SEARCH_TYPE'] = search_type

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

            # IMAGES
            image_value = ttypes.ValueType()
            image_value.fieldtype = ttypes.Type.STRING
            attributes_to_get['ANNOUNCE_IMAGE'] = image_value

            # VIDEO
            video_value = ttypes.ValueType()
            video_value.fieldtype = ttypes.Type.STRING
            attributes_to_get['VIDEO'] = video_value

            # TIMESTAMP
            timestamp_value = ttypes.ValueType()
            timestamp_value.fieldtype = ttypes.Type.STRING
            attributes_to_get['TIMESTAMP'] = timestamp_value

            expression_value = {}
            userid_value.field = user_id
            expression_value[':uid'] = userid_value

            ad_status_value = ttypes.ValueType()
            ad_status_value.field = 'ON'
            ad_status_value.fieldtype = ttypes.Type.STRING
            expression_value[':ads'] = ad_status_value

            query_result = self.client.scan(
                self.deposit_tablename, attributes_to_get, filter_expression, expression_value)

            if query_result.result.success and query_result.values:
                logging.info('Received {} announce(s)'.format(
                    len(query_result.values)))
                active_announces += query_result.values
            if query_result.scanend:
                break
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
