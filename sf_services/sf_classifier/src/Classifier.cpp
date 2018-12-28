/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <iostream>
#include "RealEstateAdClassifier.h"
#include "surfyng/sf_services/sf_utils/inc/Logger.h"
#include "PurgeRealEstateAd.h"


#include <sstream>

#include "Criteria.h"
#include "RealEstateAd.h"

using Log = surfyn::utils::Logger;

namespace surfyn
{
namespace classifier
{

static void updateDataBaseWithSimilarAd(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tablename,
                                 const RealEstateAd& prop1, const RealEstateAd& prop2)
{
   OperationResult result;
   KeyValue element_key;
   element_key.key = RealEstateKey;

   std::stringstream idstream1;
   idstream1 << prop1.getId();

   element_key.value.field = idstream1.str();
   element_key.value.fieldtype = Type::type::NUMBER;

   std::map<std::string, ValueType> fields;
   ValueType value;
   std::stringstream idstream2;
   idstream2 << prop2.getId();

   value.field = idstream2.str();
   value.fieldtype = Type::type::NUMBER;

   fields[RealEstateSimilarAd] = value;

   client->update(result, tablename, element_key, fields);

   element_key.value.field = idstream2.str();
   element_key.value.fieldtype = Type::type::NUMBER;
   value.field = idstream1.str();

   fields[RealEstateSimilarAd] = value;

   client->update(result, tablename, element_key, fields);
}

void detectSimilarRealEstateAd(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tablename)
{
   std::vector<RealEstateAd> announces;
   std::map<std::string, ValueType> attributestoget;

   ValueType value;
   value.field = "";
   value.fieldtype = Type::type::NUMBER;
   attributestoget[RealEstateKey] = value;

   value.fieldtype = Type::type::STRING;

   attributestoget[RealEstatePrice] = value;
   attributestoget[RealEstateSurface] = value;
   attributestoget[RealEstateConstructionYear] = value;
   attributestoget[RealEstateTimeToPublicTransport] = value;
   attributestoget[RealEstateBeds] = value;
   attributestoget[RealEstateCity] = value;
   attributestoget[RealEstateType] = value;
   attributestoget[RealEstateRooms] = value;
   attributestoget[RealEstateTypeOfHeating] = value;
   attributestoget[RealEstateCellars] = value;
   attributestoget[RealEstateFloor] = value;
   attributestoget[RealEstateParking] = value;

   // 1. find out announces
   bool scanend = false;
   do
     {
        ScanReqResult scanReturn;
        client->scan(scanReturn, tablename, attributestoget, "");


        std::stringstream logstream;
        logstream << "Classifier: " << scanReturn.values.size() << " elements scan\n";

        Log::getInstance()->info(logstream.str());

        for(auto iter = scanReturn.values.begin(); iter != scanReturn.values.end();++iter)
        {
           int64_t id = atol((*iter)[RealEstateKey].c_str());
           RealEstateAd prop = RealEstateAd(id);

           std::map<std::string, std::string>::const_iterator it_field;
           if( (it_field = iter->find(RealEstatePrice)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }

           if( (it_field = iter->find(RealEstateSurface)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateConstructionYear)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateTimeToPublicTransport)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateBeds)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateCity)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateType)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateRooms)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateTypeOfHeating)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateCellars)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateFloor)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }
           if( (it_field = iter->find(RealEstateParking)) != iter->end() )
           {
               prop.setDescription(it_field->first, it_field->second);
           }

           announces.emplace_back(prop);
        }
        scanend = scanReturn.scanend;
     }while(!scanend);



   // 2. process similar announces
   RealEstateAdClassifier Classifier;
   Classifier.trainClassifier();

   for(auto iter = announces.begin(); iter != announces.end(); ++iter)
   {
      for(auto it = std::next(iter, 1); it != announces.end(); ++it)
      {
         if( Classifier.isSame(*iter, *it) )
         {
            updateDataBaseWithSimilarAd(client, tablename, *iter, *it);
         }
      }
   }
}


}
}
