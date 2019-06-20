struct PredictionResult
{
   1: double predictedvalue
   2: string error
}

service house_prediction
{
   PredictionResult getprice(1:map<string, string> house_description)
}
