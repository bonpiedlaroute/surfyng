struct OperationResult
{
   1: bool success
   2: string error
}

struct GetResult
{
   1: OperationResult result
   2: map<string,string> values
}

struct ScanReqResult
{
   1: OperationResult result
   2: list<map<string, string>> values
   3: bool scanend
}

enum Type
{
   NUMBER=1,
   STRING=2
}

struct ValueType
{
   1: string field
   2: Type   fieldtype
}

struct KeyValue
{
   1: string    key
   2: ValueType value
}

service dynamodb_access
{
   OperationResult put(1: string tablename, 2: map<string, ValueType> values),
   GetResult get(1: string tablename, 2: KeyValue key, 3: map<string, ValueType> attributestoget),
   ScanReqResult scan(1: string tablename, 2: map<string, ValueType> attributestoget, 3: string filterexpression, 4: map<string, ValueType> exprValues),
   OperationResult remove(1: string tablename, 2: KeyValue key),
   OperationResult update(1: string tablename, 2: KeyValue key, 3: map<string, ValueType> values),
   OperationResult createTable(1: string tablename, 2: KeyValue key, 3: map<string, string> properties),
   OperationResult deleteTable(1: string tablename)
}
