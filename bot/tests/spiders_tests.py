#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 
# author(s): Noel Tchidjo
# All rights reserved



import unittest
from url_builder import *
from search_features import *

class TestSpiders(unittest.TestCase):
   

   def test_should_insert_at_correct_position(self):
      url = 'http://www.amazon.fr/lvres'
      url_modified = insert_in_url(url,22, 'i')

      self.assertEqual('http://www.amazon.fr/livres',url_modified)
         
      url = 'http://www.amazon.fr/livres'
      url_secured = insert_in_url(url, 4, 's')
      self.assertEqual('https://www.amazon.fr/livres', url_secured)

   def test_should_build_correct_url(self):
      url = 'http://www.seloger.com/list.htm?idtt=2&naturebien=1,2,4&idtypebien=&ci=780311'
      
      url_seloger = buildselogerurl('2', '2')
      self.assertEqual('https://www.seloger.com/list.htm?idtt=2&naturebien=1,2,4&idtypebien=2&ci=780311', url_seloger)

      url_seloger = buildselogerurl('1', '2')
      self.assertEqual('https://www.seloger.com/list.htm?idtt=2&naturebien=1,2,4&idtypebien=1&ci=780311', url_seloger)


   def test_should_return_the_correct_properties_id(self):
      
      self.assertEquals('1', getSeLogerPropertiesId(APART_ID))

      self.assertEquals('2', getSeLogerPropertiesId(HOUSE_ID))

   def test_should_return_the_correct_search_id(self):
      
      self.assertEquals('2', getSeLogerSearchTypeId(BUY_ID))

      self.assertEquals('1', getSeLogerSearchTypeId(RENT_ID))


   def test_should_return_correct_description_url(self):
      url = 'http://www.seloger.com/annonces/achat/maison/colombes-92/109240349.htm?ci=920025&idtt=2&idtypebien=2&listing-listpg=11&naturebien=1,2,4'

      self.assertEquals('https://www.seloger.com/detail,json,caracteristique_bien.json?idannonce=109240349',buildselogerdescriptionurl(url))


   def test_should_return_correct_logicimmo_url(self):
      url = 'http://www.logic-immo.com/vente-immobilier-houilles-78800,13772_2/options/groupprptypesids=1'
      self.assertEquals(url, buildLogicImmoUrl(1, BUY_ID))

      url = url[:-1]

      url = url + '2'

      self.assertEquals(url, buildLogicImmoUrl(2, BUY_ID))
   
   def test_should_return_correct_leboncoin_url(self):
      url = 'https://www.leboncoin.fr/recherche/?category=10&locations=Houilles&real_estate_type=2'

      self.assertEquals(url, buildleboncoinurl('2', '10','Houilles'))

      url = 'https://www.leboncoin.fr/recherche/?category=9&locations=Houilles&real_estate_type=1'
 
      self.assertEquals(url, buildleboncoinurl('1', '9', 'Houilles'))
 
if __name__ == '__main__':
   suite = unittest.TestLoader().loadTestsFromTestCase(TestSpiders)
   unittest.TextTestRunner(verbosity=2).run(suite)

