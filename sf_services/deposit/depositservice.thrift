struct DepositResult
{
   1: bool success
   2: string error
}

service deposit_service
{
	DepositResult announce_deposit(1: string user_id, 2: string data)
	DepositResult delete_announce(1: string user_id, 2: string announce_id)
}