/*
 * Created on Fri Apr 02 2021
 *
 * Copyright © 2021 Surfyn
 */

#ifndef INC_DEPOSITACCESS_H_
#define INC_DEPOSITACCESS_H_
#include "stdafx.h"
#include <map>
#include "sf_services/deposit/thrift_generated/deposit_service.h"
#include <memory>
#include <string>

using namespace web;

namespace surfyn
{
	namespace rest_server
	{
		class DepositAccess
		{
			private:
				std::shared_ptr<deposit_serviceClient> m_client;
			public:
				DepositAccess(const std::string& host, int port);
				DepositResult announce_deposit(const std::string user_id, std::string data);
				DepositResult delete_announce(const std::map<utility::string_t,  utility::string_t>& query);
				void fetch_user_announces(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query);

		};
	}
}
#endif /* INC_DEPOSITACCESS */