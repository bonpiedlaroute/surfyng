#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2016 Noel Tchidjo
# All rights reserved

import urllib
import sqlite3
import time
from auto_navigator import auto_navigator


""" leboncoin automatic navigator class
"""
class lbc_auto_navigator(auto_navigator):

   def __init__(self):
      auto_navigator.__init__(self)

   def check_homepage(self):
      assert "annonces" in self.browser.title

   def navigate_to_homepage(self):
      auto_navigator.navigate_to_homepage(self, "https://www.leboncoin.fr/")

   def navigate_to_region(self,region):
      self.browser.find_element_by_xpath("//a[@id='"+region+"']").click()
      time.sleep(1.5) # wait a little bit, loading time of the page
   
   def set_search_text(self, text_to_search):
      search_text_elt = self.browser.find_element_by_name("q")
      search_text_elt.clear()
      search_text_elt.send_keys(text_to_search)
      

   def select_real_estate_category(self):
      self.browser.find_element_by_xpath("//span[@id='searchboxToggleCategory']").click()
      time.sleep(1)
      self.browser.find_element_by_xpath("//a[@data-category='9']").click()
      time.sleep(0.5) # wait for price min, price max menu to appear
   
   def setprice(self, price_elt, price):
      all_options = price_elt.find_elements_by_tag_name("option")
      for option in all_options:
         if option.get_attribute("text") == price:
            option.click()

   def set_pricemin(self, price):
      price_min_elt = self.browser.find_element_by_xpath("//select[@name='ps']")
      self.setprice(price_min_elt, price)

   def set_pricemax(self, price):
      price_max_elt = self.browser.find_element_by_xpath("//select[@name='pe']")
      self.setprice(price_max_elt, price)


   def set_location(self, location):
      location_elt = self.browser.find_element_by_name("location_p")
      location_elt.clear()
      location_elt.send_keys(location)
      time.sleep(1)

   def click_on_checkbox(self, checkbox_id):
      property_type_checkbox_elt = self.browser.find_element_by_xpath("//input[@id='"+checkbox_id+"']")
      property_type_checkbox_elt.click()
      time.sleep(0.5)

   def search(self):
      search_button = self.browser.find_element_by_xpath("//input[@id='searchbutton']")
      search_button.click()
      #wait for search result
      time.sleep(1)

   def retrieve_searchresults_links(self):
      announce_list = self.browser.find_element_by_xpath("//section[@class='tabsContent block-white dontSwitch']")

      items = announce_list.find_elements_by_tag_name("a")

      for item in items:
         self.announce_links.append(item.get_attribute('href'))

   def save_results_in_database(self):
      img_counter = 0
      # connect to the database
      cnx = sqlite3.connect('/var/www/html/realestate_ad.db')
      cursor = cnx.cursor()

      for link in self.announce_links:
         self.browser.get(link)
         time.sleep(1)
         # get announce url
         url = self.browser.current_url
         # get announce property type
         property_type = self.browser.find_element_by_xpath("//span[contains(text(),'Type de bien')]/following-sibling::span").text
         # get annouce price
         price = self.browser.find_element_by_xpath("//span[contains(text(),'Prix')]/following-sibling::span").text
         # get announce rooms
         nb_rooms = self.browser.find_element_by_xpath("//span[contains(text(),'Pièces')]/following-sibling::span").text
         # get announce description
         description = self.browser.find_element_by_xpath("//p[contains(text(),'Description :')]/following-sibling::p").text
         # get announce image source
         img = self.browser.find_element_by_xpath("//div[@data-popin-type='image']/img")
         src = img.get_attribute('src')
         img_filename = "images/img"
         img_filename += str(img_counter)
         img_counter += 1
         # download the image
         urllib.urlretrieve(src,img_filename)
         
         values = (url, property_type, price, nb_rooms, description, img_filename)
         cursor.execute('INSERT OR IGNORE INTO leboncoin_ad VALUES (?,?,?,?,?,?)', values)
         
         self.browser.back()
         time.sleep(2)

      # save results
      cnx.commit()

      

if __name__ == "__main__":
   
   lbc_navigator = lbc_auto_navigator()
   # go to leboncoin home page
   lbc_navigator.navigate_to_homepage()

   lbc_navigator.check_homepage()
   
   # go to the region
   lbc_navigator.navigate_to_region("region_11")
   
   # set the search text
   lbc_navigator.set_search_text("maison")

   # select the right category
   lbc_navigator.select_real_estate_category()

   # set price min
   lbc_navigator.set_pricemin("450 000")

   # set price max
   lbc_navigator.set_pricemax("500 000")

   # set the location
   lbc_navigator.set_location("houilles")

   # click on checkbox
   lbc_navigator.click_on_checkbox("ret_1")

   # click on search button
   lbc_navigator.search()

   # retrieve search results links
   lbc_navigator.retrieve_searchresults_links()
   
   # save results in database
   lbc_navigator.save_results_in_database()


