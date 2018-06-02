/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/

#ifndef INC_DYNAMODB_ACCESS_HANDLER_H_
#define INC_DYNAMODB_ACCESS_HANDLER_H_
#include "dynamodb_access.h"
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/core/client/AsyncCallerContext.h>
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <chrono>


namespace ddba
{


class dynamodb_accessHandler : virtual public dynamodb_accessIf
{
public:
   static void Init();
   dynamodb_accessHandler();

   void put(OperationResult& _return, const std::string& tablename, const std::map<std::string, ValueType> & values);

   void get(GetResult& _return, const std::string& tablename, const KeyValue& key, const std::vector<std::string> & attributestoget);

   void scan(ScanReqResult& _return, const std::string& tablename, const std::vector<std::string> & attributestoget, const std::string& filterexpression);

   void remove(OperationResult& _return, const std::string& tablename, const KeyValue& key);

   void update(OperationResult& _return, const std::string& tablename, const KeyValue& key, const std::map<std::string, ValueType> & values);

   void createTable(OperationResult& _return, const std::string& tablename, const KeyValue& key, const std::map<std::string, std::string> & properties);

   void deleteTable(OperationResult& _return, const std::string& tablename);

private:
   void setKeyAttributeValue(Aws::DynamoDB::Model::AttributeValue& Key, const KeyValue& key) const;
   template<class T>
   void fillResult(OperationResult& result, const T& outcome) const
   {
      const Aws::Client::AWSError<Aws::DynamoDB::DynamoDBErrors> error = outcome.GetError();

      result.success = outcome.IsSuccess();
      result.error = error.GetMessage();
   }
   void manageBandwitdhLimit(const std::string& service, const std::chrono::high_resolution_clock::time_point& lasttime, const int64_t& minimum_interval);
private:
   std::shared_ptr<Aws::DynamoDB::DynamoDBClient> m_client;
};

}

#endif /* INC_DYNAMODB_ACCESS_HANDLER_H_ */
