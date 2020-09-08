#include "extracthousingprice.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "sf_services/sf_utils/inc/Logger.h"
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "rapidjson/document.h"
#include "sf_services/sf_utils/inc/Config.h"

#ifdef _WIN32
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using Log = surfyn::utils::Logger;

namespace surfyn
{
   const std::string identifier = "ID";
   const std::string city = "CITY" ;
   const std::string priceBym2Flat = "PRICE_BY_M2_FLAT";
   const std::string priceBym2House = "PRICE_BY_M2_HOUSE";
   const std::string lowPriceBym2Flat = "LOW_PRICE_BY_M2_FLAT";
   const std::string highPriceBym2Flat = "HIGH_PRICE_BY_M2_FLAT";
   const std::string medianPriceBym2Flat = "MEDIAN_PRICE_BY_M2_FLAT";
   const std::string lowPriceBym2House = "LOW_PRICE_BY_M2_HOUSE";
   const std::string highPriceBym2House = "HIGH_PRICE_BY_M2_HOUSE";
   const std::string medianPriceBym2House = "MEDIAN_PRICE_BY_M2_HOUSE";

   HousePriceExtractor::HousePriceExtractor(const std::string& host, int port)
   {
      shared_ptr<TTransport> socket(new TSocket(host, port));
      shared_ptr<TTransport> transport(new TBufferedTransport(socket));
      shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
      m_client = std::make_shared<dynamodb_accessClient>(protocol);

      transport->open();
   }

   void HousePriceExtractor::ReadHousingPrice(const std::string& tablename)
   {
      std::map<std::string, ValueType> attributestoget;
      ValueType value;
      value.field = "";
      value.fieldtype = Type::type::NUMBER;
      attributestoget[identifier] = value;
      value.fieldtype = Type::type::STRING;
      attributestoget[city] = value;
      attributestoget[priceBym2Flat] = value;
      attributestoget[priceBym2House] = value;

      bool scanend = false;
      do
      {
         ScanReqResult scanReturn;

         m_client->scan(scanReturn, tablename, attributestoget, "", {});

         std::stringstream logstream;
         logstream << "HousePriceExtractor: " << scanReturn.values.size() << " elements scan\n";

         Log::getInstance()->info(logstream.str());


         for (auto iter = scanReturn.values.begin(); iter != scanReturn.values.end(); ++iter)
         {
            int64_t id = atol((*iter)[identifier].c_str());

            HousingPriceInfo*  info = new HousingPriceInfo();

            info->id = id;

            std::map<std::string, std::string>::const_iterator it_field;

            if ((it_field = iter->find(city)) != iter->end())
            {
               info->city =  it_field->second;
            }
            if ((it_field = iter->find(priceBym2Flat)) != iter->end())
            {
               info->priceBym2Flat = it_field->second;
            }
            if ((it_field = iter->find(priceBym2House)) != iter->end())
            {
               info->priceBym2House = it_field->second;
            }
            
            m_housingPriceInfos.push_back(info);
         }

         scanend = scanReturn.scanend;
      } while (!scanend);
   }
   ValueType HousePriceExtractor::BuildValueType(const std::string& fieldName, const std::string& fieldValue) const
   {
      ValueType fieldNameValue;
      if(fieldName == city)
      {
         fieldNameValue.field = fieldValue;
         fieldNameValue.fieldtype = Type::type::STRING;
      }
      else
      {
         std::string number_value = fieldValue;
         boost::erase_all(number_value, " ");
         fieldNameValue.field = number_value;
         fieldNameValue.fieldtype = Type::type::NUMBER;
      }

      return fieldNameValue;
   }
   void HousePriceExtractor::updateValues(const std::string& json, const std::string& medianPrice, const std::string& lowPrice, const std::string& highPrice, std::map<std::string, ValueType>& valuesToUpdate)
   {
      Log::getInstance()->info("updateValues");
      std::string fieldValue = "";
      ValueType value;
      rapidjson::Document document;
      document.Parse(json.c_str());

      if(document.HasParseError())
      {
         std::stringstream error;
         error << "failed to parse notaire json: error code [";
         error << document.GetParseError();
         error << "] error offset :[";
         error << document.GetErrorOffset();
         error << "]";
         Log::getInstance()->error(error.str());

         return;
      }

      if(document.Size() > 0 )
      {
         const rapidjson::Value& priceInfo = document[0];

         if( priceInfo.HasMember("prixM2Median"))
         {
            fieldValue = std::to_string(priceInfo["prixM2Median"].GetInt());

            value = BuildValueType(medianPrice, fieldValue);
            valuesToUpdate.insert(std::make_pair(medianPrice, value));
         }
         if( priceInfo.HasMember("quartile1PrixM2"))
         {
            fieldValue = std::to_string(priceInfo["quartile1PrixM2"].GetInt());

            value = BuildValueType(lowPrice, fieldValue);
            valuesToUpdate.insert(std::make_pair(lowPrice, value));
         }
         if( priceInfo.HasMember("quartile3PrixM2"))
         {
            fieldValue = std::to_string(priceInfo["quartile3PrixM2"].GetInt());

            value = BuildValueType(highPrice, fieldValue);
            valuesToUpdate.insert(std::make_pair(highPrice, value));
         }
      }
         

      
   }
   void HousePriceExtractor::UpdateHousingPriceInfos(const std::string& tablename)
   {
      for(auto* info : m_housingPriceInfos)
      {
         std::map<std::string, ValueType> valuesToUpdate;

         std::string fieldValue = info->city;
         ValueType value = BuildValueType(city, fieldValue);
         valuesToUpdate.insert(std::make_pair(city, value));

         std::locale::global(std::locale(""));
         updateValues(info->priceBym2Flat, medianPriceBym2Flat, lowPriceBym2Flat, highPriceBym2Flat, valuesToUpdate);
         updateValues(info->priceBym2House, medianPriceBym2House, lowPriceBym2House, highPriceBym2House, valuesToUpdate);
         
         
         OperationResult result;
         KeyValue Key;
         Key.key = identifier;
         ValueType IDValue;
         IDValue.field = std::to_string(info->id);
         IDValue.fieldtype = Type::type::NUMBER;
         Key.value = IDValue;
         m_client->update(result, tablename, Key, valuesToUpdate);
         if (!result.success)
         {
            Log::getInstance()->error("Failed to update " + std::to_string(info->id) + " into table " + tablename + " error : " + result.error);
         }
         else
         {
            Log::getInstance()->info(" Succesfully update [" + std::to_string(info->id) + "] into table " + tablename );
         }
      }
   }
   HousePriceExtractor::~HousePriceExtractor()
   {
      for(auto* info: m_housingPriceInfos)
      {
         delete info;
      }
   }
} //end namespace surfyn

int main(int argc, char* argv[])
{
   Log::Init("extracthousingprice");
   Log::getInstance()->info("Starting HousePriceExtractor ...");

   std::string input_tablename = "", output_tablename = "";
   std::string host = "";
   int port = 0;

   if(argc == 2)
   {
      surfyn::utils::Config conf(argv[1]);
      conf.loadconfig();
      input_tablename = conf.getStringValue("input_tablename");
      output_tablename = conf.getStringValue("output_tablename");
      host = conf.getStringValue("host");
      port = conf.getIntValue("port");
   }
   else
   {
      Log::getInstance()->error("No config file set! you need to set the host/port/input_tablename/target_tablename in the config file");
      Log::getInstance()->error("No city set! please run [./extracthousingprice config_file]");
      return 1;
   }
 
   surfyn::HousePriceExtractor extractor(host, port) ;
   extractor.ReadHousingPrice(input_tablename);
   extractor.UpdateHousingPriceInfos(output_tablename);
   Log::getInstance()->info("HousePriceExtractor successfully finished!");
   return 0;
}
