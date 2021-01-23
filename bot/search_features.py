#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 Noel Tchidjo
# All rights reserved

APART_ID = 1
HOUSE_ID = 2
BUY_ID = 1
RENT_ID = 2


SeLogerPropertiesMapping = dict()

SeLogerPropertiesMapping[HOUSE_ID] = '2'
SeLogerPropertiesMapping[APART_ID] = '1'

logicimmo_propertiesMapping = dict()

logicimmo_propertiesMapping[APART_ID] = 'appartement'
logicimmo_propertiesMapping[HOUSE_ID] = 'maison'

logicimmo_searchTypeMapping = dict()

logicimmo_searchTypeMapping[BUY_ID] = 'vente'
logicimmo_searchTypeMapping[RENT_ID] = 'location'


LeboncoinPropertiesMapping = dict()
LeboncoinPropertiesMapping[APART_ID] = '2'
LeboncoinPropertiesMapping[HOUSE_ID] = '1'



def getSeLogerPropertiesId(surfyng_properties_id):
   
   return SeLogerPropertiesMapping[surfyng_properties_id]


def getLogicImmoPropertiesId(surfyng_properties_id):
   return logicimmo_propertiesMapping[surfyng_properties_id]

def getLogicImmoSearchTypeId(surfyng_properties_id):
   return logicimmo_searchTypeMapping[surfyng_properties_id]


def getLeboncoinPropertiesId(surfyn_properties_id):
   return LeboncoinPropertiesMapping[surfyn_properties_id]

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

LaforetPropertiesMapping = dict()
LaforetPropertiesMapping[APART_ID] = 'apartment'
LaforetPropertiesMapping[HOUSE_ID] = 'house'

LaforetSearchTypeMapping = dict()
LaforetSearchTypeMapping[BUY_ID] = 'buy'
LaforetSearchTypeMapping[RENT_ID] = 'rent'

def getLaforetPropertiesId(_id):
   return LaforetPropertiesMapping[_id]

def getLaforetSearchTypeId(_id):
   return LaforetSearchTypeMapping[_id]

OrpiPropertiesMapping = dict()
OrpiPropertiesMapping[APART_ID] = 'appartement'
OrpiPropertiesMapping[HOUSE_ID] = 'maison'

OrpiSearchTypeMapping = dict()
OrpiSearchTypeMapping[BUY_ID] = 'buy'
OrpiSearchTypeMapping[RENT_ID] = 'rent'

def getOrpiPropertiesId(_id):
   return OrpiPropertiesMapping[_id]

def getOrpiSearchTypeId(_id):
   return OrpiSearchTypeMapping[_id]


StephanePlazaImoPropertiesMapping = dict()
StephanePlazaImoPropertiesMapping[APART_ID] = '1'
StephanePlazaImoPropertiesMapping[HOUSE_ID] = '2'

StephanePlazaImoSearchTypeMapping = dict()
StephanePlazaImoSearchTypeMapping[BUY_ID] = 'buy'
StephanePlazaImoSearchTypeMapping[RENT_ID] = 'rent'

def getStephanePlazaImoPropertiesId(_id):
   return StephanePlazaImoPropertiesMapping[_id]

def getStephanePlazaImoSearchTypeId(_id):
   return StephanePlazaImoSearchTypeMapping[_id]

FonciaPropertiesMapping = dict()
FonciaPropertiesMapping[APART_ID] = 'appartement'
FonciaPropertiesMapping[HOUSE_ID] = 'maison'

FonciaSearchTypeMapping = dict()
FonciaSearchTypeMapping[BUY_ID] = 'achat'
FonciaSearchTypeMapping[RENT_ID] = 'location'

def getFonciaPropertiesId(_id):
   return FonciaPropertiesMapping[_id]

def getFonciaSearchTypeId(_id):
   return FonciaSearchTypeMapping[_id]

Century21PropertiesMapping = dict()
Century21PropertiesMapping[APART_ID] = 'appartement'
Century21PropertiesMapping[HOUSE_ID] = 'maison'

Century21SearchTypeMapping = dict()
Century21SearchTypeMapping[BUY_ID] = 'achat'
Century21SearchTypeMapping[RENT_ID] = 'location'

def getCentury21PropertiesId(_id):
   return Century21PropertiesMapping[_id]

def getCentury21SearchTypeId(_id):
   return Century21SearchTypeMapping[_id]

GuyHoquetPropertiesMapping = dict()
GuyHoquetPropertiesMapping[APART_ID] = '1'
GuyHoquetPropertiesMapping[HOUSE_ID] = '2'

GuyHoquetSearchTypeMapping = dict()
GuyHoquetSearchTypeMapping[BUY_ID] = 'Vente'
GuyHoquetSearchTypeMapping[RENT_ID] = 'Location'

def getGuyHoquetPropertiesId(_id):
   return GuyHoquetPropertiesMapping[_id]

def getGuyHoquetSearchTypeId(_id):
   return GuyHoquetSearchTypeMapping[_id]


EraImmoPropertiesMapping = dict()
EraImmoPropertiesMapping[APART_ID] = '1'
EraImmoPropertiesMapping[HOUSE_ID] = '2'

EraImmoSearchTypeMapping = dict()
EraImmoSearchTypeMapping[BUY_ID] = 'Vente'
EraImmoSearchTypeMapping[RENT_ID] = 'Location'

def getEraImmoPropertiesId(_id):
   return EraImmoPropertiesMapping[_id]

def getEraImmoSearchTypeId(_id):
   return EraImmoSearchTypeMapping[_id]



ArthurImmoPropertiesMapping = dict()
ArthurImmoPropertiesMapping[APART_ID] = '1'
ArthurImmoPropertiesMapping[HOUSE_ID] = '2'

ArthurImmoSearchTypeMapping = dict()
ArthurImmoSearchTypeMapping[BUY_ID] = '2'
ArthurImmoSearchTypeMapping[RENT_ID] = '1'

def getArthurImmoPropertiesId(_id):
   return ArthurImmoPropertiesMapping[_id]

def getArthurImmoSearchTypeId(_id):
   return ArthurImmoSearchTypeMapping[_id]


PapPropertiesMapping = dict()
PapPropertiesMapping[APART_ID] = 'appartements'
PapPropertiesMapping[HOUSE_ID] = 'maisons'

PapSearchTypeMapping = dict()
PapSearchTypeMapping[BUY_ID] = 'vente'
PapSearchTypeMapping[RENT_ID] = 'locations'

def getPapPropertiesId(_id):
   return PapPropertiesMapping[_id]

def getPapSearchTypeId(_id):
   return PapSearchTypeMapping[_id]

IadfrancePropertiesMapping = dict()
IadfrancePropertiesMapping[APART_ID] = 'AP'
IadfrancePropertiesMapping[HOUSE_ID] = 'MV'

IadfranceSearchTypeMapping = dict()
IadfranceSearchTypeMapping[BUY_ID] = 'Vente'
IadfranceSearchTypeMapping[RENT_ID] = 'Location'

def getIadfrancePropertiesId(_id):
   return IadfrancePropertiesMapping[_id]

def getIadfranceSearchTypeId(_id):
   return IadfranceSearchTypeMapping[_id]


ParuVenduPropertiesMapping = dict()
ParuVenduPropertiesMapping[APART_ID] = 'tbApp=1'
ParuVenduPropertiesMapping[HOUSE_ID] = 'tbMai=1'

ParuVenduSearchTypeMapping = dict()
ParuVenduSearchTypeMapping[BUY_ID] = '1'
ParuVenduSearchTypeMapping[RENT_ID] = '5'

def getParuVenduPropertiesId(_id):
   return ParuVenduPropertiesMapping[_id]

def getParuVenduSearchTypeId(_id):
   return ParuVenduSearchTypeMapping[_id]

AvendreAlouerPropertiesMapping = dict()
AvendreAlouerPropertiesMapping[APART_ID] = 'b-appartement'
AvendreAlouerPropertiesMapping[HOUSE_ID] = 'b-maison'

AvendreAlouerSearchTypeMapping = dict()
AvendreAlouerSearchTypeMapping[BUY_ID] = 'vente'
AvendreAlouerSearchTypeMapping[RENT_ID] = 'location'

def getAvendreAlouerPropertiesId(_id):
   return AvendreAlouerPropertiesMapping[_id]

def getAvendreAlouerSearchTypeId(_id):
   return AvendreAlouerSearchTypeMapping[_id]

NestennPropertiesMapping = dict()
NestennPropertiesMapping[APART_ID] = 'Appartement'
NestennPropertiesMapping[HOUSE_ID] = 'Maison'

NestennSearchTypeMapping = dict()
NestennSearchTypeMapping[BUY_ID] = 'Vente'
NestennSearchTypeMapping[RENT_ID] = 'Location'

def getNestennPropertiesId(_id):
   return NestennPropertiesMapping[_id]

def getNestennSearchTypeId(_id):
   return NestennSearchTypeMapping[_id]

NotairesPropertiesMapping = dict()
NotairesPropertiesMapping[APART_ID] = 'APA'
NotairesPropertiesMapping[HOUSE_ID] = 'MAA'

def getNotairesPropertiesId(_id):
   return NotairesPropertiesMapping[_id]

NotairesRefererPropertiesMapping = dict()
NotairesRefererPropertiesMapping[APART_ID] = 'APP'
NotairesRefererPropertiesMapping[HOUSE_ID] = 'MAI'

def getNotairesRefererPropertiesId(_id):
   return NotairesRefererPropertiesMapping[_id]

AgencePrincipalePropertiesMapping = dict()
AgencePrincipalePropertiesMapping[APART_ID] = '1'
AgencePrincipalePropertiesMapping[HOUSE_ID] = '2'

AgencePrincipaleSearchTypeMapping = dict()
AgencePrincipaleSearchTypeMapping[BUY_ID] = 'Vente'
AgencePrincipaleSearchTypeMapping[RENT_ID] = 'Location'

def getAgencePrincipalePropertiesId(_id):
   return AgencePrincipalePropertiesMapping[_id]

def getAgencePrincipaleSearchTypeId(_id):
   return AgencePrincipaleSearchTypeMapping[_id]

LefigaroImmobilierPropertiesMapping = dict()
LefigaroImmobilierPropertiesMapping[APART_ID] = 'appartement'                                                     
LefigaroImmobilierPropertiesMapping[HOUSE_ID] = 'maison'                                                     

LefigaroImmobilierSearchTypeMapping = dict()
LefigaroImmobilierSearchTypeMapping[BUY_ID] = 'vente'
LefigaroImmobilierSearchTypeMapping[RENT_ID] = 'location'                                               

def getLefigaroImmobilierPropertiesId(_id):
   return LefigaroImmobilierPropertiesMapping[_id]                                                      

def getLefigaroImmobilierSearchTypeId(_id):
   return LefigaroImmobilierSearchTypeMapping[_id]      

EtreProprioSearchTypeMapping = dict()
EtreProprioSearchTypeMapping[APART_ID] = 'f'
EtreProprioSearchTypeMapping[HOUSE_ID] = 'h'

def getEtreProprioPropertiesId(_id):
   return EtreProprioSearchTypeMapping[_id]

EfficityPropertiesMapping = dict()
EfficityPropertiesMapping[APART_ID] = '1'
EfficityPropertiesMapping[HOUSE_ID] = '2'

def getEfficityPropertiesId(_id):
   return EfficityPropertiesMapping[_id]