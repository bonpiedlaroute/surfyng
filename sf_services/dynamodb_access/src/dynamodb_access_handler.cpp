/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <dynamodb_access_handler.h>
#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/utils/memory/AWSMemory.h>
#include <aws/core/utils/UnreferencedParam.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSSet.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/ratelimiter/DefaultRateLimiter.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/memory/AWSMemory.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/DeleteTableRequest.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/UpdateTableRequest.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>
#include "sf_services/sf_utils/inc/Config.h"
#include "sf_services/sf_utils/inc/Str.h"
#include "sf_services/sf_utils/inc/Logger.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <aws/dynamodb/model/ScanRequest.h>
#include <thread>


#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <memory>
#include <sstream>

using namespace Aws::Http;
using namespace Aws::Client;
using namespace Aws::DynamoDB;
using namespace Aws::DynamoDB::Model;


using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using Log = surfyn::utils::Logger;

namespace ddba
{

void dynamodb_accessHandler::Init()
{
   Aws::SDKOptions options;
   Aws::InitAPI(options);
}

dynamodb_accessHandler::dynamodb_accessHandler()
{

   surfyn::utils::Config ddb_conf("dynamodb_access.ini");
   ddb_conf.loadconfig();
   std::string allocation_tag = ddb_conf.getStringValue("allocation_tag").c_str();
   std::shared_ptr<Aws::Utils::RateLimits::RateLimiterInterface> limiter = Aws::MakeShared<Aws::Utils::RateLimits::DefaultRateLimiter<>>(allocation_tag.c_str(), 200000);

   Log::getInstance()->info(" Starting dynamodb Access ");

   ClientConfiguration config;
   config.endpointOverride = ddb_conf.getStringValue("endpoint_override").c_str();
   config.scheme = static_cast<Aws::Http::Scheme>(ddb_conf.getIntValue("http_scheme"));
   config.connectTimeoutMs = ddb_conf.getLongValue("connection_timeout_ms");
   config.requestTimeoutMs = ddb_conf.getLongValue("request_timeout_ms");
   config.readRateLimiter = limiter;
   config.writeRateLimiter = limiter;
   config.httpLibOverride = Aws::Http::TransferLibType::DEFAULT_CLIENT;
   config.executor = Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>(allocation_tag.c_str(), 4);
   config.region = ddb_conf.getStringValue("region").c_str();

   m_client = Aws::MakeShared<DynamoDBClient>(allocation_tag.c_str(), config);

   Log::getInstance()->info(" dynamodb access Started ");
}



void dynamodb_accessHandler::put(OperationResult& _return, const std::string& tablename, const std::map<std::string, ValueType> & values)
{

   PutItemRequest putItemRequest;

   putItemRequest.SetTableName(tablename.c_str());


   for(auto iter = values.begin(); iter != values.end(); ++iter)
   {
      AttributeValue keyAttribute;
      switch(iter->second.fieldtype)
      {
         case Type::NUMBER:
         {
            keyAttribute.SetN(iter->second.field.c_str());
            break;
         }
         default:
         {
            keyAttribute.SetS(iter->second.field.c_str());
            break;
         }
      }

      putItemRequest.AddItem(iter->first.c_str(), keyAttribute);

   }

   PutItemOutcome putOutcome = m_client->PutItem(putItemRequest);

   Log::getInstance()->info(" put sent");

   fillResult(_return, putOutcome);

}

void dynamodb_accessHandler::setKeyAttributeValue(Aws::DynamoDB::Model::AttributeValue& Key, const KeyValue& key) const
{
   switch(key.value.fieldtype)
   {
      case Type::type::NUMBER:
      {
         Key.SetN(key.value.field.c_str());
         break;
      }
      default:
      {
         Key.SetS(key.value.field.c_str());
         break;
      }
   }
}

Type::type dynamodb_accessHandler::findAttrType(const std::string& attr, const KeyValue& key, const std::map<std::string, ValueType>& attributestoget) const
{
   if( attr == key.key )
   {
      return key.value.fieldtype;
   }
   else
   {
      return findAttrType(attr, attributestoget);
   }
}
Type::type dynamodb_accessHandler::findAttrType(const std::string& attr, const std::map<std::string, ValueType>& attributestoget) const
{
   auto iter = attributestoget.find(attr);

   if( iter != attributestoget.end() )
   {
      return iter->second.fieldtype;
   }
   else
   {
      return Type::type::STRING;
   }
}
void dynamodb_accessHandler::get(GetResult& _return, const std::string& tablename, const KeyValue& key, const std::map<std::string, ValueType> & attributestoget)
{
   GetItemRequest getItemRequest;

   getItemRequest.SetTableName(tablename.c_str());

   AttributeValue Key;
   setKeyAttributeValue(Key, key);

   getItemRequest.AddKey(key.key.c_str(), Key);

   Aws::Vector<Aws::String> attributesToGet;

   attributesToGet.push_back(key.key.c_str());

   for( auto value: attributestoget)
   {
      attributesToGet.push_back(value.first.c_str());
   }


   GetItemOutcome getOutcome = m_client->GetItem(getItemRequest);

   Log::getInstance()->info(" get sent");

   fillResult(_return.result, getOutcome);


   if( getOutcome.IsSuccess() )
   {

      Aws::Map<Aws::String, AttributeValue> itemCollection = getOutcome.GetResult().GetItem();

      for(Aws::Map<Aws::String, AttributeValue>::const_iterator iter = itemCollection.begin(); iter != itemCollection.end(); ++iter)
      {
         Type::type attrType = findAttrType(iter->first.c_str(), key, attributestoget);

         switch(attrType)
         {
            case Type::type::NUMBER:
            {
               _return.values[iter->first.c_str()] = iter->second.GetN();
               break;
            }
            default:
            {
               _return.values[iter->first.c_str()] = iter->second.GetS();
               break;
            }
         }

      }
   }
   else
   {
      std::stringstream ss;
      ss << " failed to get item "<< key.key << " in table " << tablename << ", reason[" << getOutcome.GetError().GetMessage() << "]";
      Log::getInstance()->error(ss.str());
   }
}

void dynamodb_accessHandler::remove(OperationResult& _return, const std::string& tablename, const KeyValue& key)
{
   DeleteItemRequest deleteItemRequest;

   AttributeValue Key;

   setKeyAttributeValue(Key, key);

   deleteItemRequest.AddKey(key.key.c_str(), Key);

   deleteItemRequest.SetTableName(tablename.c_str());
   deleteItemRequest.SetReturnValues(ReturnValue::NONE);



   DeleteItemOutcome deleteItemOutcome = m_client->DeleteItem(deleteItemRequest);

   Log::getInstance()->info(" delete sent");

   fillResult(_return, deleteItemOutcome);
}

void dynamodb_accessHandler::update(OperationResult& _return, const std::string& tablename, const KeyValue& key, const std::map<std::string, ValueType> & values)
{
   UpdateItemRequest updateItemRequest;
   updateItemRequest.SetTableName(tablename.c_str());

   AttributeValue Key;
   setKeyAttributeValue(Key, key);

   updateItemRequest.AddKey(key.key.c_str(), Key);

   for(auto iter = values.begin(); iter != values.end(); ++iter)
   {
      AttributeValueUpdate attValueUpdate;
      attValueUpdate.SetAction(AttributeAction::PUT);
      AttributeValue attValue;

      switch(iter->second.fieldtype)
      {
         case Type::NUMBER:
         {
            attValue.SetN(iter->second.field.c_str());
            break;
         }
         default:
         {
            attValue.SetS(iter->second.field.c_str());
            break;
         }
      }
      attValueUpdate.SetValue(attValue);
      updateItemRequest.AddAttributeUpdates(iter->first.c_str(), attValueUpdate);
   }


   UpdateItemOutcome updateItemOutcome = m_client->UpdateItem(updateItemRequest);

   Log::getInstance()->info(" update sent");

   fillResult(_return, updateItemOutcome);
}

void dynamodb_accessHandler::createTable(OperationResult& _return, const std::string& tablename, const KeyValue& key, const std::map<std::string, std::string> & properties)
{
   CreateTableRequest createTableRequest;

   AttributeDefinition Key;
   Key.SetAttributeName(key.key.c_str());

   switch(key.value.fieldtype)
   {
      case Type::type::NUMBER:
      {
         Key.SetAttributeType(ScalarAttributeType::N);
         break;
      }
      default:
      {
         Key.SetAttributeType(ScalarAttributeType::S);
         break;
      }
   }


   createTableRequest.AddAttributeDefinitions(Key);
   KeySchemaElement hashKeySchemaElement;
   hashKeySchemaElement.WithAttributeName(key.key.c_str()).WithKeyType(KeyType::HASH);

   createTableRequest.AddKeySchema(hashKeySchemaElement);
   ProvisionedThroughput provisionedThroughput;
   provisionedThroughput.SetReadCapacityUnits(5);
   provisionedThroughput.SetWriteCapacityUnits(5);
   createTableRequest.WithProvisionedThroughput(provisionedThroughput);
   createTableRequest.WithTableName(tablename.c_str());

   CreateTableOutcome createTableOutcome = m_client->CreateTable(createTableRequest);

   Log::getInstance()->info(" createtable sent");

   fillResult(_return, createTableOutcome);

}

void dynamodb_accessHandler::deleteTable(OperationResult& _return, const std::string& tablename)
{
   DeleteTableRequest deleteTableRequest;
   deleteTableRequest.SetTableName(tablename.c_str());

   DeleteTableOutcome deleteTableOutcome = m_client->DeleteTable(deleteTableRequest);

   Log::getInstance()->info(" deletetable sent");

   fillResult(_return, deleteTableOutcome);
}

void dynamodb_accessHandler::scan(ScanReqResult& _return, const std::string& tablename, const std::map<std::string, ValueType> & attributestoget, const std::string& filterexpression, const std::map<std::string, ValueType> & exprValues)
{
   // TODO manage the case of concurrent access
   // in case of concurrent access to this function m_lastEvaluatedKey could be wrong
   ScanRequest scanRequest;

   scanRequest.WithTableName(tablename.c_str());

   if(!filterexpression.empty())
   {
      scanRequest.SetFilterExpression(filterexpression.c_str());
      Aws::Map<Aws::String, AttributeValue> awsExprValues;
      for( auto value : exprValues)
      {
         AttributeValue attrValue;
         switch(value.second.fieldtype)
         {
            case Type::NUMBER:
            {
               attrValue.SetN(value.second.field.c_str());
               break;
            }
            default:
            {
               attrValue.SetS(value.second.field.c_str());
               break;
            }
         }
         awsExprValues[value.first.c_str()] = attrValue;
      }

      scanRequest.SetExpressionAttributeValues(awsExprValues);

      Aws::String projectionExpression = "";
      for( auto iter_att = attributestoget.begin(); iter_att != attributestoget.end(); ++iter_att )
      {
         if( iter_att != attributestoget.begin())
         projectionExpression += ',';

         projectionExpression += iter_att->first.c_str();
      }

      scanRequest.SetProjectionExpression(projectionExpression);
   }
   else
   {
      Aws::Vector<Aws::String> attToget;

      for( auto att : attributestoget )
      {
         attToget.push_back(att.first.c_str());
      }

      scanRequest.SetAttributesToGet(attToget);

   }

   if(!m_lastEvaluatedKey.empty())
      scanRequest.WithExclusiveStartKey(m_lastEvaluatedKey);

   ScanOutcome scanOutcome = m_client->Scan(scanRequest);

   Log::getInstance()->info(" scan sent");

   fillResult(_return.result, scanOutcome);

   if(scanOutcome.IsSuccess())
   {
         Aws::Vector<Aws::Map<Aws::String, AttributeValue>> itemsCollection = scanOutcome.GetResult().GetItems();

         std::stringstream msg;
         msg << " scan success: ";
         msg << itemsCollection.size();
         msg << " items received ";
         Log::getInstance()->info(msg.str());

         for(auto item: itemsCollection )
         {
            std::map<std::string, std::string> attributes;
            for(Aws::Map<Aws::String, AttributeValue>::const_iterator iter = item.begin(); iter != item.end(); ++iter)
            {
               Type::type attrType = findAttrType(iter->first.c_str(), attributestoget);

               switch(attrType)
               {
                  case Type::type::NUMBER:
                  {
                     /*std::stringstream msg;
                     msg << " inserting ...";
                     msg << iter->first.c_str();
                     msg << ":";
                     msg << iter->second.GetN();

                     Log::getInstance()->info(msg.str());*/
                     attributes[iter->first.c_str()] = iter->second.GetN();
                     break;
                  }
                  default:
                  {
                     /*std::stringstream msg;
                     msg << " inserting ...";
                     msg << iter->first.c_str();
                     msg << ":";
                     msg << iter->second.GetS();

                     Log::getInstance()->info(msg.str());*/

                     attributes[iter->first.c_str()] = iter->second.GetS();
                     break;
                  }
               }
            }

            _return.values.push_back(attributes);
         }

   }
   else
   {
      std::stringstream ss;
      ss << " failed to scan table " << tablename << ", reason[" << scanOutcome.GetError().GetMessage() << "]";
      Log::getInstance()->error(ss.str());
   }

   m_lastEvaluatedKey = scanOutcome.GetResult().GetLastEvaluatedKey();
   if( m_lastEvaluatedKey.empty() )
   {
      _return.scanend = true;
   }
   else
   {
      _return.scanend = false;
   }

}

}
