#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

APART_ID = 1
HOUSE_ID = 2

SeLogerPropertiesMapping = dict()

SeLogerPropertiesMapping[HOUSE_ID] = '2'
SeLogerPropertiesMapping[APART_ID] = '1'

logicimmo_mappingids = dict()

logicimmo_mappingids[APART_ID] = '1'
logicimmo_mappingids[HOUSE_ID] = '2'



def getSeLogerPropertiesId(surfyng_properties_id):
   
   return SeLogerPropertiesMapping[surfyng_properties_id]


def getLogicImmoPropertiesId(surfyng_properties_id):
   return logicimmo_mappingids[surfyng_properties_id]


BUY_ID = 1
RENT_ID = 2
SeLogerSearchTypeMapping = dict()

SeLogerSearchTypeMapping[BUY_ID] = '2'
SeLogerSearchTypeMapping[RENT_ID] = '1'

def getSeLogerSearchTypeId(surfyn_searchtype):
   return SeLogerSearchTypeMapping[surfyn_searchtype]
 
