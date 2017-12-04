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
#include "../thrift_generated/dynamodb_client.h"
#include <aws/dynamodb/DynamoDBErrors.h>

namespace ddba
{

class DDBAContext : public Aws::Client::AsyncCallerContext
{
public:
   DDBAContext(int64_t userid, int64_t jobid) :Aws::Client::AsyncCallerContext()
         , m_userid(userid), m_jobid(jobid)
   {

   }
   int64_t getUserid() const
   {
      return m_userid;
   }
   int64_t getJobid() const
   {
      return m_jobid;
   }
private:
   int64_t m_userid;
   int64_t m_jobid;

};

class dynamodb_accessHandler : virtual public dynamodb_accessIf
{
public:
   static void Init();
   dynamodb_accessHandler();

   int64_t putAsync(const int64_t userid, const std::string& tablename, const std::map<std::string, ValueType> & values) override;

   int64_t getAsync(const int64_t userid, const std::string& tablename, const KeyValue& key,
                     const std::vector<std::string> & attributestoget) override;

   int64_t deleteAsync(const int64_t userid, const std::string& tablename, const KeyValue& key) override;

   int64_t updateAsync(const int64_t userid, const std::string& tablename, const KeyValue& key, const std::map<std::string, ValueType> & values) override;

   void createTable(OperationResult& _return, const std::string& tablename, const KeyValue& key, const std::map<std::string, std::string> & properties) override;

   void deleteTable(OperationResult& _return, const std::string& tablename) override;

   void putItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::PutItemRequest& request,
                               const Aws::DynamoDB::Model::PutItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context);
   void getItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::GetItemRequest& request,
                               const Aws::DynamoDB::Model::GetItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context);
   void updateItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::UpdateItemRequest& request,
                                  const Aws::DynamoDB::Model::UpdateItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context);
   void deleteItemOutcomeReceived(const Aws::DynamoDB::DynamoDBClient* sender, const Aws::DynamoDB::Model::DeleteItemRequest& request,
           const Aws::DynamoDB::Model::DeleteItemOutcome& outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context);
protected:
   void loadUserSettings();
   bool userIdExist(const int64_t& userid) const;
   void createUserConnection(const int64_t userid);
   bool isUserConnectionExist(const int64_t userid) const;
private:
   void setKeyAttributeValue(Aws::DynamoDB::Model::AttributeValue& Key, const KeyValue& key) const;
   template<class T>
   void fillResult(OperationResultAsync& result, const T& outcome) const
   {
      const Aws::Client::AWSError<Aws::DynamoDB::DynamoDBErrors> error = outcome.GetError();

      result.success = outcome.IsSuccess();
      result.error = error.GetMessage();
   }
private:
   std::shared_ptr<Aws::DynamoDB::DynamoDBClient> m_client;
   std::unordered_map<int64_t, std::pair<std::string, int>> m_usersettings;
   int64_t                          m_jobid;
   std::unordered_map<int64_t, std::shared_ptr<dynamodb_clientClient>> m_users;
};

}

#endif /* INC_DYNAMODB_ACCESS_HANDLER_H_ */
