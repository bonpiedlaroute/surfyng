/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "DBaccess.h"


namespace surfyn
{
namespace rest_server
{
   void DBaccess::fetchData(utility::stringstream_t& sstream)
   {
      sstream << U("[\n");
      sstream << U("{\n");
      sstream << U("\"ID\":\"1\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Appartement\",\n");
      sstream << U("\"SURFACE\": \"46 m2\",\n");
      sstream << U("\"NB_ROOMS\": \"2 pièces\",\n");
      sstream << U("\"CITY\": \"Houilles\",\n");
      sstream << U("\"HISTORY\": \"il y'a 3h\", \n");

      sstream << U("\"PRICE\": \"320 000  €\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_1.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"seloger\",\n\
         \"logicimmo\",\n\
         \"leboncoin\"\n\
      ]");
      sstream << U("},\n");

      sstream << U("{\n");
      sstream << U("\"ID\":\"2\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Appartement\",\n");
      sstream << U("\"SURFACE\": \"53 m2\",\n");
      sstream << U("\"NB_ROOMS\": \"3 pièces\",\n");
      sstream << U("\"CITY\": \"Houilles\",\n");
      sstream << U("\"HISTORY\": \"il y'a 2h\", \n");

      sstream << U("\"PRICE\": \"339 000  €\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_2.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"seloger\",\n\
         \"seloger\"]\n");
      sstream << U("},\n");

      sstream << U("{\n");
      sstream << U("\"ID\":\"3\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Maison\",\n");
      sstream << U("\"SURFACE\": \"228 m2\",\n");
      sstream << U("\"NB_ROOMS\": \"8 pièces\",\n");
      sstream << U("\"CITY\": \"Houilles\",\n");
      sstream << U("\"HISTORY\": \"il y'a 1h\", \n");

      sstream << U("\"PRICE\": \"800 000  €\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_3.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"leboncoin\"\n\
      ]");
      sstream << U("},\n");

      sstream << U("{\n");
      sstream << U("\"ID\":\"4\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Appartement\",\n");
      sstream << U("\"SURFACE\": \"63 m2\",\n");
      sstream << U("\"NB_ROOMS\": \"3 pièces\",\n");
      sstream << U("\"CITY\": \"Houilles\",\n");
      sstream << U("\"HISTORY\": \"il y'a 2h\", \n");

      sstream << U("\"PRICE\": \"215 000  €\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_5.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"seloger\",\n\
         \"logicimmo\",\n\
         \"leboncoin\"\n\
      ]");
      sstream << U("}\n");

      sstream << U("\n]\n");

   }
}
}
