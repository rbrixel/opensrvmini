
#include <DataStorage.h>

void DataStorage::storeDataDouble(std::string channelName, double data)
{
    _map[channelName] = data;
}

void DataStorage::storeDataInt(std::string channelName,int data)
{
    _map[channelName] = data;
}

double DataStorage::getDataDouble(std::string channelName)
{
    return _map[channelName];
}

int DataStorage::getDataInt(std::string channelName)
{
    return _map[channelName];
}
