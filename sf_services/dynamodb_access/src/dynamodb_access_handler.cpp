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
#include "surfyng/sf_services/sf_utils/inc/Config.h"
#include "surfyng/sf_services/sf_utils/inc/Str.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <memory>

using namespace Aws::Http;
using namespace Aws::Client;
using namespace Aws::DynamoDB;
using namespace Aws::DynamoDB::Model;


using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

namespace ddba
{

void dynamodb_accessHandler::putItemOutcomeReceived(const DynamoDBClient* sender, const PutItemRequest& request, const PutItemOutcome& outcome, const std::shared_ptr<const AsyncCallerContext>& context)
{

   OperationResultAsync result;
   fillResult(result, outcome);

   std::shared_ptr<const DDBAContext> ddba_context = std::dynamic_pointer_cast<const DDBAContext>(context);

   if(ddba_context !=  nullptr)
      m_users[ddba_context->getUserid()]->notifyPutAsync(ddba_context->getJobid(), result);
}

void dynamodb_accessHandler::getItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::GetItemRequest& request,
      const Aws::DynamoDB::Model::GetItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context)
{
   OperationResultAsync result;
   fillResult(result, outcome);

   Aws::Map<Aws::String, AttributeValue> itemCollection = outcome.GetResult().GetItem();
   std::map<std::string, std::string> values;

   for(Aws::Map<Aws::String, AttributeValue>::const_iterator iter = itemCollection.begin(); iter != itemCollection.end(); ++iter)
   {
      values[iter->first.c_str()] = iter->second.GetS();
   }

   std::shared_ptr<const DDBAContext> ddba_context = std::dynamic_pointer_cast<const DDBAContext>(context);

   if(ddba_context !=  nullptr)
      m_users[ddba_context->getUserid()]->notifyGetAsync(ddba_context->getJobid(), values, result);
}

void dynamodb_accessHandler::updateItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::UpdateItemRequest& request,
       const Aws::DynamoDB::Model::UpdateItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context)
{
   OperationResultAsync result;
   fillResult(result, outcome);

   std::shared_ptr<const DDBAContext> ddba_context = std::dynamic_pointer_cast<const DDBAContext>(context);

   if(ddba_context !=  nullptr)
      m_users[ddba_context->getUserid()]->notifyUpdateAsync(ddba_context->getJobid(), result);
}

void dynamodb_accessHandler::deleteItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::DeleteItemRequest& request,
        const Aws::DynamoDB::Model::DeleteItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context)
{

   OperationResultAsync result;
   fillResult(result, outcome);

   std::shared_ptr<const DDBAContext> ddba_context = std::dynamic_pointer_cast<const DDBAContext>(context);

   if(ddba_context !=  nullptr)
      m_users[ddba_context->getUserid()]->notifyDeleteAsync(ddba_context->getJobid(), result);
}

void dynamodb_accessHandler::Init()
{
   Aws::SDKOptions options;
   Aws::InitAPI(options);
}

dynamodb_accessHandler::dynamodb_accessHandler()
{
   m_jobid = 0;

   surfyng::utils::Config ddb_conf("dynamodb_access.ini");
   ddb_conf.loadconfig();
   std::string allocation_tag = ddb_conf.getStringValue("allocation_tag");
   std::shared_ptr<Aws::Utils::RateLimits::RateLimiterInterface> limiter = Aws::MakeShared<Aws::Utils::RateLimits::DefaultRateLimiter<>>(allocation_tag.c_str(), 200000);

   ClientConfiguration config;
   config.endpointOverride = ddb_conf.getStringValue("endpoint_override");
   config.scheme = static_cast<Aws::Http::Scheme>(ddb_conf.getIntValue("http_scheme"));
   config.connectTimeoutMs = ddb_conf.getLongValue("connection_timeout_ms");
   config.requestTimeoutMs = ddb_conf.getLongValue("request_timeout_ms");
   config.readRateLimiter = limiter;
   config.writeRateLimiter = limiter;
   config.httpLibOverride = Aws::Http::TransferLibType::DEFAULT_CLIENT;
   config.executor = Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>(allocation_tag.c_str(), 4);
   config.region = ddb_conf.getStringValue("region");

   m_client = Aws::MakeShared<DynamoDBClient>(allocation_tag.c_str(), config);
}

void dynamodb_accessHandler::loadUserSettings()
{
   std::fstream file;
   file.open("users.dat", std::fstream::in);
   std::string line;

   while( std::getline(file,line) )
   {
      if( line[0] != '#' )
      {
         std::vector<std::string> usersettings;
         surfyng::utils::split(line, ",", usersettings);

         if( usersettings.size() == 3 )
         {
            m_usersettings[atoi(usersettings[0].c_str())] = std::make_pair(usersettings[1], atoi(usersettings[2].c_str()));
         }
      }
   }
}

bool dynamodb_accessHandler::userIdExist(const int64_t& userid) const
{
   return m_usersettings.find(userid) != m_usersettings.end();

}

int64_t dynamodb_accessHandler::putAsync(const int64_t userid, const std::string& tablename, const std::map<std::string, ValueType> & values)
{
   if( !userIdExist(userid) )
   {
      return 0;
   }

   if(! isUserConnectionExist(userid) )
   {
      createUserConnection(userid);
   }

   m_jobid++;
   auto putItemHandler = std::bind(&dynamodb_accessHandler::putItemOutcomeReceived, this, std::placeholders::_1, std::placeholders::_2,
   std::placeholders::_3, std::placeholders::_4);
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

   std::shared_ptr<const AsyncCallerContext> context = std::make_shared<const DDBAContext>(userid, m_jobid);

   m_client->PutItemAsync(putItemRequest, putItemHandler, context);

   return m_jobid;
}

bool dynamodb_accessHandler::isUserConnectionExist(const int64_t userid) const
{
   return m_users.find(userid) != m_users.end();
}

void dynamodb_accessHandler::createUserConnection(const int64_t userid)
{
   boost::shared_ptr<TTransport> socket(new TSocket(m_usersettings[userid].first,m_usersettings[userid].second ));
   boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

   m_users[userid] = std::make_shared<dynamodb_clientClient>(protocol);
   transport->open();
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
int64_t dynamodb_accessHandler::getAsync(const int64_t userid, const std::string& tablename, const KeyValue& key, const std::vector<std::string> & attributestoget)
{
   if( !userIdExist(userid) )
   {
      return 0;
   }

   if(! isUserConnectionExist(userid) )
   {
      createUserConnection(userid);
   }
   m_jobid++;

   auto getItemHandler = std::bind(&dynamodb_accessHandler::getItemOutcomeReceived, this, std::placeholders::_1, std::placeholders::_2,
         std::placeholders::_3, std::placeholders::_4);

   GetItemRequest getItemRequest;

   getItemRequest.SetTableName(tablename.c_str());

   AttributeValue Key;
   setKeyAttributeValue(Key, key);

   getItemRequest.AddKey(key.key.c_str(), Key);

   Aws::Vector<Aws::String> attributesToGet;

   attributesToGet.push_back(key.key.c_str());

   for( auto value: attributestoget)
   {
         attributesToGet.push_back(value.c_str());
   }

   std::shared_ptr<const AsyncCallerContext> context = std::make_shared<const DDBAContext>(userid, m_jobid);

   m_client->GetItemAsync(getItemRequest, getItemHandler, context);

   return m_jobid;
}

int64_t dynamodb_accessHandler::deleteAsync(const int64_t userid, const std::string& tablename, const KeyValue& key)
{
   if( !userIdExist(userid) )
   {
      return 0;
   }

   if(! isUserConnectionExist(userid) )
   {
      createUserConnection(userid);
   }
   m_jobid++;

   auto deleteItemHandler = std::bind(&dynamodb_accessHandler::deleteItemOutcomeReceived, this, std::placeholders::_1, std::placeholders::_2,
   std::placeholders::_3, std::placeholders::_4);

   DeleteItemRequest deleteItemRequest;

   AttributeValue Key;

   setKeyAttributeValue(Key, key);

   deleteItemRequest.AddKey(key.key.c_str(), Key);

   deleteItemRequest.SetTableName(tablename.c_str());
   deleteItemRequest.SetReturnValues(ReturnValue::ALL_OLD);

   m_client->DeleteItemAsync(deleteItemRequest, deleteItemHandler);

   return m_jobid;
}

int64_t dynamodb_accessHandler::updateAsync(const int64_t userid, const std::string& tablename, const KeyValue& key,
                                            const std::map<std::string, ValueType> & values)
{
   if( !userIdExist(userid) )
   {
      return 0;
   }

   if(! isUserConnectionExist(userid) )
   {
      createUserConnection(userid);
   }
   m_jobid++;

   auto updateItemHandler = std::bind(&dynamodb_accessHandler::updateItemOutcomeReceived, this, std::placeholders::_1, std::placeholders::_2,
   std::placeholders::_3, std::placeholders::_4);

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

   std::shared_ptr<const AsyncCallerContext> context = std::make_shared<const DDBAContext>(userid, m_jobid);

   m_client->UpdateItemAsync(updateItemRequest, updateItemHandler, context);

   return m_jobid;
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

   _return.success = createTableOutcome.IsSuccess();
   _return.error = createTableOutcome.GetError().GetMessage();

}

void dynamodb_accessHandler::deleteTable(OperationResult& _return, const std::string& tablename)
{
   DeleteTableRequest deleteTableRequest;
   deleteTableRequest.SetTableName(tablename.c_str());

   DeleteTableOutcome deleteTableOutcome = m_client->DeleteTable(deleteTableRequest);

   _return.success = deleteTableOutcome.IsSuccess();
   _return.error = deleteTableOutcome.GetError().GetMessage();
}


}
