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

			transport->open();
		}

		DepositResult DepositAccess::announce_deposit(const std::string user_id, std::string data)
		{
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
			
			try
			{
				m_client->announce_deposit(_return, userid, data);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
				Log::getInstance()->info(e.what());
			}

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
<<<<<<< HEAD
		
=======

>>>>>>> Announce deposit available
		DepositResult DepositAccess::delete_announce(const std::map<utility::string_t,  utility::string_t>& query)
		{
			auto iter = query.find("user_id");

			DepositResult _return;
			if(iter == query.end())
			{
				_return.success = false;
				_return.error = "No user_id found!";

				return _return;
			}
			std::string user_id = iter->second;

			iter = query.find("ad_id");
			if(iter == query.end())
			{
				_return.success = false;
				_return.error = "No announce_id found!";

				return _return;
			}
			std::string ad_id = iter->second;

			m_client->delete_announce(_return, user_id, ad_id);

			return _return;
		}

		void DepositAccess::fetch_user_announces(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query)
		{
			auto iter = query.find("userid");
			if(iter == query.end())
			{
				sstream << U("[]");
				return;
			}
			
			std::string user_id = iter->second;
			std::vector<std::map<std::string, std::string>> _return;
			m_client->fetch_user_announces(_return, user_id);

			if(_return.empty())
			{
				sstream << U("[]");
				return;
			}

			sstream << U("[\n");
			for(auto iter_my_ad = _return.begin(); iter_my_ad != _return.end(); ++iter_my_ad)
			{
				if(iter_my_ad != _return.begin())
				{
					sstream << U(",\n");
				}
				sstream << U("{\n");
				for(auto attribute_iter = iter_my_ad->begin(); attribute_iter != iter_my_ad->end(); ++attribute_iter)
				{
					if( attribute_iter != iter_my_ad->begin())
					{
					sstream << U(",\n");
					}

					sstream << "\"";
					sstream << attribute_iter->first;
					sstream << "\":\"";
					sstream << attribute_iter->second;
					sstream << "\"";
				}
				sstream << U("\n}");

			}
			sstream << U("\n]");
		}
	}
}