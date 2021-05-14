struct SendEmailResult
{
   1: bool success
   2: string error
}

service sendemailservice
{
   SendEmailResult sendemailtosurfyn(1: string sender_email, 2:string subject, 3: string msg),
   SendEmailResult sendaccountcreationemail(1:string display_name, 2:string new_account_email)
}
