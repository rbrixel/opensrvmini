#include <BMEDataCollector.h>
#include <Arduino.h>

BMEDataCollector::BMEDataCollector(std::string channelName)
{
    _channelName = channelName;
}

void BMEDataCollector::setDataStorage(IDataStorage *storage)
{
    _dataStorage = storage;
}

void BMEDataCollector::updateData()
{
    double value = readBMEData();
    _dataStorage->storeDataDouble(_channelName, value);
}

std::string BMEDataCollector::getName(){
    return _channelName;
}

double  BMEDataCollector::readBMEData() {
    return std::rand()/((RAND_MAX + 1u)/35);;
}
