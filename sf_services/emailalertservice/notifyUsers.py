#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2019 
# author(s): Noel Tchidjo
# All rights reserved

import glob
import configparser
import sys
sys.path.append('thrift_generated/emailalertservice')


from thrift_generated.emailalertservice.emailalertservice import Client


from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
from thrift import Thrift


if __name__ == '__main__':

   try:
      config = configparser.ConfigParser()
      config.read('config_notify.ini')

      ip = config['DEFAULT']['emailalertservice_ip']
      port = int(config['DEFAULT']['emailalertservice_port'])

      # Make socket
      mysock = TSocket.TSocket(ip, port)

      # Buffering is critical. Raw sockets are very slow
      transport = TTransport.TBufferedTransport(mysock)

      # Wrap in a protocol
      protocol = TBinaryProtocol.TBinaryProtocol(transport)

      # Create a client to use the protocol encoder
      client = Client(protocol)

      # Connect!
      transport.open()

      if len(sys.argv) != 2:
         print ("python notifyUsers city")
         sys.exit()
      
      city = sys.argv[1]
      print('Sending notification for {}'.format(city))
      ret = client.notifyNewAnnounces(city)


      if ret.success:
         print "Successfully notification for {}".format(city)
      else:
         print "Failed to send notification to {}".format(city)


   except Thrift.TException as tx:
      print('%s' % tx.message) 
