
#include <DataStorage.h>

/// 
/// stores a double in the channelname
void DataStorage::addData(std::string channelName, double data)
{
    _map[channelName] = data;
}

///
/// Resolves the channelName and returns the stored Data
double DataStorage::getData(std::string channelName)
{
    return _map[channelName];
}

std::map<std::string, double> DataStorage::getMapCopy()
{
    std::map<std::string, double> result = _map;
    return result;
}
