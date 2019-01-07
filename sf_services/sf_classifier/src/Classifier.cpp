/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <iostream>
#include "RealEstateAdClassifier.h"
#include "Logger.h"
#include "PurgeRealEstateAd.h"


#include <sstream>

#include "Criteria.h"
#include "RealEstateAd.h"
#include "rapidjson/document.h"
#include <locale>

using Log = surfyn::utils::Logger;

namespace surfyn
{
namespace classifier
{
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
   std::locale::global(std::locale{ "" });

   std::vector<RealEstateAd> announces;
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

   /*attributestoget[RealEstatePrice] = value;
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
   attributestoget[RealEstateParking] = value;*/

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
         /*for (auto it = iter->begin(); it != iter->end(); ++it)
         {
            Log::getInstance()->info(it->first + " : " + it->second);
         }*/

         int64_t id = atol((*iter)[RealEstateKey].c_str());
         RealEstateAd prop = RealEstateAd(id);

         std::map<std::string, std::string>::const_iterator it_field;
         if ((it_field = iter->find(RealEstateCity)) != iter->end())
         {
            prop.setDescription(it_field->first, it_field->second);
         }
         if ((it_field = iter->find(RealEstateType)) != iter->end())
         {
            prop.setDescription(it_field->first, it_field->second);
         }
         if ((it_field = iter->find(PROPERTY_DESCRIPTION)) != iter->end())
         {
            using namespace rapidjson;

            Document document;
            document.Parse(it_field->second.c_str());
            if (document.HasMember("infos_acquereur"))
            {
               const Value& info_acquereur = document["infos_acquereur"];
               if (info_acquereur.HasMember("prix"))
               {
                  const Value& prix = info_acquereur["prix"];
                  if (prix.HasMember("prix"))
                  {
                     double price = prix["prix"].GetDouble();
                     prop.setDescription(RealEstatePrice, std::to_string(price));
                  }
               }
            }

            if (document.HasMember("categories"))
            {
               const Value& categories = document["categories"];

               for (SizeType i = 0; i < categories.Size(); i++)
               {
                  const Value& category = categories[i];
                  if (category.HasMember("id") && category.HasMember("criteria"))
                  {
                     const int id = category["id"].GetInt();
                     const Value& criteria = category["criteria"];
                     if (1 == id)
                     {
                        for (SizeType j = 0; j < criteria.Size(); ++j)
                        {
                           const Value& object = criteria[j];
                           if (object.HasMember("order") && object.HasMember("value"))
                           {
                              const int order = object["order"].GetInt();
                              if (order == 2430)
                              {
                                 prop.setDescription(RealEstateCellars, "1");
                              }
                           }
                        }
                     }
                     else if (2 == id)
                     {
                        char surface[20];
                        char floor[10];
                        int bedroomNb = 0, pieceNb = 0, constructionYear = 1970, buildingTotalFloor = 1;
                        for (SizeType j = 0; j < criteria.Size(); j++)
                        {
                           const Value& object = criteria[j];
                           if (object.HasMember("order") && object.HasMember("value"))
                           {
                              const int order = object["order"].GetInt();
                              std::string value = object["value"].GetString();
                              if (2090 == order)
                              {
                                 sscanf(value.c_str(), "Surface de %s m²", surface);
                              }
                              else if (2092 == order)
                              {
                                 sscanf(value.c_str(), "Année de construction %d", &constructionYear);
                              }
                              else if (2094 == order)
                              {
                                 sscanf(value.c_str(), "Bâtiment de %d étage", &buildingTotalFloor);
                              }
                              else if (2096 == order)
                              {
                                 sscanf(value.c_str(), "Au %s étage", floor);
                              }
                              else if (2140 == order)
                              {
                                 sscanf(value.c_str(), "%d Pièces", &pieceNb);
                              }
                              else if (2165 == order)
                              {
                                 sscanf(value.c_str(), "%d Chambre", &bedroomNb);
                              }
                           }
                        }

                        prop.setDescription(RealEstateSurface, surface);
                        prop.setDescription(RealEstateConstructionYear, std::to_string(constructionYear));
                        prop.setDescription(RealEstateBeds, std::to_string(bedroomNb));
                        prop.setDescription(RealEstateRooms, std::to_string(pieceNb));
                        prop.setDescription(RealEstateFloor, floor);
                          
                     }
                     else if (3 == id)
                     {
                        for (SizeType j = 0; j < criteria.Size(); ++j)
                        {
                           const Value& object = criteria[j];
                           if (object.HasMember("order") && object.HasMember("value"))
                           {
                              const int order = object["order"].GetInt();
                              const std::string value = object["value"].GetString();
                              if (order == 2540)
                              {
                                 prop.setDescription(RealEstateTypeOfHeating, value);
                              }
                           }
                        }
                     }
                     else if (4 == id)
                     {
                        for (SizeType j = 0; j < criteria.Size(); ++j)
                        {
                           const Value& object = criteria[j];
                           if (object.HasMember("order") && object.HasMember("value"))
                           {
                              const int order = object["order"].GetInt();
                              const std::string value = object["value"].GetString();
                              if (order == 2430)
                              {
                                 auto index = value.find_first_of(' ');
                                 if (index != std::string::npos)
                                 {
                                    prop.setDescription(RealEstateParking, value.substr(0, index));
                                 }
                              }
                           }
                        }
                     }
                     else
                     {

                     }
                  }
               }
                 
            }
         }

         //TODO to add more fields
         if ((it_field = iter->find(RealEstateTimeToPublicTransport)) != iter->end())
         {
            prop.setDescription(it_field->first, it_field->second);
         }

         announces.emplace_back(prop);
      }
      scanend = scanReturn.scanend;
   }while(!scanend);


   Log::getInstance()->info("Read table contents finished");

   // 2. process similar announces
   RealEstateAdClassifier Classifier;
   //Classifier.trainClassifier();

   int sameAnnoucePairNb = 0;
   for(auto iter = announces.begin(); iter != announces.end(); ++iter)
   {
      for(auto it = std::next(iter, 1); it != announces.end(); ++it)
      {
         std::stringstream ss;
         ss << "Comparing " << iter->getId() << " and " << it->getId();
         Log::getInstance()->info(ss.str());

         if( Classifier.isSame(*iter, *it) )
         {
            std::stringstream ss;
            ss << iter->getId() << " and " << it->getId() << " are the same";
            Log::getInstance()->info(ss.str());
            sameAnnoucePairNb++;
            //updateDataBaseWithSimilarAd(client, tablename, *iter, *it);
         }
      }
   }
   sameAnnoucePairNb /= 2;
   Log::getInstance()->info("Same announce pair is " + std::to_string(sameAnnoucePairNb));
}


}
}
