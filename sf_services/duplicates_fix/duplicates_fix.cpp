/*
   (c) copyright 2020
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <string>
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
#include <unordered_set>
#include <math.h>

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

#define CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_INT(attribute, filedName) \
   std::string left##attribute##Str = leftAnnounce->getDescription(filedName); \
   std::string right##attribute##Str = rightAnnounce->getDescription(filedName); \
   if (!left##attribute##Str.empty() && !right##attribute##Str.empty()) \
   { \
      int left##attribute##Nb = atoi(left##attribute##Str.c_str()); \
      int right##attribute##Nb = atoi(right##attribute##Str.c_str()); \
      if (left##attribute##Nb != right##attribute##Nb) \
      { \
         logStream << leftAnnounce->getId() << " ##attribute##[" << left##attribute##Nb << "] and " << rightAnnounce->getId() << " ##attribute##[" \
            << right##attribute##Nb << "] are not similar as their ##attribute## differ"; \
         Log::getInstance()->info(logStream.str()); \
         return false; \
      } \
   } \
   else \
   { \
         logStream << leftAnnounce->getId() << " ##attribute##[" << left##attribute##Str << "] and " << rightAnnounce->getId() << " ##attribute##[" \
            << right##attribute##Str << "] are not similar as one of their ##attribute## is empty"; \
         Log::getInstance()->info(logStream.str()); \
         return false; \
   }

#define CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_STRING(attribute, filedName) \
   std::string left##attribute##Str = leftAnnounce->getDescription(filedName); \
   std::string right##attribute##Str = rightAnnounce->getDescription(filedName); \
   if (!left##attribute##Str.empty() && !right##attribute##Str.empty()) \
   { \
      if (left##attribute##Str != right##attribute##Str) \
      { \
         logStream << leftAnnounce->getId() << " ##attribute##[" << left##attribute##Str << "] and " << rightAnnounce->getId() << " ##attribute##[" \
            << right##attribute##Str << "] are not similar as their ##attribute## differ"; \
         Log::getInstance()->info(logStream.str()); \
         return false; \
      } \
   } \
   else \
   { \
         logStream << leftAnnounce->getId() << " ##attribute##[" << left##attribute##Str << "] and " << rightAnnounce->getId() << " ##attribute##[" \
            << right##attribute##Str << "] are not similar as their ##attribute## differ"; \
         Log::getInstance()->info(logStream.str()); \
         return false; \
   }

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
   const char* SEARCH_TYPE = "SEARCH_TYPE";
   const char* FIRST_TIMESTAMP = "FIRST_TIMESTAMP";
   const char* RealEstateSearchType = "SEARCH_TYPE";
   static double priceMaxErrorPercentage = 0.001;
   static double surfaceMaxErrorPercentage = 1.0;
   

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

void readTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, std::vector<std::shared_ptr<classifier::RealEstateAd>>& announcesID, const std::string& city,
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
   attributestoget[SEARCH_TYPE] = value;
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

         auto  realEstate = std::make_shared<classifier::RealEstateAd>(id);


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
         if ((it_field = iter->find(SEARCH_TYPE)) != iter->end())
         {
            realEstate->setDescription(SEARCH_TYPE, it_field->second);
         }
         if ((it_field = iter->find(FIRST_TIMESTAMP)) != iter->end())
         {
            realEstate->setDescription(FIRST_TIMESTAMP, it_field->second);
         }
         
         announcesID.push_back(realEstate);
      }

      scanend = scanReturn.scanend;
   } while (!scanend);

   std::stringstream msg;
   msg <<  announcesID.size() << " elements loaded from databases\n";
   Log::getInstance()->info(msg.str());
}

/*bool isNotTooFar(std::shared_ptr<classifier::RealEstateAd>& lhs, std::shared_ptr<classifier::RealEstateAd>& rhs, int maxDelayToConsider)
{
   const auto lhsFirsttimestamp = lhs->getDescription(FIRST_TIMESTAMP);
   struct tm lhsFirst;
   strptime(lhsFirsttimestamp.c_str(), "%Y-%m-%dT%H:%M:%S", &lhsFirst);
   time_t lhs_modification_time = timegm(&lhsFirst);

   const auto rhsFirsttimestamp = rhs->getDescription(FIRST_TIMESTAMP);
   struct tm rhsFirst;
   strptime(rhsFirsttimestamp.c_str(), "%Y-%m-%dT%H:%M:%S", &rhsFirst);
   time_t rhs_modification_time = timegm(&rhsFirst);

   return std::abs(lhs_modification_time - rhs_modification_time) < maxDelayToConsider;
}*/

  bool IsSimilar_v2(const std::shared_ptr<classifier::RealEstateAd>& leftAnnounce, const std::shared_ptr<classifier::RealEstateAd>& rightAnnounce)
   {
      if( leftAnnounce->getId() == rightAnnounce->getId())
         return false;
      std::stringstream logStream;
      std::string leftPriceStr = leftAnnounce->getDescription(RealEstatePrice);
      std::string rightPriceStr = rightAnnounce->getDescription(RealEstatePrice);
      if (!leftPriceStr.empty() && !rightPriceStr.empty())
      {
         double leftPrice = atof(leftPriceStr.c_str());
         double rightPrice = atof(rightPriceStr.c_str());
         if (fabs(leftPrice - rightPrice) / leftPrice > priceMaxErrorPercentage)
         {
            logStream << leftAnnounce->getId() << " price[" << leftPrice << "] and " << rightAnnounce->getId() << " price[" 
               << rightPrice << "] are not similar as their prices differ";
            Log::getInstance()->info(logStream.str());
            return false;
         }
      }
      else 
      {
      logStream << leftAnnounce->getId() << " price[" << leftPriceStr << "] and " << rightAnnounce->getId() << " price[" 
               << rightPriceStr << "] are not similar as one price is empty";
            Log::getInstance()->info(logStream.str());
            return false;
      }
      std::string leftSurfaceStr = leftAnnounce->getDescription(RealEstateSurface);
      std::string rightSurfaceStr = rightAnnounce->getDescription(RealEstateSurface);
      if (!leftSurfaceStr.empty() && !rightSurfaceStr.empty())
      {
         double leftSurface = atof(leftSurfaceStr.c_str());
         double rightSurface = atof(rightSurfaceStr.c_str());
         if (fabs(leftSurface - rightSurface) >= surfaceMaxErrorPercentage)
         {
            logStream << leftAnnounce->getId() << " surface[" << leftSurface << "] and " << rightAnnounce->getId() << " surface["
               << rightSurface << "] are not similar as their surfaces differ";
            Log::getInstance()->info(logStream.str());
            return false;
         }
      }
      else
      {
            logStream << leftAnnounce->getId() << " surface[" << leftSurfaceStr << "] and " << rightAnnounce->getId() << " surface["
               << rightSurfaceStr << "] are not similar as one surfaces is empty";
            Log::getInstance()->info(logStream.str());
            return false;
      }
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_INT(Room, RealEstateRooms);

      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_STRING(RealEstateType, RealEstateType); 
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_STRING(RealEstateSearchType, RealEstateSearchType);
      return true;
   }

/*bool IsSimilar(const std::shared_ptr<classifier::RealEstateAd>& lhs, const std::shared_ptr<classifier::RealEstateAd>& rhs, int maxDelayToConsider)
{
   Log::getInstance()->info("IsSimilar: checking if [" + std::to_string(lhs->getId()) + "] and [" + std::to_string(rhs->getId()) + "] can be similar");
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
         (std::abs(surface_lhs -surface_rhs) < 1
         && rhs->getDescription(RealEstateType) == lhs->getDescription(RealEstateType)
         && rhs->getDescription(SEARCH_TYPE) == lhs->getDescription(SEARCH_TYPE)
         && isNotTooFar(lhs, rhs, maxDelayToConsider)) ;
}*/

void updateGroupOfDuplicates(std::vector<std::shared_ptr<classifier::RealEstateAd>>& duplicatesAd)
{
   for(auto ad: duplicatesAd)
   {
      std::string duplicates = ad->getDescription(RealEstateDuplicates);
      boost::erase_all(duplicates, " ");
      std::unordered_set<std::string> uniqueDuplicatesIds;
      boost::split(uniqueDuplicatesIds, duplicates, [](char c) { return c == ','; });

      
      std::string sources = ad->getDescription(RealEstateSources);
      boost::erase_all(sources, " ");
      std::unordered_set<std::string> uniqueSources;
      boost::split(uniqueSources, sources, [](char c) { return c == ','; });

      for(auto dup : duplicatesAd)
      {
         if(dup != ad )
         {
            uniqueDuplicatesIds.insert(std::to_string(dup->getId()));
            uniqueSources.insert(dup->getDescription(RealEstateAnnounceSource));
         }

      }

      duplicates = "";
      for(auto strId: uniqueDuplicatesIds)
      {
         if(!strId.empty())
         {
            if(!duplicates.empty())
            {
               duplicates += ",";
            }
            duplicates += strId;
         }

      }

      sources = "";
      for(auto strSource: uniqueSources)
      {
         if(!strSource.empty())
         {
            if(!sources.empty())
            {
               sources += ",";
            }
            sources += strSource;
         }
         
      }

      Log::getInstance()->info("Announce ["+ std::to_string(ad->getId()) +"] list of duplicates [" + duplicates + "] list of sources [" + sources + "]");
      ad->setDescription(RealEstateDuplicates, duplicates);
      ad->setDescription(RealEstateSources, sources);
   }
}

void updateDuplicates(const std::vector<std::shared_ptr<classifier::RealEstateAd>>& announcesID, std::vector<std::shared_ptr<surfyn::classifier::RealEstateAd>>& announcestoUpdate)
{
   Log::getInstance()->info("updateDuplicates: Begin to check for new duplicates");
   for (auto it = announcesID.begin(), it_next = std::next(it, 1); it != announcesID.end() && it_next != announcesID.end(); )
   {
      std::string similarAnnounces = (*it)->getDescription(RealEstateDuplicates);

      std::vector<std::shared_ptr<classifier::RealEstateAd>> duplicatesAd;
      bool firstTime = true;
      while(it_next != announcesID.end() && IsSimilar_v2(*it, *it_next))
      {
         Log::getInstance()->info("updateDuplicates: Announce [" + std::to_string((*it)->getId()) + "] and Announce ["
         + std::to_string((*it_next)->getId()) + " are Similar");
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
}

void updateTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, const std::vector<std::shared_ptr<surfyn::classifier::RealEstateAd>>& announcestoUpdate)
{
   Log::getInstance()->info("updateTable: "+std::to_string(announcestoUpdate.size()) + " elements to update!");
   for (auto ad : announcestoUpdate)
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
   bool operator()(std::shared_ptr<surfyn::classifier::RealEstateAd> announce1, std::shared_ptr<surfyn::classifier::RealEstateAd> announce2)
   {
      std::string raw_price1 = announce1->getDescription(surfyn::RealEstatePrice);
      std::string raw_price2 = announce2->getDescription(surfyn::RealEstatePrice);

      if( atoi(raw_price1.c_str()) < atoi(raw_price2.c_str()) )
         return true;
      else 
      {
         if(atoi(raw_price1.c_str()) > atoi(raw_price2.c_str()))
            return false;
         else 
         {
            std::string rooms1 = announce1->getDescription(surfyn::RealEstateRooms);
            std::string rooms2 = announce2->getDescription(surfyn::RealEstateRooms);

            if( atoi(rooms1.c_str()) < atoi(rooms2.c_str()) )
                  return true;
            else 
            {
               if(atoi(rooms1.c_str()) > atoi(rooms2.c_str()))
                  return false;
               else 
               {
                  auto surface1 = announce1->getDescription(surfyn::RealEstateSurface);
                  auto surface2 = announce2->getDescription(surfyn::RealEstateSurface);

                  return atof(surface1.c_str()) < atof(surface2.c_str());
               }
            }
         }
      }
   }
};

int main(int argc, char* argv[])
{
   

   Log::Init("duplicates_fix");
   Log::getInstance()->info("Starting duplicates_fix ...");
   
   std::string tablename = "";
   int port = 0;
   std::string host = "", city = "";
   //int maxDelayToConsider = 1209600;
   if(argc == 3)
   {
      surfyn::utils::Config duplicates_fix_conf(argv[1]);
      duplicates_fix_conf.loadconfig();
      tablename = duplicates_fix_conf.getStringValue("tablename");

      host = duplicates_fix_conf.getStringValue("host");
      port = duplicates_fix_conf.getIntValue("port");
      surfyn::priceMaxErrorPercentage = duplicates_fix_conf.getDoubleValue("price_max_error_percentage");
      surfyn::surfaceMaxErrorPercentage = duplicates_fix_conf.getDoubleValue("surface_max_error_percentage");
      Log::getInstance()->info("duplicates_fix: Initializing priceMaxErrorPercentage with value [" + std::to_string(surfyn::priceMaxErrorPercentage) + "]");
      Log::getInstance()->info("duplicates_fix: Initializing surfaceMaxErrorPercentage with value [" + std::to_string(surfyn::surfaceMaxErrorPercentage) + "]");
      //maxDelayToConsider = duplicates_fix_conf.getIntValue("max_delay_to_consider");
      city = argv[2];
   }
   else
   {
      std::cout << "[ERROR] not enough parameters in command line\n";
      std::cout << "please run ./duplicate_fix config.ini city \n";
      return 1;
   }
   shared_ptr<TTransport> socket(new TSocket(host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client = std::make_shared<dynamodb_accessClient>(protocol);
   transport->open();

   std::vector<std::pair <std::string, std::string>> propertyTypes = {{"Maison",":pyMaison"}, {"Appartement",":pyAppartement"}};

   for(auto propType: propertyTypes)
   {
      std::vector<std::shared_ptr<surfyn::classifier::RealEstateAd>> announces;
      std::vector<std::shared_ptr<surfyn::classifier::RealEstateAd>> announcestoUpdate;

      surfyn::readTable(client, tablename, announces, city, propType);
      std::sort(announces.begin(), announces.end(), Compare());
      surfyn::updateDuplicates(announces, announcestoUpdate);
      surfyn::updateTable(client, tablename, announcestoUpdate);
   }
   
   Log::getInstance()->info("Table " + tablename + " successfully updated ");

   return 0;
}
