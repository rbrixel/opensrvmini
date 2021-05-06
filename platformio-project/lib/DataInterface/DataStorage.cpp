/*
 * DataStorage.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <DataStorage.h>

/// 
/// stores a double in the channelname
void DataStorage::addData(std::string channelName, double data)
{
    _map[channelName] = data;
}

bool DataStorage::isChannelExistant(std::string channelName)
{
    if (_map.end() == _map.find(channelName))
    {
        return false;
    }
    return true;
}

///
/// Resolves the channelName and returns the stored Data
double DataStorage::getData(std::string channelName)
{
    std::map<std::string, double>::iterator it = _map.find(channelName);
    if (it == _map.end())
    {
        return UNKNOWNRESULT;
    }
    
    return _map[channelName];
}

std::map<std::string, double> DataStorage::getMapCopy()
{
    std::map<std::string, double> result = _map;
    return result;
}
