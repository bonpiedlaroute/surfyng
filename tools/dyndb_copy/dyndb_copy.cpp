/*
   (c) copyright 2019
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

const int port = 5050;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using Log = surfyn::utils::Logger;


#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

namespace surfyn
{
   // Fields copied from sf_classifier
   const std::string ID = "ID";
   const std::string ANNOUNCE_LINK = "ANNOUNCE_LINK";
   const std::string ANNOUNCE_SOURCE = "ANNOUNCE_SOURCE";
   const std::string ANNOUNCE_TITLE = "ANNOUNCE_TITLE";
   const std::string CITY = "CITY";
   const std::string PROPERTY_DESCRIPTION = "PROPERTY_DESCRIPTION";
   const std::string PROPERTY_TYPE = "PROPERTY_TYPE";
   const std::string REGION = "REGION";
   const std::string SEARCH_TYPE = "SEARCH_TYPE";
   const std::string TIMESTAMP = "TIMESTAMP";
   const std::string SIMILAR_ANNOUNCE = "TF_SIMILAR_ANNOUNCE";
   const std::string ANNOUNCE_IMAGE = "ANNOUNCE_IMAGE";
   const std::string IMAGE_COUNT = "IMAGE_COUNT";


   const char* RealEstatePrice = "PRICE";
   const char* RealEstateSurface = "SURFACE";
   const char* RealEstateConstructionYear = "CONSTRUCTION_YEAR";
   const char* RealEstateTimeToPublicTransport = "TIME_TO_PUBLICTRANSPORT";
   const char* RealEstateBeds = "BEDS";
   const char* RealEstateCity = "CITY";
   const char* RealEstateType = "PROPERTY_TYPE";
   const char* RealEstateRooms = "ROOMS";
   const char* RealEstateTypeOfHeating = "TYPE_OF_HEATING";
   const char* RealEstateCellar = "CELLAR";
   const char* RealEstateFloor = "FLOOR";
   const char* RealEstateParking = "PARKING";
   const char* RealEstateBox = "BOX";
   const char* RealEstateSearchType = "SEARCH_TYPE";
   const char* RealEstateLandSurface = "LAND_SURFACE";
   const char* RealEstateLift = "LIFT";
   const char* RealEstateBalcony = "BALCONY";
   const char* RealEstateKey = "ID";
   const char* RealEstateSimilarAd = "SIMILAR_AD";
   const char* RealEstateLocation = "LOCATION";

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

   void readTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, std::unordered_map<int64_t, classifier::RealEstateAd>& announcesByID)
   {
      std::map<std::string, ValueType> attributestoget;

      ValueType value;
      value.field = "";
      value.fieldtype = Type::type::NUMBER;
      attributestoget[ID] = value;

      value.fieldtype = Type::type::STRING;

      attributestoget[ANNOUNCE_LINK] = value;
      attributestoget[ANNOUNCE_SOURCE] = value;
      attributestoget[ANNOUNCE_TITLE] = value;
      attributestoget[CITY] = value;
      attributestoget[PROPERTY_DESCRIPTION] = value;
      attributestoget[PROPERTY_TYPE] = value;
      attributestoget[REGION] = value;
      attributestoget[SEARCH_TYPE] = value;
      attributestoget[TIMESTAMP] = value;
      attributestoget[SIMILAR_ANNOUNCE] = value;
      attributestoget[ANNOUNCE_IMAGE] = value;
      attributestoget[IMAGE_COUNT] = value;

      bool scanend = false;
      do
      {
         ScanReqResult scanReturn;
         client->scan(scanReturn, tableName, attributestoget, "", std::map<std::string, ValueType>());

         std::stringstream logstream;
         logstream << "dyndb_copy: " << scanReturn.values.size() << " elements scan\n";

         Log::getInstance()->info(logstream.str());


         for (auto iter = scanReturn.values.begin(); iter != scanReturn.values.end(); ++iter)
         {
            int64_t id = atol((*iter)[RealEstateKey].c_str());

            classifier::RealEstateAd realEstate(id);


            std::map<std::string, std::string>::const_iterator it_field;
            if ((it_field = iter->find(ANNOUNCE_LINK)) != iter->end())
            {
               realEstate.setDescription(ANNOUNCE_LINK, it_field->second);
            }
            if ((it_field = iter->find(RealEstateCity)) != iter->end())
            {
               realEstate.setDescription(RealEstateCity, it_field->second);
            }
            if ((it_field = iter->find(RealEstateType)) != iter->end())
            {
               realEstate.setDescription(RealEstateType, it_field->second);
            }
            if ((it_field = iter->find(RealEstateSearchType)) != iter->end())
            {
               realEstate.setDescription(RealEstateSearchType, it_field->second);
            }

            if ((it_field = iter->find(ANNOUNCE_IMAGE)) != iter->end())
            {
               realEstate.setDescription(ANNOUNCE_IMAGE, it_field->second);
            }
            std::string announceSource;
            if ((it_field = iter->find(ANNOUNCE_SOURCE)) != iter->end())
            {
               announceSource = it_field->second;
               realEstate.setDescription(ANNOUNCE_SOURCE, announceSource);
            }

            if ((it_field = iter->find(TIMESTAMP)) != iter->end())
            {
               realEstate.setDescription(TIMESTAMP, it_field->second);
            }
            if ((it_field = iter->find(SIMILAR_ANNOUNCE)) != iter->end())
            {

               realEstate.setDescription(SIMILAR_ANNOUNCE, it_field->second);

            }
            if ((it_field = iter->find(ANNOUNCE_TITLE)) != iter->end())
            {

               realEstate.setDescription(ANNOUNCE_TITLE, it_field->second);

            }
            if ((it_field = iter->find(PROPERTY_DESCRIPTION)) != iter->end())
            {

               realEstate.setDescription(PROPERTY_DESCRIPTION, it_field->second);

            }
            if ((it_field = iter->find(IMAGE_COUNT))!= iter->end())
            {

               realEstate.setDescription(IMAGE_COUNT, it_field->second);

            }

            announcesByID[id] = realEstate;
         }
         scanend = scanReturn.scanend;
      } while (!scanend);

   }

   void putTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, const std::unordered_map<int64_t, classifier::RealEstateAd>& announcesByID)
   {
      for (auto it = announcesByID.begin(); it != announcesByID.end(); ++it)
      {
         std::map<std::string, ValueType> valuesToPut;
         int64_t id = it->first;
         ValueType IDValue;
         IDValue.field = std::to_string(id);
         IDValue.fieldtype = Type::type::NUMBER;
         valuesToPut[ID] = IDValue;

         const auto& descriptions = it->second.GetAllDescriptions();
         for (auto iter = descriptions.begin(); iter != descriptions.end(); ++iter)
         {
            const std::string& fieldName = iter->first;
            const std::string& fieldValue = iter->second;
            if (!fieldValue.empty())
            {
               ValueType value = BuildValueType(fieldName, iter->second);
               valuesToPut[fieldName] = value;
            }
         }
         if (valuesToPut.empty())
         {
            Log::getInstance()->error("ValuesToPut is empty for item " + std::to_string(id));
         }

         OperationResult result;
         client->put(result, tableName, valuesToPut);
         if (!result.success)
         {
            Log::getInstance()->error("Failed to put " + std::to_string(id) + " into table " + tableName + " error : " + result.error);
         }

      }
   }
} //end surfyn namespace


int main(int argc, char* argv[])
{
   std::unordered_map<int64_t, surfyn::classifier::RealEstateAd> announces;

   Log::Init("dyndb_copy");
   Log::getInstance()->info("Starting dyndb_copy ...");
   shared_ptr<TTransport> socket(new TSocket("localhost", port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client = std::make_shared<dynamodb_accessClient>(protocol);

   transport->open();
   std::string table_to_copy = "FR_PROPERTIES";

   if(argc == 2)
   {
      surfyn::utils::Config dyndb_copy_conf(argv[1]);
      dyndb_copy_conf.loadconfig();
      table_to_copy = dyndb_copy_conf.getStringValue("tablename");
   }

   surfyn::readTable(client,table_to_copy,announces);
   std::string new_table = table_to_copy+"_COPY";
   OperationResult result;
   KeyValue key;
   key.key = "ID";
   key.value.fieldtype = Type::type::NUMBER;

   client->createTable(result, new_table, key , std::map<std::string, std::string>());

   if(!result.success)
   {
      Log::getInstance()->error("Failed to create table " + new_table + " error: [" + result.error + "]");
      Log::getInstance()->error("no table copy performed");
      return 1;
   }
   surfyn::putTable(client, new_table, announces);
   Log::getInstance()->info("Table " + table_to_copy + " successfully copy to " + new_table);
   return 0;
}
