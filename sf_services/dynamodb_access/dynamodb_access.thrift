struct OperationResult
{
   1: bool success
   2: string error
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
   i64 putAsync(1: i64 userid, 2: string tablename, 3: map<string, ValueType> values),
   i64 getAsync(1: i64 userid, 2: string tablename, 3: KeyValue key, 4: list<string> attributestoget),
   i64 deleteAsync(1: i64 userid, 2: string tablename, 3: KeyValue key),
   i64 updateAsync(1: i64 userid, 2: string tablename, 3: KeyValue key, 4: map<string, ValueType> values),
   OperationResult createTable(1: string tablename, 2: KeyValue key, 3: map<string, string> properties),
   OperationResult deleteTable(1: string tablename)
}
