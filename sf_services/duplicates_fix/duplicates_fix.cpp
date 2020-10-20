/*
   (c) copyright 2020
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <dynamodb_access.h>
#include <dynamodb_access_types.h>
#include <sf_services/sf_utils/inc/Logger.h>
#include "sf_services/sf_utils/inc/Config.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include "sf_services/sf_classifier/inc/RealEstateAd.h"
#include <numeric>


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
const std::string exprval_city = ":ct";

using Log = surfyn::utils::Logger;


#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

namespace surfyn
{

   const std::string ID = "ID";
   const char* RealEstatePrice = "PRICE";
   const char* RealEstateSurface = "SURFACE";
   const char* RealEstateRooms = "ROOMS";
   const char* RealEstateKey = "ID";
   const char* RealEstateDuplicates = "DUPLICATES";
   const char* RealEstateCity = "CITY";
   const char* RealEstateType = "PROPERTY_TYPE";
   const char* RealEstateAnnounceSource = "ANNOUNCE_SOURCE";
   const char* RealEstateSources = "SOURCES";

   std::vector<classifier::RealEstateAd*> announcestoUpdate;

   ValueType BuildValueType(const std::string& fieldName, const std::string& fieldValue)
   {
      ValueType fieldNameValue;
      if(fieldName == RealEstatePrice || fieldName == RealEstateRooms || fieldName == RealEstateSurface)
      {
         std::string number_value = fieldValue;
         boost::erase_all(number_value, " ");
         fieldNameValue.field = number_value;
         fieldNameValue.fieldtype = Type::type::NUMBER;
      }
      else
      {
         fieldNameValue.field = fieldValue;
         fieldNameValue.fieldtype = Type::type::STRING;
      }

      return fieldNameValue;
   }
   void fillFilterExprAndExprValue(std::stringstream &filterexpression, std::map<std::string, ValueType> &expressionValue,
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

void readTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, std::vector<classifier::RealEstateAd*>& announcesID, const std::string& city,
const std::pair<std::string, std::string>& propType)
{
   std::map<std::string, ValueType> attributestoget;

   ValueType value;
   value.field = "";
   value.fieldtype = Type::type::NUMBER;
   attributestoget[ID] = value;
   attributestoget[RealEstatePrice] = value;
   attributestoget[RealEstateSurface] = value;
   attributestoget[RealEstateRooms] = value;

   value.fieldtype = Type::type::STRING;

   ///attributestoget[ANNOUNCE_LINK] = value;
   //attributestoget[ANNOUNCE_SOURCE] = value;
   //attributestoget[ANNOUNCE_TITLE] = value;
   attributestoget[RealEstateCity] = value;
   //attributestoget[PROPERTY_DESCRIPTION] = value;
   attributestoget[RealEstateType] = value;
   attributestoget[RealEstateAnnounceSource] = value;
   attributestoget[RealEstateSources] = value;


   bool scanend = false;
   do
   {
      ScanReqResult scanReturn;
      std::stringstream filterExpression;
      std::map<std::string, ValueType> expressionValue;
      fillFilterExprAndExprValue(filterExpression, expressionValue, "CITY", exprval_city, city, "=");
      filterExpression << " and ";
      filterExpression << "PROPERTY_TYPE";
      filterExpression << " in (";
      std::string expr = propType.second;
      filterExpression << expr;
      filterExpression << ")";

      ValueType exprValue;
      exprValue.field = propType.first;
      exprValue.fieldtype = Type::type::STRING;
      expressionValue[expr] = exprValue;
      client->scan(scanReturn, tableName, attributestoget, filterExpression.str(), expressionValue);

      std::stringstream logstream;
      logstream << "duplicates_fix: " << scanReturn.values.size() << " elements scan\n";

      Log::getInstance()->info(logstream.str());


      for (auto iter = scanReturn.values.begin(); iter != scanReturn.values.end(); ++iter)
      {
         int64_t id = atol((*iter)[RealEstateKey].c_str());

         classifier::RealEstateAd*  realEstate = new classifier::RealEstateAd(id);


         std::map<std::string, std::string>::const_iterator it_field;

         if ((it_field = iter->find(RealEstateCity)) != iter->end())
         {
            realEstate->setDescription(RealEstateCity, it_field->second);
         }
         if ((it_field = iter->find(RealEstateType)) != iter->end())
         {
            realEstate->setDescription(RealEstateType, it_field->second);
         }
         if ((it_field = iter->find(RealEstatePrice)) != iter->end())
         {
            realEstate->setDescription(RealEstatePrice, it_field->second);
         }
         if ((it_field = iter->find(RealEstateRooms)) != iter->end())
         {
            realEstate->setDescription(RealEstateRooms, it_field->second);
         }
         if ((it_field = iter->find(RealEstateSurface)) != iter->end())
         {
            realEstate->setDescription(RealEstateSurface, it_field->second);
         }
         if ((it_field = iter->find(RealEstateDuplicates)) != iter->end())
         {
            realEstate->setDescription(RealEstateDuplicates, it_field->second);
         }
         if ((it_field = iter->find(RealEstateSources)) != iter->end())
         {
            realEstate->setDescription(RealEstateSources, it_field->second);
         }
         if ((it_field = iter->find(RealEstateAnnounceSource)) != iter->end())
         {
            realEstate->setDescription(RealEstateAnnounceSource, it_field->second);
         }
         announcesID.push_back(realEstate);
      }

      scanend = scanReturn.scanend;
   } while (!scanend);

}
bool IsSimilar(classifier::RealEstateAd* lhs, classifier::RealEstateAd* rhs)
{
   std::string lhs_raw_price = lhs->getDescription(RealEstatePrice);
   std::string rhs_raw_price = rhs->getDescription(RealEstatePrice);
   int price_lhs = atoi(lhs_raw_price.c_str());
   int price_rhs = atoi(rhs_raw_price.c_str());

   std::string lhs_raw_rooms = lhs->getDescription(RealEstateRooms);
   std::string rhs_raw_rooms = rhs->getDescription(RealEstateRooms);
   int rooms_lhs = atoi(lhs_raw_rooms.c_str());
   int rooms_rhs = atoi(rhs_raw_rooms.c_str());

   std::string lhs_raw_surface = lhs->getDescription(RealEstateSurface);
   std::string rhs_raw_surface = rhs->getDescription(RealEstateSurface);
   float surface_lhs = atof(lhs_raw_surface.c_str());
   float surface_rhs = atof(rhs_raw_surface.c_str());

   return (price_lhs == price_rhs) && (rooms_lhs == rooms_rhs) &&
         (abs(surface_lhs -surface_rhs) < 1) ;
}

void updateGroupOfDuplicates(std::vector<classifier::RealEstateAd*>& duplicatesAd)
{
   for(auto* ad: duplicatesAd)
   {
      std::string duplicates = "";
      std::string source = ad->getDescription(RealEstateAnnounceSource);

      for(auto* dup : duplicatesAd)
      {
         if(dup != ad )
         {
            if(!duplicates.empty())
               duplicates += ",";

            duplicates += std::to_string(dup->getId());
            source += ",";
            source += dup->getDescription(RealEstateAnnounceSource);
         }

      }
      ad->setDescription(RealEstateDuplicates, duplicates);
      ad->setDescription(RealEstateSources, source);
   }
}

void updateDuplicates(std::vector<classifier::RealEstateAd*>& announcesID)
{
   for (auto it = announcesID.begin(), it_next = std::next(it, 1); it != announcesID.end() && it_next != announcesID.end(); )
   {
      std::string similarAnnounces = (*it)->getDescription(RealEstateDuplicates);

      if( similarAnnounces.empty())
      {
         std::vector<classifier::RealEstateAd*> duplicatesAd;
         bool firstTime = true;
         while(it_next != announcesID.end() && IsSimilar(*it, *it_next))
         {
            if(firstTime)
            {
               duplicatesAd.push_back(*it);
               announcestoUpdate.push_back(*it);
               firstTime = false;
            }
            duplicatesAd.push_back(*it_next);
            announcestoUpdate.push_back(*it_next);
            ++it_next;
         }
         if(!duplicatesAd.empty())
         {
            updateGroupOfDuplicates(duplicatesAd);
            it = it_next;
         }
         else
         {
            ++it;
         }
         ++it_next;
      }
      else
      {
         ++it;
         ++it_next;
      }
   }
}

void updateTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName)
{
   for (auto* ad : announcestoUpdate)
   {
      std::map<std::string, ValueType> valuesToUpdate;

      std::string fieldValue = ad->getDescription(RealEstateDuplicates);
      ValueType value = BuildValueType(RealEstateDuplicates, fieldValue);
      valuesToUpdate[RealEstateDuplicates] = value;

      fieldValue = ad->getDescription(RealEstateSources);
      value = BuildValueType(RealEstateSources, fieldValue);
      valuesToUpdate[RealEstateSources] = value;



      OperationResult result;
      KeyValue Key;
      Key.key = ID;
      ValueType IDValue;
      IDValue.field = std::to_string(ad->getId());
      IDValue.fieldtype = Type::type::NUMBER;
      Key.value = IDValue;
      client->update(result, tableName, Key, valuesToUpdate);
      if (!result.success)
      {
         Log::getInstance()->error("Failed to update " + std::to_string(ad->getId()) + " into table " + tableName + " error : " + result.error);
      }
      else
      {
         Log::getInstance()->info(" Succesfully update [" + std::to_string(ad->getId()) + "] into table " + tableName );
      }

   }
}
} //end surfyn namespace

struct Compare
{
   bool operator()(surfyn::classifier::RealEstateAd* announce1, surfyn::classifier::RealEstateAd* announce2)
   {
      std::string raw_price1 = announce1->getDescription(surfyn::RealEstatePrice);
      std::string raw_price2 = announce2->getDescription(surfyn::RealEstatePrice);

      return atoi(raw_price1.c_str()) < atoi(raw_price2.c_str());
   }
};

int main(int argc, char* argv[])
{
   std::vector<surfyn::classifier::RealEstateAd*> announces;

   Log::Init("duplicates_fix");
   Log::getInstance()->info("Starting duplicates_fix ...");
   
   std::string tablename = "";
   int port = 0;
   std::string host = "", city = "";
   if(argc == 3)
   {
      surfyn::utils::Config duplicates_fix_conf(argv[1]);
      duplicates_fix_conf.loadconfig();
      tablename = duplicates_fix_conf.getStringValue("tablename");

      host = duplicates_fix_conf.getStringValue("host");
      port = duplicates_fix_conf.getIntValue("port");
      city = argv[2];
   }
   shared_ptr<TTransport> socket(new TSocket(host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client = std::make_shared<dynamodb_accessClient>(protocol);
   transport->open();

   std::vector<std::pair <std::string, std::string>> propertyTypes = {{"Maison",":pyMaison"}, {"Appartement",":pyAppartement"}};

   for(auto propType: propertyTypes)
   {
      surfyn::readTable(client, tablename, announces, city, propType);
      std::sort(announces.begin(), announces.end(), Compare());
      surfyn::updateDuplicates(announces);
      surfyn::updateTable(client, tablename);
   }
   
   Log::getInstance()->info("Table " + tablename + " successfully updated ");
   for( auto * ad : announces)
   {
      delete ad;
   }
   return 0;
}
