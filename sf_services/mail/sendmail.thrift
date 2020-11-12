struct SendEmailResult
{
   1: bool success
   2: string error
}

service sendemailservice
{
   SendEmailResult sendemailtosurfyn(1: string sender_email, 2:string subject, 3: string msg)
}
