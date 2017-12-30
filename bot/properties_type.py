#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

APART_ID = 1
HOUSE_ID = 2

SeLogerPropertiesMapping = dict()

SeLogerPropertiesMapping[HOUSE_ID] = '2'
SeLogerPropertiesMapping[APART_ID] = '1'

def getSeLogerPropertiesId(surfyng_properties_id):
   
   return SeLogerPropertiesMapping[surfyng_properties_id]

