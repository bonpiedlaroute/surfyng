/**
* TODO: This file is created at base of sf_classifier/Classifier.cpp
* Should refactory with it later on
*/

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <dynamodb_access.h>
#include <dynamodb_access_types.h>
#include "rapidjson/document.h"
#include <Logger.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <set>

const int port = 5050;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;


#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

using Log = surfyn::utils::Logger;

#define ADD_STRING_FIELD_TO_PUT(fieldName, fieldValue) \
   ValueType fieldName##Value;   \
   fieldName##Value.field = fieldValue;  \
   fieldName##Value.fieldtype = Type::type::STRING;   \
   valuesToPut[fieldName] = fieldName##Value;

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
   const std::string SIMILAR_ANNOUNCE = "SIMILAR_ANNOUNCE";

   const char* RealEstatePrice = "PRICE";
   const char* RealEstateSurface = "SURFACE";
   const char* RealEstateConstructionYear = "CONSTRUCTION_YEAR";
   const char* RealEstateTimeToPublicTransport = "TIME_TO_PUBLICTRANSPORT";
   const char* RealEstateBeds = "BEDS";
   const char* RealEstateCity = "CITY";
   const char* RealEstateType = "PROPERTY_TYPE";
   const char* RealEstateRooms = "ROOMS";
   const char* RealEstateTypeOfHeating = "TYPE_OF_HEATING";
   const char* RealEstateCellars = "CELLARS";
   const char* RealEstateFloor = "FLOOR";
   const char* RealEstateParking = "PARKING";
   const char* RealEstateBox = "BOX";
   const char* RealEstateSearchType = "SEARCH_TYPE";
   const char* RealEstateLandSurface = "LAND_SURFACE";
   const char* RealEstateLift = "LIFT";
   const char* RealEstateBalcony = "BALCONY";
   const char* RealEstateKey = "ID";
   const char* RealEstateSimilarAd = "SIMILAR_AD";

   // Fields for table FR_SUMMARY
   const std::string HISTORY = "HISTORY";
   const std::string IMAGE = "IMAGE";
   const std::string IMAGE_COUNT = "IMAGE_COUNT";
   const std::string NBROOMS = "NB_ROOMS";
   const std::string SOURCES = "SOURCES";
   const std::string LOCATION = "LOCATION";
   const std::string SOURCE_LOGO = "SOURCE_LOGO";
   const std::string TRANSPORT = "TRANSPORT";
   const std::string CELLAR = "CELLAR";
   const std::string DUPLICATES = "DUPLICATES";

   void ProcessSelogerJSON(const std::string json, std::map<std::string, ValueType>& valuesToPut)
   {
      rapidjson::Document document;
      document.Parse(json.c_str());
      if (document.HasMember("infos_acquereur"))
      {
         const rapidjson::Value& info_acquereur = document["infos_acquereur"];
         if (info_acquereur.HasMember("prix"))
         {
            const rapidjson::Value& prix = info_acquereur["prix"];
            if (prix.HasMember("prix"))
            {
               double price = prix["prix"].GetDouble();
               ADD_STRING_FIELD_TO_PUT(RealEstatePrice, std::to_string(price));
            }
         }
      }

      if (document.HasMember("categories"))
      {
         const rapidjson::Value& categories = document["categories"];

         for (rapidjson::SizeType i = 0; i < categories.Size(); i++)
         {
            const rapidjson::Value& category = categories[i];
            if (category.HasMember("id") && category.HasMember("criteria"))
            {
               const int id = category["id"].GetInt();
               const rapidjson::Value& criteria = category["criteria"];
               if (1 == id)
               {
                  for (rapidjson::SizeType j = 0; j < criteria.Size(); ++j)
                  {
                     const rapidjson::Value& object = criteria[j];
                     if (object.HasMember("order") && object.HasMember("value"))
                     {
                        const int order = object["order"].GetInt();
                        std::string value = object["value"].GetString();
                        if (order == 2430)
                        {
                           ADD_STRING_FIELD_TO_PUT(CELLAR, "1");
                        }
                        else if (order == 2410)
                        {
                           ADD_STRING_FIELD_TO_PUT(RealEstateLift, "1");
                        }
                        else if (order == 2450)
                        {
                           auto index = value.find_first_of(' ');
                           if (index != std::string::npos)
                           {
                              ADD_STRING_FIELD_TO_PUT(RealEstateBalcony, value.substr(0, index));
                           }
                        }
                     }
                  }
               }
               else if (2 == id)
               {
                  char surface[20];
                  char floor[10];
                  int bedroomNb = 0, pieceNb = 0, constructionYear = 1970, buildingTotalFloor = 1;
                  for (rapidjson::SizeType j = 0; j < criteria.Size(); j++)
                  {
                     const rapidjson::Value& object = criteria[j];
                     if (object.HasMember("order") && object.HasMember("value"))
                     {
                        const int order = object["order"].GetInt();
                        std::string value = object["value"].GetString();
                        if (2090 == order)
                        {
                           sscanf(value.c_str(), "Surface de %s m2", surface);
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

                  ADD_STRING_FIELD_TO_PUT(RealEstateSurface, std::string(surface));
                  ADD_STRING_FIELD_TO_PUT(RealEstateConstructionYear, std::to_string(constructionYear));
                  ADD_STRING_FIELD_TO_PUT(RealEstateBeds, std::to_string(bedroomNb));
                  ADD_STRING_FIELD_TO_PUT(RealEstateRooms, std::to_string(pieceNb));
                  ADD_STRING_FIELD_TO_PUT(RealEstateFloor, std::string(floor));

               }
               else if (3 == id)
               {
                  for (rapidjson::SizeType j = 0; j < criteria.Size(); ++j)
                  {
                     const rapidjson::Value& object = criteria[j];
                     if (object.HasMember("order") && object.HasMember("value"))
                     {
                        const int order = object["order"].GetInt();
                        const std::string value = object["value"].GetString();
                        if (order == 2540)
                        {
                           ADD_STRING_FIELD_TO_PUT(RealEstateTypeOfHeating, value);
                        }
                     }
                  }
               }
               else if (4 == id)
               {
                  for (rapidjson::SizeType j = 0; j < criteria.Size(); ++j)
                  {
                     const rapidjson::Value& object = criteria[j];
                     if (object.HasMember("order") && object.HasMember("value"))
                     {
                        const int order = object["order"].GetInt();
                        const std::string value = object["value"].GetString();
                        if (order == 2430)
                        {
                           auto index = value.find_first_of(' ');
                           if (index != std::string::npos)
                           {
                              ADD_STRING_FIELD_TO_PUT(RealEstateParking, value.substr(0, index));
                           }
                        }
                        else if (order == 2470)
                        {
                           auto index = value.find_first_of(' ');
                           if (index != std::string::npos)
                           {
                              ADD_STRING_FIELD_TO_PUT(RealEstateBox, value.substr(0, index));
                           }
                        }
                        else if (order == 3250)
                        {
                           float landSurface = 0;
                           sscanf(value.c_str(), "Terrain de %f m2", &landSurface);
                           ADD_STRING_FIELD_TO_PUT(RealEstateLandSurface, std::to_string(landSurface));
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

   void ProcessLeboncoinJSON(const std::string json, std::map<std::string, ValueType>& valuesToPut)
   {
      rapidjson::Document document;
      document.Parse(json.c_str());
      if (document.HasMember("price"))
      {
         double price = document["price"][0].GetDouble();
         ADD_STRING_FIELD_TO_PUT(RealEstatePrice, std::to_string(price));
      }
      if (document.HasMember("attributes"))
      {
         const rapidjson::Value& attributes = document["attributes"];

         std::string surface, pieceNb;
         for (rapidjson::SizeType j = 0; j < attributes.Size(); j++)
         {
            const rapidjson::Value& attribute = attributes[j];
            if (attribute.HasMember("key") && attribute.HasMember("value"))
            {
               const std::string key = attribute["key"].GetString();
               const std::string value = attribute["value"].GetString();
               if ("square" == key)
               {
                  surface = value;
               }
               else if ("rooms" == key)
               {
                  pieceNb = value;
               }
            }
         }

         ADD_STRING_FIELD_TO_PUT(RealEstateSurface, surface);
         ADD_STRING_FIELD_TO_PUT(RealEstateRooms, pieceNb);
      }
      if (document.HasMember("images"))
      {
         const rapidjson::Value& images = document["images"];
         if (images.HasMember("nb_images"))
         {
            int imageCount = images["nb_images"].GetInt();
            ADD_STRING_FIELD_TO_PUT(IMAGE_COUNT, std::to_string(imageCount));
         }
         if (images.HasMember("urls"))
         {
            std::string imageUrls = images["urls"].GetString();
            ADD_STRING_FIELD_TO_PUT(IMAGE, imageUrls);
         }
      }
   }

   void ProcessTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& srcTable, const std::string& destTable)
   {
      std::locale::global(std::locale{ "" });

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

      bool scanend = false;
      do
      {
         ScanReqResult scanReturn;
         client->scan(scanReturn, srcTable, attributestoget, "");

         std::stringstream logstream;
         logstream << "Adapt model: " << scanReturn.values.size() << " elements scan\n";

         Log::getInstance()->info(logstream.str());

         for (auto iter = scanReturn.values.begin(); iter != scanReturn.values.end(); ++iter)
         {
            int64_t id = atol((*iter)[RealEstateKey].c_str());
            std::map<std::string, ValueType> valuesToPut;

            ValueType IDValue;
            IDValue.field = std::to_string(id);
            IDValue.fieldtype = Type::type::NUMBER;
            valuesToPut[ID] = IDValue;

            std::map<std::string, std::string>::const_iterator it_field;
            if ((it_field = iter->find(ANNOUNCE_LINK)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(ANNOUNCE_LINK, it_field->second);
            }
            if ((it_field = iter->find(RealEstateCity)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(RealEstateCity, it_field->second);
            }
            if ((it_field = iter->find(RealEstateType)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(RealEstateType, it_field->second);
            }
            if ((it_field = iter->find(RealEstateSearchType)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(RealEstateSearchType, it_field->second);
            }
            std::string announceSource;
            if ((it_field = iter->find(ANNOUNCE_SOURCE)) != iter->end())
            {
               announceSource = it_field->second;
            }
            if ((it_field = iter->find(PROPERTY_DESCRIPTION)) != iter->end())
            {
               std::string json = it_field->second;
               if (!announceSource.empty())
               {
                  if (announceSource == "seloger")
                  {
                     ProcessSelogerJSON(json, valuesToPut);
                  }
                  else if (announceSource == "leboncoin")
                  {
                     ProcessLeboncoinJSON(json, valuesToPut);
                  }
               }
               else
               {
                  ProcessSelogerJSON(json, valuesToPut);
               }
               
            }

            //TODO to add more fields
            if ((it_field = iter->find(RealEstateTimeToPublicTransport)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(RealEstateTimeToPublicTransport, it_field->second);
            }
            if ((it_field = iter->find(TIMESTAMP)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(TIMESTAMP, it_field->second);
            }
            if ((it_field = iter->find(SIMILAR_ANNOUNCE)) != iter->end())
            {
               std::string similarAnnouces = it_field->second;
               ADD_STRING_FIELD_TO_PUT(DUPLICATES, similarAnnouces);

               std::vector<std::string> annoucesIDs;
               boost::split(annoucesIDs, similarAnnouces, [](char c) { return c == ','; });
               std::set<std::string> sources;
               for (std::string similarID : annoucesIDs)
               {
                  GetResult getResult;
                  KeyValue key;
                  key.key = ID;
                  ValueType keyValue;
                  keyValue.field = similarID;
                  keyValue.fieldtype = Type::type::NUMBER;
                  key.value = keyValue;

                  std::map<std::string, ValueType> attributes;

                  ValueType valueToGet;
                  valueToGet.field = "";
                  valueToGet.fieldtype = Type::type::NUMBER;
                  attributes[ID] = valueToGet;

                  valueToGet.fieldtype = Type::type::STRING;

                  attributes[ANNOUNCE_SOURCE] = valueToGet;

                  client->get(getResult, srcTable, key, attributes);
                  if (getResult.result.success)
                  {
                     auto it_get = getResult.values.find(ANNOUNCE_SOURCE);
                     if (it_get != getResult.values.end())
                     {
                        sources.insert(it_get->second);
                     }
                  }
                  else
                  {
                     Log::getInstance()->error("Failed to get annouce of ID " + similarID);
                  }
               }
               if (!sources.empty())
               {
                  std::string annouceSources = std::accumulate(std::begin(sources),
                     std::end(sources),
                     std::string{},
                     [](const std::string& a, const std::string &b)
                        {
                        return a.empty() ? b : a + ',' + b;
                        });
                  ADD_STRING_FIELD_TO_PUT(SOURCES, annouceSources);
               }
            }
            ADD_STRING_FIELD_TO_PUT(IMAGE, "data/annonce_1.jpg");
            ADD_STRING_FIELD_TO_PUT(SOURCE_LOGO, "data/SL0.svg");

            OperationResult result;
            client->put(result, destTable, valuesToPut);
            if (!result.success)
            {
               Log::getInstance()->error("Failed to put " + std::to_string(id) + " into table " + destTable + " error : " + result.error);
            }
         }
         scanend = scanReturn.scanend;
      } while (!scanend);


      Log::getInstance()->info("Read table contents finished");
   }
}

int main(int argc, char* argv[])
{
   Log::getInstance()->info("Starting adapt model ...");
   shared_ptr<TTransport> socket(new TSocket("localhost", port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client = std::make_shared<dynamodb_accessClient>(protocol);

   transport->open();

   surfyn::ProcessTable(client, "FR_PROPERTIES", "FR_SUMMARY");

   return 0;
}
