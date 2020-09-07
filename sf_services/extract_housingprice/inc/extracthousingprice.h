#ifndef INC_EXTRACT_HOUSING_PRICE_H_
#define INC_EXTRACT_HOUSING_PRICE_H_
#include "sf_services/dynamodb_access/thrift_generated/dynamodb_access.h"
#include <bits/stdint-intn.h>
#include <string>
#include <vector>

namespace surfyn
{
    struct HousingPriceInfo
    {
        int64_t id{};
        std::string city{};
        std::string priceBym2Flat{};
        std::string priceBym2House{};
    };

class HousePriceExtractor
{
private:
    std::shared_ptr<dynamodb_accessClient> m_client;
    std::vector<HousingPriceInfo*> m_housingPriceInfos;

private:
ValueType BuildValueType(const std::string& fieldName, const std::string& fieldValue) const;
void updateValues(const std::string& json, const std::string& medianPrice, const std::string& lowPrice, const std::string& highPrice, std::map<std::string, ValueType>& valuesToUpdate);
public:
    HousePriceExtractor(const std::string& host, int port);
    void ReadHousingPrice(const std::string& tablename);
    void UpdateHousingPriceInfos(const std::string& tablename);
    ~HousePriceExtractor();
};

}
#endif