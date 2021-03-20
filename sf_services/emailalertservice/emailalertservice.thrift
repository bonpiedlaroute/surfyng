struct EmailAlertResult
{
   1: bool success
   2: string error
}
service emailalertservice
{
   EmailAlertResult registerEmailAlert(1: string userid, 2: map<string, string> parameters),
   EmailAlertResult notifyNewAnnounces(1: string city),
   list<map<string, string>> my_realestate_search(1: string userid),
   EmailAlertResult  changeAlertStatus(1: string alert_id, 2:string alert_status)
}
