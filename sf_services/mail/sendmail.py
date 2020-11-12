#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2020 
# author(s): Noel Tchidjo
# All rights reserved


import glob
import sys
sys.path.append('thrift_generated/sendmail')

from thrift_generated.sendmail.sendemailservice import Processor
from thrift_generated.sendmail.sendemailservice import Iface
from thrift_generated.sendmail.ttypes import SendEmailResult


from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
import logging
from datetime import datetime
import configparser
import os
import smtplib





class SendMailHandler(Iface):
   def __init__(self, config):
      logging.info("Initialize SendMailService ...")
      self.smtp_host = config['DEFAULT']['smtp_host']
      self.smtp_port = config['DEFAULT']['smtp_port']

      self.from_addr = config['DEFAULT']['from_addr']
      self.to_addrs = config['DEFAULT']['to_addrs']
      self.password = config['DEFAULT']['password']

   def sendemailtosurfyn(self, sender_email, subject, msg):
      logging.info("start sending email to Surfyn")
      smtp_host_port = self.smtp_host + ':' + self.smtp_port
      smtp_server = smtplib.SMTP(smtp_host_port)  
      smtp_server.starttls()
      logging.info("log in to smtp server")
      smtp_server.login(self.from_addr, self.password)

      msg_to_send = 'De la part de :' + sender_email + '\n\n '+ msg

      message = 'Subject: {}\n\n{}'.format(subject, msg_to_send) 
      logging.info("sending msg :\n\n"+message)
      recipients = self.to_addrs.split(",")      
      smtp_server.sendmail(self.from_addr, recipients, message)


      smtp_server.quit()

      logging.info("send email finished!")

      retval = SendEmailResult()
      retval.success = True
      retval.error = ""

      return retval

if __name__ == '__main__':

   now = datetime.now()
   log_filename = "sendmail_" + now.strftime("%Y-%m-%d-%H-%M-%S") + ".log"
   logging.basicConfig(filename=log_filename,level=logging.INFO, format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')

   logging.info("Starting Mail service")
   config = configparser.ConfigParser()
   config.read('config.ini')
   host = config['DEFAULT']['host']
   port = int(config['DEFAULT']['port'])

   handler = SendMailHandler(config)
   processor = Processor(handler)
   transport = TSocket.TServerSocket(host, port)
   tfactory = TTransport.TBufferedTransportFactory()
   pfactory = TBinaryProtocol.TBinaryProtocolFactory()

   server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

   logging.info('Waiting for request ...')
   server.serve()
   logging.info('done.')

