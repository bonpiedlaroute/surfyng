#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2016 Noel Tchidjo
# All rights reserved

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
import time


""" generic automatic website navigator class 
"""
class auto_navigator:

      def __init__(self):
         self.browser = webdriver.Firefox()
         self.announce_links = []

      def navigate_to_homepage(self, website_url):
         self.browser.get(website_url)
         time.sleep(1)


