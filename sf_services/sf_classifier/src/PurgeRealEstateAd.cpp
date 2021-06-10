/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "PurgeRealEstateAd.h"
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include "sf_services/sf_utils/inc/Algorithm.h"
#include "sf_services/sf_utils/inc/Logger.h"
#include <sstream>

#ifdef _WIN32
#include <strptime.h>
#define timegm _mkgmtime
#endif




namespace surfyn
{
namespace classifier
{

const std::string id_field = "ID";
const std::string timestamp_field = "TIMESTAMP";
const std::string adstatus_field = "AD_STATUS";


using Log = surfyn::utils::Logger;

void purgeRealEstateAd(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tablename, const std::string& city, int maxDelayBeforePurgeSec, int maxDelayBeforePurgeSecForSurfynAd)
{
   Log::getInstance()->info("Purge old Ad");

   std::vector<std::string> elementKeyToDelete;
   std::map<std::string, ValueType> attributestoget;

   ValueType value;
   value.field = "";
   value.fieldtype = Type::type::NUMBER;
   attributestoget[id_field] = value;

   value.fieldtype = Type::type::STRING;
   attributestoget[timestamp_field] = value;
   value.fieldtype = Type::type::STRING;
   attributestoget[adstatus_field] = value;

   bool scanend = false;

   std::string filterexpression = "CITY = :ct";
   std::map<std::string, ValueType> expressionValue;
   ValueType city_value;
   city_value.field = city;
   city_value.fieldtype = Type::type::STRING;
   expressionValue[":ct"] = city_value;


   // finding outdate keys
   do
   {
      ScanReqResult scanReturn;

      client->scan(scanReturn, tablename, attributestoget, filterexpression, expressionValue);

      if (!scanReturn.result.success)
      {
        Log::getInstance()->error("Failed to scan into table " + tablename + " error : " + scanReturn.result.error);
      }
      time_t current_time = time(nullptr);

      std::stringstream logstream;
      logstream << "Purge: " << scanReturn.values.size() << " elements scan\n";

      Log::getInstance()->info(logstream.str());

      for(auto iter = scanReturn.values.begin(); iter != scanReturn.values.end();++iter)
      {
         const auto it_timestamp = iter->find(timestamp_field);

         if( it_timestamp != iter->end())
         {
            struct tm last;
            strptime(it_timestamp->second.c_str(), "%Y-%m-%dT%H:%M:%S", &last);
            time_t last_modification_time = timegm(&last);

            int maxDelay;
            
            auto it_adstatus =  iter->find(adstatus_field);
            if( it_adstatus != iter->end())
            {
               if( it_adstatus->second  == "OFF")
               {
                  const auto it_id = iter->find(id_field);

                  if( it_id != iter->end())
                  {
                     elementKeyToDelete.emplace_back(it_id->second);
                  }
                  continue;
               }
               maxDelay = maxDelayBeforePurgeSecForSurfynAd;
            }
            else 
            {
               maxDelay = maxDelayBeforePurgeSec;
            }


            if((current_time - last_modification_time) >= maxDelay)
            {
               const auto it_id = iter->find(id_field);

               if( it_id != iter->end())
               {
                  elementKeyToDelete.emplace_back(it_id->second);
               }
            }
         }
      }
      scanend = scanReturn.scanend;
   }while(!scanend);

   // remove keys found
   OperationResult removeReturn;
   for( auto key : elementKeyToDelete)
   {
      KeyValue keyToRemove;
      keyToRemove.value.fieldtype = Type::type::NUMBER;
      keyToRemove.value.field = key;
      keyToRemove.key = "ID";

      client->remove(removeReturn, tablename, keyToRemove);
      // TODO manage bandwith limit
      // work-around
      std::this_thread::sleep_for(std::chrono::milliseconds(50));

   }

   Log::getInstance()->info("Purge Finished!");
}

}
}

