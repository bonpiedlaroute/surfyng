#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 
# author(s): Noel Tchidjo
# All rights reserved



import unittest
from hash_id import *

class TestHashId(unittest.TestCase):
   

   def test_should_provide_the_same_hash_after_3_run(self):
      self.assertEqual(7556105, hash_id("http://www.seloger.com/annonces/achat/appartement/houilles-78/126789777.htm?ci=780311&idtt=2&idtypebien=1,2&tri=initial"))
      self.assertEqual(7556105, hash_id("http://www.seloger.com/annonces/achat/appartement/houilles-78/126789777.htm?ci=780311&idtt=2&idtypebien=1,2&tri=initial"))
      self.assertEqual(7556105, hash_id("http://www.seloger.com/annonces/achat/appartement/houilles-78/126789777.htm?ci=780311&idtt=2&idtypebien=1,2&tri=initial"))

   def test_should_provide_different_hash_for_different_inputs(self):
      self.assertNotEqual(hash_id("http://www.seloger.com/annonces/achat/maison/houilles-78/blanches-pierrats-tonkin/124806085.htm?ci=780311&idtt=2&idtypebien=1,2&tri=initial"), hash_id("http://www.seloger.com/annonces/achat/appartement/houilles-78/126789777.htm?ci=780311&idtt=2&idtypebien=1,2&tri=initial"))

      self.assertNotEqual(hash_id("http://www.seloger.com/annonces/achat/maison/houilles-78/blanches-pierrats-tonkin/124806085.htm?ci=780311&idtt=2&idtypebien=1,2&tri=initial"), hash_id("http://www.seloger.com/annonces/achat/maison/houilles-78/126438695.htm?ci=780311&idtt=2&idtypebien=1,2&naturebien=1,2,4&tri=initial"))

if __name__ == '__main__':
   suite = unittest.TestLoader().loadTestsFromTestCase(TestHashId)
   unittest.TextTestRunner(verbosity=2).run(suite)

