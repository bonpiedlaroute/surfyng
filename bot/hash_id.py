#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2017 
# author(s): Noel Tchidjo
# All rights reserved

# djb2 hash function

def hash_id(url):
   hashval = 5381

   for c in url:
      hashval = ord(c) + ((hashval << 5) + hashval) # ascii(c) + hashval * 33

   return hashval % 20000107
