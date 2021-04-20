/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "DBaccess.h"



#include <bits/stdint-uintn.h>
#include <sstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "sf_services/sf_utils/inc/Str.h"
#include "sf_services/sf_utils/inc/Logger.h"
#include <boost/algorithm/string.hpp>


using Log = surfyn::utils::Logger;


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
const std::string id_lift = "LIFT";
const std::string id_typeofheating = "TYPE_OF_HEATING";
const std::string id_constructionyear = "CONSTRUCTION_YEAR";
const std::string id_parking = "PARKING";
const std::string id_searchtype = "SEARCH_TYPE";
const std::string id_timestamp = "TIMESTAMP";
const std::string id_first_timestamp = "FIRST_TIMESTAMP";
const std::string id_bedrooms = "BEDROOMS";
const std::string id_landsurface = "LAND_SURFACE";
const std::string id_adtextdescription = "AD_TEXT_DESCRIPTION";

const std::string details_table = "FR_SUMMARY_COPY";
const std::string exprval_city = ":ct";
const std::string exprval_searchType = ":st";
const std::string exprval_priceMin = ":pi";
const std::string exprval_priceMax = ":pa";
const std::string exprval_propType = ":py";
const std::string exprval_areaMin = ":ai";
const std::string exprval_areaMax = ":aa";
const std::string exprval_rooms = ":ro";

const std::string city_info_table = "FR_CITIES_DESCRIPTION";
const std::string descriptionAttribute = "DESCRIPTION";

std::string searchTypeValue = "";

   DBaccess::DBaccess(const std::string& host, int port)
   {
      shared_ptr<TTransport> socket(new TSocket(host, port));
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
      exprValue.fieldtype = (param == "PRICE" || param == "SURFACE" || param == "ROOMS") ? Type::type::NUMBER : Type::type::STRING;
      expressionValue[paramvalue] = exprValue;
   }
   bool DBaccess::isAlreadyProvided(const std::string& id, const std::unordered_set<std::string>& adprovided)
   {
      const auto iter = adprovided.find(id);

      return iter != adprovided.end() ;
   }
   void DBaccess::updateProvidedAd(const std::string& dup,  std::unordered_set<std::string>& adprovided)
   {
      std::vector<std::string> duplicates;
      boost::split(duplicates, dup, [](char c) { return c == ','; });

      for(auto item : duplicates)
      {
         adprovided.insert(item);
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

      value.fieldtype = Type::type::STRING;
      attributestoget[id_timestamp] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_first_timestamp] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_adtextdescription] = value;
      

      bool scanend = false;


      // search anounces on summary table
      // and construct a json string

      std::vector<std::map<std::string, std::string> >  results;

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
         else {
               sstream << U("[]");
               return;
         }

         iter = query.find("search_type");
         if( iter != query.end() )
         {
            filterExpression << " and ";
            searchTypeValue = strcmp((iter->second).c_str(), "1") == 0 ? "For sale":"For rent";
            fillFilterExprAndExprValue(filterExpression, expressionValue, "SEARCH_TYPE", exprval_searchType, searchTypeValue , "=");
         }
         else
         {
               sstream << U("[]");
               return;
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
            boost::split(output, iter->second, [](char c) { return c == ','; });
            for(auto iter_value = output.begin();iter_value != output.end(); ++iter_value)
            {
               if( iter_value != output.begin() )
               filterExpression << ",";
               std::string propertyType = *iter_value == "1" || *iter_value == "3" ? "Appartement" : "Maison";

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
         iter = query.find("rooms");
         if( iter != query.end() )
         {
            filterExpression << " and ";

            fillFilterExprAndExprValue(filterExpression, expressionValue, "ROOMS", exprval_rooms, (iter->second).c_str(), "=");
         }


         m_client->scan(scanReturn, details_table, attributestoget, filterExpression.str(), expressionValue);


         std::stringstream logstream;
         logstream << "fetchSummary: " << scanReturn.values.size() << " elements scan\n";

         Log::getInstance()->info(logstream.str());

         results.reserve(results.size() + scanReturn.values.size());
         results.insert(results.end(), scanReturn.values.begin(), scanReturn.values.end());

         scanend = scanReturn.scanend;
      }while(!scanend);

      buildJsonString(sstream, results);
   }

   void DBaccess::buildJsonString(utility::stringstream_t& sstream,
                                  std::vector<std::map<std::string, std::string> >& results)
   {
      sstream << U("[\n");

      std::unordered_set<std::string> adProvided;
      bool firsttime = true;

      for(auto announce : results)
      {
         auto iter_id = announce.find(id_field);
         if(iter_id == announce.end())
            continue;
         if(isAlreadyProvided(iter_id->second, adProvided))
            continue;
         else 
         {
            adProvided.insert(iter_id->second);
         }
         if(firsttime)
         {
            firsttime = false;
         }
         else
         {
            sstream << U(",\n");
         }
         sstream << U("{\n");
         for(auto attribute_iter = announce.begin(); attribute_iter != announce.end(); ++attribute_iter)
         {

            if(attribute_iter != announce.begin() )
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

               if(attribute_iter->first == "DUPLICATES")
               {
                  updateProvidedAd(attribute_iter->second, adProvided);
               }
            }
            else
            {
               sstream <<  U("\"SOURCES\": [\n");
               std::vector<std::string> sources;
               boost::split(sources, attribute_iter->second, [](char c) { return c == ','; });
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
      }
      sstream << U("\n]\n");
   }
   void DBaccess::fetchDetails(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query, 
   const std::shared_ptr<surfyn::utils::GeoLocal>&  geolocalservice )
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
      attributestoget[id_property_type] = value;

      value.fieldtype = Type::type::NUMBER;
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

      value.fieldtype = Type::type::NUMBER;
      attributestoget[id_price] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_constructionyear] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_lift] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_typeofheating] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_parking] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_searchtype] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_city] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_bedrooms] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_landsurface] = value;

      value.fieldtype = Type::type::STRING;
      attributestoget[id_adtextdescription] = value;

      GetResult _return;
      KeyValue key;
      key.key = "ID";
      key.value.fieldtype = Type::type::NUMBER;
      key.value.field  = iter_id->second;

      m_client->get(_return, details_table,key , attributestoget);

      // if we have nothing, directly return empty list
      if(_return.values.empty())
      {
         sstream << U("[]");
         return;
      }
      std::map<std::string, std::string>::iterator iter_duplicate_ad = _return.values.end();
      std::string searchCity = "", searchType = "", propertyType = "", medianPrice = "";
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
            if( iter_item->first == id_city )
               searchCity  = iter_item->second;
            if( iter_item->first == id_searchtype )
               searchType = iter_item->second;
            if( iter_item->first == id_property_type )
               propertyType = iter_item->second;

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
      //populating median price for the city
      if(!searchCity.empty() && searchType == "For sale")
      {
         boost::to_upper(searchCity);
         auto inseeCode = geolocalservice->getInseeCode(searchCity);
         Log::getInstance()->info(" inseeCode = [" + inseeCode +"] City = " + searchCity );

         GetResult inseeCodereturn;
         KeyValue inseeCodeKey;
         inseeCodeKey.key = "ID";
         inseeCodeKey.value.fieldtype = Type::type::NUMBER;
         inseeCodeKey.value.field  = inseeCode;

         std::map<std::string, ValueType> infostoget;

         ValueType medianpricevalue;
         medianpricevalue.field = "";
         medianpricevalue.fieldtype = Type::type::NUMBER;
         std::string attributeName = "";
         if( propertyType == "Appartement")
            attributeName = "MEDIAN_PRICE_BY_M2_FLAT";
         else {
            attributeName = "MEDIAN_PRICE_BY_M2_HOUSE";
         }
         infostoget[attributeName] = medianpricevalue;

         m_client->get(inseeCodereturn, "FR_PRICEBYM2_INFOS",inseeCodeKey , infostoget);

         if( inseeCodereturn.result.success)
         {
            auto iter_medianPrice = inseeCodereturn.values.find(attributeName);
            if( iter_medianPrice != inseeCodereturn.values.end())
            {
               medianPrice = iter_medianPrice->second;
               Log::getInstance()->info(" Median price found " + medianPrice);

               sstream << U(",\n");
               sstream << "\"";
               sstream << U("MEDIAN_PRICE_BY_M2");
               sstream << "\":\"";
               sstream << U(medianPrice.c_str());
               sstream << "\"";
            }
         }
         else {
            std::stringstream error_msg;
            error_msg << " Fail to get median price for city [";
            error_msg << searchCity;
            error_msg << "] ";
            error_msg << " inseeCode [";
            error_msg << inseeCode;
            error_msg << "] ";
            error_msg << inseeCodereturn.result.error;

          Log::getInstance()->error(error_msg.str());
         }

      }
      sstream << U("}");

      if( iter_duplicate_ad != _return.values.end())
      {
         std::vector<std::string> duplicates;
         boost::split(duplicates, iter_duplicate_ad->second, [](char c) { return c == ','; } );

         for(auto item : duplicates)
         {
            key.value.field = item;
            m_client->get(_return, details_table,key , attributestoget);

            if(_return.values.size() > 0)
            {
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
               if(!medianPrice.empty())
               {
                  sstream << U(",\n");
                  sstream << "\"";
                  sstream << U("MEDIAN_PRICE_BY_M2");
                  sstream << "\":\"";
                  sstream << U(medianPrice.c_str());
                  sstream << "\"";
               }
               sstream << U("\n}");
            }
            
         }

      }
      sstream << U("\n]\n");

   }

   void DBaccess::fetchHighAndLowPriceByM2(const std::string& cityId, bool isFlat, std::string& highpricebym2, std::string& lowpricebym2)
   {
      GetResult inseeCodereturn;
      KeyValue inseeCodeKey;
      inseeCodeKey.key = "ID";
      inseeCodeKey.value.fieldtype = Type::type::NUMBER;
      inseeCodeKey.value.field  = cityId;

      std::map<std::string, ValueType> infostoget;

      ValueType highpricevalue;
      highpricevalue.field = "";
      highpricevalue.fieldtype = Type::type::NUMBER;
      std::string HighPriceAttributeName = "";
      if( isFlat )
         HighPriceAttributeName = "HIGH_PRICE_BY_M2_FLAT";
      else {
         HighPriceAttributeName = "HIGH_PRICE_BY_M2_HOUSE";
      }
      infostoget[HighPriceAttributeName] = highpricevalue;

      ValueType lowpricevalue;
      lowpricevalue.field = "";
      lowpricevalue.fieldtype = Type::type::NUMBER;
      std::string LowPriceAttributeName = "";
      if( isFlat )
         LowPriceAttributeName = "LOW_PRICE_BY_M2_FLAT";
      else {
         LowPriceAttributeName = "LOW_PRICE_BY_M2_HOUSE";
      }
      infostoget[LowPriceAttributeName] = lowpricevalue;

      m_client->get(inseeCodereturn, "FR_PRICEBYM2_INFOS",inseeCodeKey , infostoget);

      if(inseeCodereturn.result.success)
      {
         auto iter_highprice = inseeCodereturn.values.find(HighPriceAttributeName);
         if(iter_highprice != inseeCodereturn.values.end())
            highpricebym2 = iter_highprice->second;

         auto iter_lowprice = inseeCodereturn.values.find(LowPriceAttributeName);
         if(iter_lowprice != inseeCodereturn.values.end())
            lowpricebym2 = iter_lowprice->second;
      }
      else 
      {
         std::stringstream msg;
         msg << "Fail to get price by m2 infos for city insee code [";
         msg << cityId;
         msg << "] error msg => ";
         msg << inseeCodereturn.result.error;
         Log::getInstance()->error(msg.str());
      }
   }

   void DBaccess::fetchCityInfo(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query, 
   const std::shared_ptr<surfyn::utils::GeoLocal>&  geolocalservice)
   {
      auto iter_city = query.find("city");
      if( iter_city == query.end())
      {
         Log::getInstance()->error("fetchCityInfo : no city found");
         return;
      }
      std::string city = iter_city->second;
      boost::to_upper(city);
      auto inseeCode = geolocalservice->getInseeCode(city);

      GetResult cityDescriptionreturn;
      KeyValue inseeCodeKey;
      inseeCodeKey.key = "ID";
      inseeCodeKey.value.fieldtype = Type::type::NUMBER;
      inseeCodeKey.value.field  = inseeCode;

      std::map<std::string, ValueType> infostoget;

      ValueType descriptionValue;
      descriptionValue.field = "";
      descriptionValue.fieldtype = Type::type::STRING;

      infostoget[descriptionAttribute] = descriptionValue;

      m_client->get(cityDescriptionreturn, city_info_table,inseeCodeKey , infostoget);

      if(cityDescriptionreturn.result.success)
      {
         auto iter_desc = cityDescriptionreturn.values.find(descriptionAttribute);
         if(iter_desc != cityDescriptionreturn.values.end())
            sstream << U(iter_desc->second.c_str());
         else
            sstream << "{}";

      }
      else 
      {
         std::stringstream msg;
         msg << "Fail to get description for city insee code [";
         msg << inseeCode;
         msg << "] error msg => ";
         msg << cityDescriptionreturn.result.error;
         Log::getInstance()->error(msg.str());
      }
   }
}
}
