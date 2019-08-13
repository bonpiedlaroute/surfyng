/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "DBaccess.h"


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "sf_services/sf_utils/inc/Str.h"
#include "sf_services/sf_utils/inc/Logger.h"
#include <boost/algorithm/string.hpp>


using Log = surfyn::utils::Logger;

const int port = 5050;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

namespace surfyn
{
namespace rest_server
{
const std::string id_field = "ID";
const std::string id_city = "CITY";
const std::string id_property_type = "PROPERTY_TYPE";
const std::string id_surface = "SURFACE";
const std::string id_nbrooms = "ROOMS";
const std::string id_history = "HISTORY";
const std::string id_price = "PRICE";
const std::string id_image = "IMAGE";
const std::string id_source = "SOURCES";
const std::string id_announcelink = "ANNOUNCE_LINK";
const std::string id_imagecount = "IMAGE_COUNT";
const std::string id_transport = "TRANSPORT";
const std::string id_location = "LOCATION";
const std::string id_sourcelogo = "SOURCE_LOGO";
const std::string id_floor = "FLOOR";
const std::string id_cellar = "CELLAR";
const std::string id_duplicates = "DUPLICATES";

const std::string details_table = "FR_SUMMARY";
const std::string exprval_city = ":ct";
const std::string exprval_searchType = ":st";
const std::string exprval_priceMin = ":pi";
const std::string exprval_priceMax = ":pa";
const std::string exprval_propType = ":py";
const std::string exprval_areaMin = ":ai";
const std::string exprval_areaMax = ":aa";

std::string searchTypeValue = "";

   DBaccess::DBaccess()
   {
      shared_ptr<TTransport> socket(new TSocket("localhost", port));
      shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      m_client = std::make_shared<dynamodb_accessClient>(protocol);

      transport->open();
   }
   void DBaccess::fillFilterExprAndExprValue(std::stringstream &filterexpression, std::map<std::string, ValueType> &expressionValue,
                                   const std::string &param, const std::string& paramvalue, const std::string & value,
                                   const std::string &Operator)
   {
      filterexpression << param;
      filterexpression << " " << Operator << " ";
      filterexpression << paramvalue;
      ValueType exprValue;
      exprValue.field = value.c_str();
      exprValue.fieldtype = (param == "PRICE" || param == "SURFACE") ? Type::type::NUMBER : Type::type::STRING;
      expressionValue[paramvalue] = exprValue;
   }
   bool DBaccess::isAlreadyProvided(const std::map<std::string, std::string>& table_entry, const std::set<int64_t>& adprovided)
   {
      const auto iter = table_entry.find("ID");
      if(iter!= table_entry.end())
      {
         return adprovided.find(atol((iter->second).c_str())) != adprovided.end();
      }
      return false;
   }
   void DBaccess::updateProvidedAd(const std::map<std::string, std::string>& table_entry,  std::set<int64_t>& adprovided)
   {
      /* insert the ID of the ad */
      const auto iter = table_entry.find("ID");
      if(iter!= table_entry.end())
      {
         adprovided.insert(atol((iter->second).c_str()));
      }

      /* insert the duplicate id */
      const auto iter_dup = table_entry.find(id_duplicates);
      if(iter_dup!= table_entry.end())
      {
         std::vector<std::string> duplicates;
         surfyn::utils::split(iter_dup->second, ",", duplicates);

         for(auto item : duplicates)
         {
            adprovided.insert(atol(item.c_str()));
         }
      }


   }
   void DBaccess::fetchSummary(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query )
   {
      // for quick testing ...
      /*sstream << U("[\n");
      sstream << U("{\n");
      sstream << U("\"ID\":\"1\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Appartement\",\n");
      sstream << U("\"SURFACE\": \"46\",\n");
      sstream << U("\"ROOMS\": \"2\",\n");
      sstream << U("\"CITY\": \"Colombes\",\n");
      sstream << U("\"HISTORY\": \"il y'a 3h\", \n");

      sstream << U("\"PRICE\": \"320000\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_1.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"seloger\",\n\
         \"bienici\",\n\
         \"leboncoin\"\n\
      ]");
      sstream << U("},\n");

      sstream << U("{\n");
      sstream << U("\"ID\":\"2\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Appartement\",\n");
      sstream << U("\"SURFACE\": \"53\",\n");
      sstream << U("\"ROOMS\": \"3\",\n");
      sstream << U("\"CITY\": \"Colombes\",\n");
      sstream << U("\"HISTORY\": \"il y'a 2h\", \n");

      sstream << U("\"PRICE\": \"339000\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_2.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"seloger\",\n\
         \"seloger\"]\n");
      sstream << U("},\n");

      sstream << U("{\n");
      sstream << U("\"ID\":\"3\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Maison\",\n");
      sstream << U("\"SURFACE\": \"228\",\n");
      sstream << U("\"ROOMS\": \"8\",\n");
      sstream << U("\"CITY\": \"Colombes\",\n");
      sstream << U("\"HISTORY\": \"il y'a 1h\", \n");

      sstream << U("\"PRICE\": \"800000\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_3.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"leboncoin\"\n\
      ]");
      sstream << U("},\n");

      sstream << U("{\n");
      sstream << U("\"ID\":\"4\",\n");
      sstream << U("\"PROPERTY_TYPE\": \"Appartement\",\n");
      sstream << U("\"SURFACE\": \"63\",\n");
      sstream << U("\"ROOMS\": \"3\",\n");
      sstream << U("\"CITY\": \"Colombes\",\n");
      sstream << U("\"HISTORY\": \"il y'a 2h\", \n");

      sstream << U("\"PRICE\": \"215000\", \n");
      sstream << U("\"IMAGE\": \"data/annonce_5.jpg\",\n");
      sstream << U("\"SOURCES\": [\n\
         \"seloger\",\n\
         \"bienici\",\n\
         \"leboncoin\"\n\
      ]");
      sstream << U("}\n");

      sstream << U("\n]\n");

      return;*/

      std::map<std::string, ValueType> attributestoget;
      std::set<int64_t> adProvided;

      ValueType value;
      value.field = "";
      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_field] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_city] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_property_type] = value;

      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_surface] = value;

      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_nbrooms] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_history] = value;

      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_price] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_image] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_source] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_duplicates] = value;

      bool scanend = false;

      sstream << U("[\n");
      // search anounces on summary table
      // and construct a json string
      bool firsttime = true;
      do
      {
         ScanReqResult scanReturn;
         ///TODO enter the filter in the last parameter depending on the user query
         std::stringstream filterExpression;
         std::map<std::string, ValueType> expressionValue;
         auto iter = query.find("search_city");
         if( iter != query.end() )
         {
            std::string city = iter->second;
            boost::to_lower(city);

            fillFilterExprAndExprValue(filterExpression, expressionValue, "CITY", exprval_city, city, "=");
         }

         iter = query.find("search_type");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            searchTypeValue = strcmp((iter->second).c_str(), "1") == 0 ? "For sale":"For rent";
            fillFilterExprAndExprValue(filterExpression, expressionValue, "SEARCH_TYPE", exprval_searchType, searchTypeValue , "=");
         }

         iter = query.find("price_min");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            std::string formatPrice;
            if( searchTypeValue == "For sale")
            {
               int64_t price = atoi((iter->second).c_str());
               price *= 1000;
               formatPrice = std::to_string(price);
            }
            else
            {
               formatPrice = iter->second;
            }

            fillFilterExprAndExprValue(filterExpression, expressionValue, "PRICE", exprval_priceMin, formatPrice, ">=");
         }

         iter = query.find("price_max");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            std::string formatPrice;
            if( searchTypeValue == "For sale")
            {
               int64_t price = atoi((iter->second).c_str());
               price *= 1000;
               formatPrice = std::to_string(price);
            }
            else
            {
               formatPrice = iter->second;
            }

            fillFilterExprAndExprValue(filterExpression, expressionValue, "PRICE", exprval_priceMax, formatPrice, "<=");
         }

         iter = query.find("prop_type");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            filterExpression << "PROPERTY_TYPE";
            filterExpression << " in (";
            std::vector<std::string> output;
            surfyn::utils::split((iter->second).c_str(), ",", output);
            for(auto iter_value = output.begin();iter_value != output.end(); ++iter_value)
            {
               if( iter_value != output.begin() )
               filterExpression << ",";
               std::string propertyType = *iter_value == "1" ? "Appartement" : "Maison";

               std::string expr = exprval_propType + *iter_value;
               filterExpression << expr;

               ValueType exprValue;
               exprValue.field = propertyType.c_str();
               exprValue.fieldtype = Type::type::STRING;
               expressionValue[expr] = exprValue;
            }
            filterExpression << ")";
         }
         iter = query.find("area_min");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            fillFilterExprAndExprValue(filterExpression, expressionValue, "SURFACE", exprval_areaMin, (iter->second).c_str(), ">=");
         }

         iter = query.find("area_max");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            fillFilterExprAndExprValue(filterExpression, expressionValue, "SURFACE", exprval_areaMax, (iter->second).c_str(), "<=");
         }


         m_client->scan(scanReturn, "FR_SUMMARY", attributestoget, filterExpression.str(), expressionValue);


         std::stringstream logstream;
         logstream << "fetchSummary: " << scanReturn.values.size() << " elements scan\n";

         Log::getInstance()->info(logstream.str());


         for(auto table_entry_iter = scanReturn.values.begin(); table_entry_iter != scanReturn.values.end();++table_entry_iter)
         {
            if(isAlreadyProvided(*table_entry_iter, adProvided))
                  continue;
            if(firsttime)
            {
               firsttime = false;
            }
            else
            {
               sstream << U(",\n");
            }
            sstream << U("{\n");
            for(auto attribute_iter = table_entry_iter->begin(); attribute_iter != table_entry_iter->end(); ++attribute_iter)
            {

               if(attribute_iter != table_entry_iter->begin() )
               {
                  sstream << ",\n";
               }

               if(attribute_iter->first !=  "SOURCES")
               {
                  sstream << "\"";
                  sstream << attribute_iter->first;
                  sstream << "\":\"";
                  sstream << attribute_iter->second;
                  sstream << "\"";
               }
               else
               {
                  sstream <<  U("\"SOURCES\": [\n");
                  std::vector<std::string> sources;
                  surfyn::utils::split(attribute_iter->second, ",", sources);
                  for(auto item_iter = sources.begin(); item_iter != sources.end(); ++item_iter)
                  {
                     if(item_iter != sources.begin())
                     {
                        sstream << U(",\n");
                     }
                     sstream << U("\"");
                     sstream << U(*item_iter);
                     sstream << U("\"");
                  }
                  sstream << U("]");
               }

            }
            sstream << U("\n}");
            updateProvidedAd(*table_entry_iter, adProvided);
         }
         scanend = scanReturn.scanend;
      }while(!scanend);
      sstream << U("\n]\n");

   }
   void DBaccess::fetchDetails(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query )
   {
      auto iter_id = query.find("id");
      if( iter_id == query.end())
      {
         Log::getInstance()->error("fetchDetails : no id found");
         return;
      }
      std::map<std::string, ValueType> attributestoget;

      ValueType value;
      value.field = "";
      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_field] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_announcelink] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_imagecount] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_surface] = value;

      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_nbrooms] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_transport] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_location] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_image] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_sourcelogo] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_floor] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_cellar] = value;

      GetResult _return;
      KeyValue key;
      key.key = "ID";
      key.value.fieldtype = Type::type::NUMBER;
      key.value.field  = iter_id->second;

      m_client->get(_return, details_table,key , attributestoget);


      std::map<std::string, std::string>::iterator iter_duplicate_ad = _return.values.end();
      sstream << U("[\n");
      sstream << U("{\n");
      for(auto iter_item = _return.values.begin(); iter_item != _return.values.end(); ++iter_item)
      {
         if(iter_item->first != id_duplicates)
         {
            if( iter_item != _return.values.begin())
            {
               sstream << U(",\n");
            }
            sstream << "\"";
            sstream << U(iter_item->first.c_str());
            sstream << "\":\"";
            sstream << U(iter_item->second.c_str());
            sstream << "\"";
         }
         else
         {
            iter_duplicate_ad = iter_item;
         }

      }
      sstream << U("}");

      if( iter_duplicate_ad != _return.values.end())
      {
         std::vector<std::string> duplicates;
         surfyn::utils::split(iter_duplicate_ad->second, ",", duplicates);

         for(auto item : duplicates)
         {
            key.value.field = item;
            m_client->get(_return, details_table,key , attributestoget);

            sstream << U(",\n{\n");
            for(auto iter_item = _return.values.begin(); iter_item != _return.values.end(); ++iter_item)
            {
               if(iter_item->first != id_duplicates)
               {
                  if( iter_item != _return.values.begin())
                  {
                     sstream << U(",\n");
                  }
                  sstream << "\"";
                  sstream << U(iter_item->first.c_str());
                  sstream << "\":\"";
                  sstream << U(iter_item->second.c_str());
                  sstream << "\"";
               }

            }
            sstream << U("\n}");
         }

      }
      sstream << U("\n]\n");

   }
}
}
