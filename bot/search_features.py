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


LeboncoinPropertiesMapping = dict()
LeboncoinPropertiesMapping[APART_ID] = '2'
LeboncoinPropertiesMapping[HOUSE_ID] = '1'



def getSeLogerPropertiesId(surfyng_properties_id):
   
   return SeLogerPropertiesMapping[surfyng_properties_id]


def getLogicImmoPropertiesId(surfyng_properties_id):
   return logicimmo_mappingids[surfyng_properties_id]


def getLeboncoinPropertiesId(surfyn_properties_id):
   return LeboncoinPropertiesMapping[surfyn_properties_id]

BUY_ID = 1
RENT_ID = 2
SeLogerSearchTypeMapping = dict()
SeLogerSearchTypeMapping[BUY_ID] = '2'
SeLogerSearchTypeMapping[RENT_ID] = '1'


LeboncoinSearchTypeMapping = dict()
LeboncoinSearchTypeMapping[BUY_ID] = '9'
LeboncoinSearchTypeMapping[RENT_ID] = '10'



def getSeLogerSearchTypeId(surfyn_searchtype):
   return SeLogerSearchTypeMapping[surfyn_searchtype]


def getLeboncoinSearchTypeId(surfyn_searchtype):
   return LeboncoinSearchTypeMapping[surfyn_searchtype]

BienIciPropertiesMapping = dict()
BienIciPropertiesMapping[APART_ID] = 'flat'
BienIciPropertiesMapping[HOUSE_ID] = 'house'

BienIciSearchTypeMapping = dict()
BienIciSearchTypeMapping[BUY_ID] = 'buy'
BienIciSearchTypeMapping[RENT_ID] = 'rent'



def getBienIciPropertiesId(surfyn_properties_id):
   return BienIciPropertiesMapping[surfyn_properties_id]

def getBienIciSearchTypeId(surfyn_searchtype):
   return BienIciSearchTypeMapping[surfyn_searchtype]
