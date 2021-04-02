/*
 * Created on Fri Apr 02 2021
 *
 * Copyright © 2021 Surfyn
 */

#include "DepositAccess.h"

#include <sstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "sf_services/deposit/thrift_generated/depositservice_types.h"
#include "sf_services/sf_utils/inc/Logger.h"
#include "string"

using Log = surfyn::utils::Logger;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using boost::shared_ptr;

namespace surfyn
{
	namespace rest_server
	{
		DepositAccess::DepositAccess(const std::string& host, int port)
		{
			shared_ptr<TTransport> socket(new TSocket(host, port));
			shared_ptr<TTransport> transport(new TBufferedTransport(socket));
			shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
			m_client = std::make_shared<deposit_serviceClient>(protocol);

			// transport->open();
		}

		DepositResult DepositAccess::announce_deposit(const std::string user_id, std::string data)
		{
			Log::getInstance()->info("Deposit announce");
			auto data_json = json::value::parse(data);
			DepositResult _return;

			if(data_json.has_field("user_id") == false)
			{
				Log::getInstance()->info("User id not found");
				_return.success = false;
				_return.error = "No user_id found";
				return _return;
			}

			std::string userid = data_json.at(U("user_id")).as_string();
			m_client->announce_deposit(_return, userid, data);
			if(_return.success)
			{
				Log::getInstance()->info("Successully deposit announce for user [" + std::string(user_id) + "]");
			}
			else
			{
				Log::getInstance()->error("Unable to deposit announce for user [" + std::string(user_id) + "] error: " + std::string(_return.error));
			}

			return _return;
		}
	}
}