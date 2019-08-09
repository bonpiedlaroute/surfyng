/**
* TODO: This file is created at base of sf_classifier/Classifier.cpp
* Should refactory with it later on
*/


#include "TransformTable.h"

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

#define CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_INT(attribute, filedName) \
   std::string left##attribute##Str = leftAnnounce.getDescription(filedName); \
   std::string right##attribute##Str = rightAnnounce.getDescription(filedName); \
   if (!left##attribute##Str.empty() && !right##attribute##Str.empty()) \
   { \
      int left##attribute##Nb = atoi(left##attribute##Str.c_str()); \
      int right##attribute##Nb = atoi(right##attribute##Str.c_str()); \
      if (left##attribute##Nb != right##attribute##Nb) \
      { \
         logStream << leftAnnounce.getId() << " ##attribute##[" << left##attribute##Nb << "] and " << rightAnnounce.getId() << " ##attribute##[" \
            << right##attribute##Nb << "] are not similar as their ##attribute## differ"; \
         Log::getInstance()->info(logStream.str()); \
         return false; \
      } \
   }

#define CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_STRING(attribute, filedName) \
   std::string left##attribute##Str = leftAnnounce.getDescription(filedName); \
   std::string right##attribute##Str = rightAnnounce.getDescription(filedName); \
   if (!left##attribute##Str.empty() && !right##attribute##Str.empty()) \
   { \
      if (left##attribute##Str != right##attribute##Str) \
      { \
         logStream << leftAnnounce.getId() << " ##attribute##[" << left##attribute##Str << "] and " << rightAnnounce.getId() << " ##attribute##[" \
            << right##attribute##Str << "] are not similar as their ##attribute## differ"; \
         Log::getInstance()->info(logStream.str()); \
         return false; \
      } \
   }

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

   void RemoveElementFromVectorString(std::string& output, std::string toRemove)
   {
      std::vector<std::string> elements;
      boost::split(elements, output, [](char c) { return c == ','; });
      std::set<std::string> tmp;
      for (auto e : elements)
      {
         if (e != toRemove)
         {
            tmp.insert(e);
         }
      }
      output = std::accumulate(std::begin(tmp),
         std::end(tmp),
         std::string{},
         [](const std::string& a, const std::string &b)
      {
         return a.empty() ? b : a + ',' + b;
      });
   }

   ValueType BuildValueType(const std::string& fieldName, const std::string& fieldValue)
   {
      ValueType fieldNameValue;
      if(fieldName == "PRICE" )
      {
         std::string price = fieldValue;
         boost::erase_all(price, " ");
         fieldNameValue.field = price;
         fieldNameValue.fieldtype = Type::type::NUMBER;
      }
      else
      {
         fieldNameValue.field = fieldValue;
         fieldNameValue.fieldtype = Type::type::STRING;
      }

      return fieldNameValue;
   }

   bool IsSimilarAnnounces(const classifier::RealEstateAd& leftAnnounce, const classifier::RealEstateAd& rightAnnounce)
   {
      if( leftAnnounce.getId() == rightAnnounce.getId())
         return false;
      std::stringstream logStream;
      std::string leftPriceStr = leftAnnounce.getDescription(RealEstatePrice);
      std::string rightPriceStr = rightAnnounce.getDescription(RealEstatePrice);
      if (!leftPriceStr.empty() && !rightPriceStr.empty())
      {
         double leftPrice = atof(leftPriceStr.c_str());
         double rightPrice = atof(rightPriceStr.c_str());
         if (abs(leftPrice - rightPrice) / leftPrice > 0.05)
         {
            logStream << leftAnnounce.getId() << " price[" << leftPrice << "] and " << rightAnnounce.getId() << " price[" 
               << rightPrice << "] are not similar as their prices differ";
            Log::getInstance()->info(logStream.str());
            return false;
         }
      }
      std::string leftSurfaceStr = leftAnnounce.getDescription(RealEstateSurface);
      std::string rightSurfaceStr = rightAnnounce.getDescription(RealEstateSurface);
      if (!leftSurfaceStr.empty() && !rightSurfaceStr.empty())
      {
         double leftSurface = atof(leftSurfaceStr.c_str());
         double rightSurface = atof(rightSurfaceStr.c_str());
         if (abs(leftSurface - rightSurface) / leftSurface > 0.05)
         {
            logStream << leftAnnounce.getId() << " surface[" << leftSurface << "] and " << rightAnnounce.getId() << " surface["
               << rightSurface << "] are not similar as their surfaces differ";
            Log::getInstance()->info(logStream.str());
            return false;
         }
      }
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_INT(Room, RealEstateRooms);
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_INT(Bed, RealEstateBeds);
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_INT(Floor, RealEstateFloor);
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_STRING(RealEstateType, RealEstateType); 
      CHECK_SIMILAR_ANNOUNCE_ATTRIBUTE_AS_STRING(RealEstateSearchType, RealEstateSearchType);
      return true;
   }

   void DataFormater::ReadSelogerJSON(const std::string json, classifier::RealEstateAd& realEstate)
   {
      std::locale::global(std::locale(""));

      rapidjson::Document document;
      document.Parse(json.c_str());

      if(document.HasParseError())
      {
         std::stringstream error;
         error << "failed to parse seloger json: error code [";
         error << document.GetParseError();
         error << "] error offset :[";
         error << document.GetErrorOffset();
         error << "]";
         Log::getInstance()->error(error.str());

         return;
      }
      if (document.HasMember("infos_acquereur"))
      {
         const rapidjson::Value& info_acquereur = document["infos_acquereur"];
         if (info_acquereur.HasMember("prix"))
         {
            const rapidjson::Value& prix = info_acquereur["prix"];
            if (prix.HasMember("prix"))
            {
               double price = prix["prix"].GetDouble();
               realEstate.setDescription(RealEstatePrice, std::to_string((int)price));
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
                           realEstate.setDescription(CELLAR, "1");
                        }
                        else if (order == 2410)
                        {
                           realEstate.setDescription(RealEstateLift, "1");
                        }
                        else if (order == 2450)
                        {
                           auto index = value.find_first_of(' ');
                           if (index != std::string::npos)
                           {
                              std::string balcony = value.substr(0, index);
                              realEstate.setDescription(RealEstateLift, balcony);
                           }
                        }
                     }
                  }
               }
               else if (2 == id)
               {
                  std::string surface, floor;
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
                           auto it = value.find_last_of(' ');
                           surface = value.substr(10, it - 10);
                        }
                        else if (2092 == order)
                        {
                           sscanf(value.c_str(), "Année de construction %d", &constructionYear);
                        }
                        else if (2094 == order)
                        {
                           sscanf(value.c_str(), "Bâtiment de %d étages", &buildingTotalFloor);
                        }
                        else if (2096 == order)
                        {
                           auto it = value.find_last_of(' ');
                           floor = value.substr(3, it - 3);
                        }
                        else if (2140 == order)
                        {
                           sscanf(value.c_str(), "%d", &pieceNb);
                        }
                        else if (2165 == order)
                        {
                           sscanf(value.c_str(), "%d", &bedroomNb);
                        }
                     }
                  }

                  realEstate.setDescription(RealEstateSurface, std::string(surface));
                  realEstate.setDescription(RealEstateConstructionYear, std::to_string(constructionYear));
                  realEstate.setDescription(RealEstateBeds, std::to_string(bedroomNb));
                  realEstate.setDescription(RealEstateRooms, std::to_string(pieceNb));
                  realEstate.setDescription(RealEstateFloor, std::string(floor));

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
                           realEstate.setDescription(RealEstateTypeOfHeating, value);
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
                              std::string parking = value.substr(0, index);
                              realEstate.setDescription(RealEstateParking, parking);
                           }
                        }
                        else if (order == 2470)
                        {
                           auto index = value.find_first_of(' ');
                           if (index != std::string::npos)
                           {
                              std::string box = value.substr(0, index);
                              realEstate.setDescription(RealEstateBox, box);
                           }
                        }
                        else if (order == 3250)
                        {
                           float landSurface = 0;
                           sscanf(value.c_str(), "Terrain de %f m2", &landSurface);
                           realEstate.setDescription(RealEstateLandSurface, std::to_string(landSurface));
                        }
                     }
                  }
               }
            }
         }

      }
   }

   void DataFormater::ReadLeboncoinJSON(const std::string json, classifier::RealEstateAd& realEstate)
   {
      std::locale::global(std::locale(""));

      rapidjson::Document document;
      document.Parse(json.c_str());

      if(document.HasParseError())
      {
         std::stringstream error;
         error << "failed to parse leboncoin json: error code [";
         error << document.GetParseError();
         error << "] error offset :[";
         error << document.GetErrorOffset();
         error << "]";
         Log::getInstance()->error(error.str());

         return;
      }

      if (document.HasMember("price"))
      {
         double price = document["price"][0].GetDouble();
         realEstate.setDescription(RealEstatePrice, std::to_string((int)price));
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

         realEstate.setDescription(RealEstateSurface, surface);
         realEstate.setDescription(RealEstateRooms, pieceNb);
      }
      /*if (document.HasMember("images"))
      {
         const rapidjson::Value& images = document["images"];
         if (images.HasMember("nb_images"))
         {
            int imageCount = images["nb_images"].GetInt();
            realEstate.setDescription(IMAGE_COUNT, std::to_string(imageCount));
         }
         if (images.HasMember("urls"))
         {
            std::stringstream ss;
            //bool first = true;
            const rapidjson::Value& imageUrls = images["urls"];
            //for (rapidjson::SizeType j = 0; j < imageUrls.Size(); j++)
            //{
            //   if (!first) ss << ',';
               //ss << "\"" << imageUrls[j].GetString() << "\"";
            ss << imageUrls[0].GetString();

            //}
            realEstate.setDescription(IMAGE, ss.str());
         }
      }*/
   }


   void DataFormater::ReadLogicImmoJSON(const std::string json, classifier::RealEstateAd& realEstate)
   {
      std::locale::global(std::locale(""));

      rapidjson::Document document;
      document.Parse(json.c_str());

      if(document.HasParseError())
      {
         std::stringstream error;
         error << "failed to parse logicimmo json: error code [";
         error << document.GetParseError();
         error << "] error offset :[";
         error << document.GetErrorOffset();
         error << "]";
         Log::getInstance()->error(error.str());

         return;
      }

      if (document.HasMember("price"))
      {
         std::string price = document["price"].GetString();
         auto pos = price.find_last_of(' ');
         price = price.substr(0, pos);
         realEstate.setDescription(RealEstatePrice, price);
      }
      if (document.HasMember("surface"))
      {
         std::string surface = document["surface"].GetString();
         realEstate.setDescription(RealEstateSurface, surface);
      }
      if (document.HasMember("nb_room"))
      {
         std::string nb_room = document["nb_room"].GetString();
         realEstate.setDescription(RealEstateRooms, nb_room);
      }
   }

   void DataFormater::ReadBienIciJSON(const std::string json, classifier::RealEstateAd& realEstate)
   {
      std::locale::global(std::locale(""));

      rapidjson::Document document;
      document.Parse(json.c_str());

      if(document.HasParseError())
      {
         std::stringstream error;
         error << "failed to parse logicimmo json: error code [";
         error << document.GetParseError();
         error << "] error offset :[";
         error << document.GetErrorOffset();
         error << "]";
         Log::getInstance()->error(error.str());

         return;
      }

      if (document.HasMember("price"))
      {
         std::string price = std::to_string(document["price"].GetUint());

         realEstate.setDescription(RealEstatePrice, price);
      }
      if (document.HasMember("surfaceArea"))
      {
         std::string surface = std::to_string(document["surfaceArea"].GetDouble());

         realEstate.setDescription(RealEstateSurface, surface);
      }
      if (document.HasMember("roomsQuantity"))
      {
         std::string nb_room = std::to_string(document["roomsQuantity"].GetUint());

         realEstate.setDescription(RealEstateRooms, nb_room);
      }
   }

void DataFormater::ReadTableAndFormatEntries(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName)
   {
      std::locale::global(std::locale(""));

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
      attributestoget[IMAGE_COUNT] = value;
      attributestoget[ANNOUNCE_IMAGE] = value;

      bool scanend = false;
      do
      {
         ScanReqResult scanReturn;
         client->scan(scanReturn, tableName, attributestoget, "", std::map<std::string, ValueType>());

         std::stringstream logstream;
         logstream << "Adapt model: " << scanReturn.values.size() << " elements scan\n";

         Log::getInstance()->info(logstream.str());

         // TODO find a way to scan a part of entries into memories. Now we import all
         /*std::unordered_map<int64_t, std::map<std::string, std::string>> scanValuesByID;
         for (auto scanValue : scanReturn.values)
         {
            int64_t id = atol(scanValue[RealEstateKey].c_str());
            scanValuesByID[id] = scanValue;
         }*/

         for (auto iter = scanReturn.values.begin(); iter != scanReturn.values.end(); ++iter)
         {
            int64_t id = atol((*iter)[RealEstateKey].c_str());

            classifier::RealEstateAd realEstate(id);
            std::string CurrentImageCount = "", CurrentAnnounceImage="";

            /*std::map<std::string, ValueType> valuesToPut;
            ValueType IDValue;
            IDValue.field = std::to_string(id);
            IDValue.fieldtype = Type::type::NUMBER;
            valuesToPut[ID] = IDValue;*/

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
               //FR_PROPERTIES table store realestatetype in english
               //work-around conversion in french, so that i don't need to relaunch the scrawler, and rerun
               //tensorflow
               //TODO change this
               if(it_field->second == "apartment")
               {
                  realEstate.setDescription(RealEstateType, "Appartement");
               }
               else
               {
                  if(it_field->second == "house")
                  {
                     realEstate.setDescription(RealEstateType, "Maison");
                  }
                  else
                     realEstate.setDescription(RealEstateType, it_field->second);
               }

            }
            if ((it_field = iter->find(RealEstateSearchType)) != iter->end())
            {
               realEstate.setDescription(RealEstateSearchType, it_field->second);
            }
            if ((it_field = iter->find(IMAGE_COUNT)) != iter->end())
            {
               CurrentImageCount = it_field->second;

            }
            if ((it_field = iter->find(ANNOUNCE_IMAGE)) != iter->end())
            {
               CurrentAnnounceImage = it_field->second;
            }
            std::string announceSource;
            if ((it_field = iter->find(ANNOUNCE_SOURCE)) != iter->end())
            {
               announceSource = it_field->second;
               realEstate.setDescription(ANNOUNCE_SOURCE, announceSource);
            }
            if ((it_field = iter->find(PROPERTY_DESCRIPTION)) != iter->end())
            {
               std::string json = it_field->second;
               if (!announceSource.empty())
               {
                  if (announceSource == "seloger")
                  {

                     ReadSelogerJSON(json, realEstate);
                     realEstate.setDescription(SOURCE_LOGO, "data/SL0.svg");
                     realEstate.setDescription(IMAGE_COUNT, CurrentImageCount);
                     realEstate.setDescription(IMAGE, CurrentAnnounceImage);
                  }
                  else if (announceSource == "leboncoin")
                  {
                     ReadLeboncoinJSON(json, realEstate);
                     realEstate.setDescription(SOURCE_LOGO, "data/lbc0.svg");
                     realEstate.setDescription(IMAGE_COUNT, CurrentImageCount);
                     realEstate.setDescription(IMAGE, CurrentAnnounceImage);

                  }else if( announceSource == "logicimmo")
                  {
                     ReadLogicImmoJSON(json, realEstate);
                     realEstate.setDescription(SOURCE_LOGO, "data/li0.svg");
                     realEstate.setDescription(IMAGE_COUNT, CurrentImageCount);
                     realEstate.setDescription(IMAGE, CurrentAnnounceImage);
                  }
                  else if( announceSource == "bienici")
                  {
                     ReadBienIciJSON(json, realEstate);
                     realEstate.setDescription(SOURCE_LOGO, "data/bienici.png");
                     realEstate.setDescription(IMAGE_COUNT, CurrentImageCount);
                     realEstate.setDescription(IMAGE, CurrentAnnounceImage);
                  }
               }
               else
               {
                  ReadSelogerJSON(json, realEstate);
               }
               
            }

            //TODO to add more fields
            /*if ((it_field = iter->find(RealEstateTimeToPublicTransport)) != iter->end())
            {
               ADD_STRING_FIELD_TO_PUT(RealEstateTimeToPublicTransport, it_field->second);
            }*/
            if ((it_field = iter->find(TIMESTAMP)) != iter->end())
            {
               realEstate.setDescription(TIMESTAMP, it_field->second);
            }
            if ((it_field = iter->find(SIMILAR_ANNOUNCE)) != iter->end())
            {
               //std::string similarAnnouces = it_field->second;
               realEstate.setDescription(SIMILAR_ANNOUNCE, it_field->second);
               //ADD_STRING_FIELD_TO_PUT(DUPLICATES, similarAnnouces);

               /*std::vector<std::string> annoucesIDs;
               boost::split(annoucesIDs, similarAnnouces, [](char c) { return c == ','; });
               std::set<std::string> sources;
               for (std::string similarIDStr : annoucesIDs)
               {
                  int64_t otherID = atol(similarIDStr.c_str());
                  std::map<std::string, std::string> otherAnnounceValues = scanValuesByID[otherID];
                  auto it_other = otherAnnounceValues.find(ANNOUNCE_SOURCE);
                  if (it_other != otherAnnounceValues.end())
                  {
                     sources.insert(it_other->second);
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
               }*/
            }


            //ADD_STRING_FIELD_TO_PUT(SOURCE_LOGO, "data/SL0.svg");

            /*OperationResult result;
            client->put(result, destTable, valuesToPut);
            if (!result.success)
            {
               Log::getInstance()->error("Failed to put " + std::to_string(id) + " into table " + destTable + " error : " + result.error);
            }*/
            m_AnnouncesByID[id] = realEstate;
         }
         scanend = scanReturn.scanend;
      } while (!scanend);


      Log::getInstance()->info("Read table contents finished");
   }

   void DataFormater::CheckSimilarAnnounces()
   {
      for (auto it = m_AnnouncesByID.begin(); it != m_AnnouncesByID.end(); ++it)
      {
         classifier::RealEstateAd& realEstate = it->second;
         const std::string& similarAnnouces = realEstate.getDescription(SIMILAR_ANNOUNCE);
         if (!similarAnnouces.empty())
         {
            std::vector<std::string> annoucesIDs;
            boost::split(annoucesIDs, similarAnnouces, [](char c) { return c == ','; });
            std::vector<std::string> sources;
            std::vector<std::string> checkedSimilarIDs;
            sources.push_back(realEstate.getDescription(ANNOUNCE_SOURCE));
            for (std::string similarIDStr : annoucesIDs)
            {
               int64_t otherID = atol(similarIDStr.c_str());
               classifier::RealEstateAd& otherRealEstate = m_AnnouncesByID[otherID];

               if (IsSimilarAnnounces(realEstate, otherRealEstate))
               {
                  std::string otherAnnounceSource = otherRealEstate.getDescription(ANNOUNCE_SOURCE);
                  if (!otherAnnounceSource.empty())
                  {
                     sources.push_back(otherAnnounceSource);
                  }
                  checkedSimilarIDs.push_back(similarIDStr);
               }
               else
               {
                  std::string otherSimilarAnnounces = otherRealEstate.getDescription(SIMILAR_ANNOUNCE);
                  RemoveElementFromVectorString(otherSimilarAnnounces, std::to_string(realEstate.getId()));
                  otherRealEstate.setDescription(SIMILAR_ANNOUNCE, otherSimilarAnnounces);
               }
            }
            if (!checkedSimilarIDs.empty())
            {
               std::string duplicates = std::accumulate(std::begin(checkedSimilarIDs),
                  std::end(checkedSimilarIDs),
                  std::string{},
                  [](const std::string& a, const std::string &b)
               {
                  return a.empty() ? b : a + ',' + b;
               });
               realEstate.setDescription(DUPLICATES, duplicates);
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
               realEstate.setDescription(SOURCES, annouceSources);
            }
         }
         else
         {
            realEstate.setDescription(SOURCES, realEstate.getDescription(ANNOUNCE_SOURCE));
         }
      } // end of for
   } // end of CheckSimilarAnnounces

   void DataFormater::PutTargetTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName)
   {
      for (auto it = m_AnnouncesByID.begin(); it != m_AnnouncesByID.end(); ++it)
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
         // TODO manage bandwith limit
         // work-around
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
   }

}

int main(int argc, char* argv[])
{
   Log::Init("adapt_model");
   Log::getInstance()->info("Starting adapt model ...");
   shared_ptr<TTransport> socket(new TSocket("localhost", port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   auto client = std::make_shared<dynamodb_accessClient>(protocol);

   transport->open();

   surfyn::DataFormater dataFormater;
   dataFormater.ReadTableAndFormatEntries(client, "FR_PROPERTIES");
   dataFormater.CheckSimilarAnnounces();
   dataFormater.PutTargetTable(client, "FR_SUMMARY");

   return 0;
}
