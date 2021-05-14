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
from email.mime.text import MIMEText
from email.header import Header
from email.utils import formataddr






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
      logging.info("sendemailtosurfyn log in to smtp server")
      smtp_server.login(self.from_addr, self.password)

      msg_to_send = 'De la part de :' + sender_email + '\n\n '+ msg

      msg1 = MIMEText(msg_to_send, _charset='utf-8')
      msg1['Subject'] = "Contact depuis surfyn.fr: " + subject
      msg1['From'] = formataddr((str(Header('Surfyn', 'utf-8')), self.from_addr))
      msg1['To'] = self.to_addrs 
      logging.info("sending msg :\n\n"+msg_to_send)
      recipients = self.to_addrs.split(",")
      smtp_server.sendmail(self.from_addr, recipients, msg1.as_string())

      new_recipients = []
      new_recipients.append(sender_email)
      new_subject = "Merci d'avoir contacté Surfyn"
      new_msg = "Bonjour\n nous avons bien reçu votre message, nous allons revenir vers vous dans les meilleurs délais\n\n  A bientôt\nL'équipe Surfyn"
      msg2 = MIMEText(new_msg, _charset='utf-8')
      msg2['Subject'] = new_subject
      msg2['From'] = formataddr((str(Header('Surfyn', 'utf-8')), self.from_addr))
      msg2['To'] = sender_email 

      smtp_server.sendmail(self.from_addr, new_recipients, msg2.as_string())



      smtp_server.quit()

      logging.info("sendemailtosurfyn finished!")

      retval = SendEmailResult()
      retval.success = True
      retval.error = ""

      return retval


   def sendaccountcreationemail(self, display_name, new_account_email):
      logging.info(u'start sending email to user {}'.format(display_name))
      smtp_host_port = self.smtp_host + ':' + self.smtp_port
      smtp_server = smtplib.SMTP(smtp_host_port) 
      logging.info("sendaccountcreationemail log in to smtp server")
      smtp_server.login(self.from_addr, self.password)

      msg_to_send = u'Bonjour '
      msg_to_send += display_name
      msg_to_send += u',\n\n'
      msg_to_send += u'Votre compte Surfyn a bien été créé.\n'
      msg_to_send += u'Connectez-vous dès à  présent sur https://surfyn.fr et créez des alertes email.\n'
      msg_to_send += u'Ainsi, vous serez notifié par email dès qu\'une annonce immobilière, correspondant à votre recherche, est mise en ligne sur le web.\n\n'
      msg_to_send += u'A votre service  \n'
      msg_to_send += u'L\' équipe Surfyn\n'

      msg = MIMEText(msg_to_send, _charset='utf-8')
      msg['Subject'] = u'Merci - Votre compte Surfyn a bien été créé'
      msg['From'] = formataddr((str(Header('Surfyn', 'utf-8')), self.from_addr))
      msg['To'] = new_account_email
      logging.info("sending msg :\n\n"+msg_to_send)
      recipients = []
      recipients.append(new_account_email)
      smtp_server.sendmail(self.from_addr, recipients, msg.as_string())

      smtp_server.quit()

      logging.info("sendaccountcreationemail finished!")

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

