struct DepositResult
{
   1: bool success
   2: string error
}

service deposit_service
{
	DepositResult annouce_deposit(1: string user_id, 2: map<string, string> parameters)
	DepositResult delete_announce(1: string user_id, 2: string announce_id)
}