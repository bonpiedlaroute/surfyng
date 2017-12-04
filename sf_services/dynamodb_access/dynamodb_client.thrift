struct OperationResultAsync
{
   1: bool success
   2: string error
}

service dynamodb_client
{
   void notifyPutAsync(1: i64 jobid, 2: OperationResultAsync operationresult),
   void notifyGetAsync(1: i64 jobid, 2: map<string, string> values, 3: OperationResultAsync operationresult),
   void notifyDeleteAsync(1: i64 jobid, 2: OperationResultAsync operationresult),
   void notifyUpdateAsync(1: i64 jobid, 2: OperationResultAsync operationresult)
}
