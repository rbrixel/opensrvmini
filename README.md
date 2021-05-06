# OpenSRVmini
![ESPBuildBadge](https://github.com/rbrixel/opensrvmini/actions/workflows/build.yml/badge.svg)

![OpenSRVmini Logo](./OpenSRVmini-Logo.jpg)

#WIP - Work in progress

## Description/Beschreibung

**(EN)** With OpenSRVmini you can measure the following in your caravan / motor home: temperature inside and outside, air humidity inside, 12V battery and the location using a gyro sensor.
The circuit communicates with a smartphone app via Bluetooth. A new firmware can be flashed via wifi via OTA update.

**(DE)** Mit OpenSRVmini kannst du folgendes in deinem Wohnwagen/Wohnmobil messen: Temperatur innen und außen, Lufgfeuchtigkeit innen, 12V Akku und die Lage anhand einem Kreiselsensor.
Die Schaltung kommuniziert über Bluetooth mit einer Smartphone-App. Über WLAN kann via OTA-Update eine neue Firmware geflashed werden.

## Hardware

* ESP32 "Dev Kit" (with wifi and bluetooth)
* BME280 (temperature, humidity and air pressure for indoor)
* MPU-6050 (3-axis-gyroscope)
* DS18B20 (waterproof for outdoor measurement)
* ADS1115 (4-Channel, 16-Bit AD, i2c)

**(EN)** I use the ADS1115 to measure the current because the EPS32's internal ADC is not accurate enough.

**(DE)** Ich verwende zur Messung des Stroms den ADS1115, da der interne ADC des EPS32 nicht genau genug ist.

## Software

Using VSCode with installed Platform IO Extension for development.
PlatformIO supports the Arduino Framework which makes it more easy to access allready supported devices.

* https://code.visualstudio.com/
* https://platformio.org/

# Startup Hints

After cloning the repository you can open the platformio-project folder as a project:

![PIO Open Project](./pio-openproject.png)  
![PIO Choose Folder](./pio-choosefolder.png)

Once the project opened one way to use Platform IO environment is using the taskbar:
![PIO Taskbar](./pio-taskbar.png)

Right from the GIT symbols the Platform IO Icons are shown.
* House: Go TO PlatformIO home
* Checkmark : Compile current project using selected Environment (later here)
* Arrow: Compile current project and upload to ESP using selected Environment (later here)
* Trashcan: Clean current Project
* Connector Plug: Show ESP Serial Monitor interface
* Square: Open Terminal (less used)
* Folder Symbol with env:.... : Select active environment 

### Environment 

Please use https://platformio.org/ or https://docs.platformio.org/en/latest/projectconf/index.html for detailed descriptions. For this project so far just the info, that it will have multiple environments configured, depending for developer and connection.
Each section like [env:esp32dev-ota] in platformio.ini describes a usable environment, which can be used. This makes it more easy to switch between OTA upload or different serial ports.

# Coding

## Short Introduction
Based on Arduino Project there are one setup function and a loop function. The Setup will run once after the startup, and is used to initialize IOs, Hardware etc. The loop function is called again direct after it has been finished.
This project goal is to not change the loop code itself for customized codings to implement new sensors or similar.

## Participants in this game

### DataCollectors
The idea is to have a set of DataCollectors (implementing/deriving IDataCollector interface), that are used to get one ore more double values from a Hardware Sensor. The Datacollector Implementation does everything to intialize the handled hardware, and fill the datastorage that has been setup during the initialisation phase.
There is one Collector implementation for every type of hardware, and i would suggest to use one instance per physical sensor. 

### DataStorage
The Datastorage right now is just a map <String,Double> where the Collectors can store its collected Values. The Key is used as a channelname. Every Collector Instance has to use its own unique channelname, and for every stored value type an extension. Example: BME Collector will save temperature as well as pressure. The Channelname could be "BMESENSOR" and with both extensions the Stored Keys are:
BMESENSOR.TEMP and BMESENSOR.PRESS
There is no check on this keys at the moment, the data is stored as it is. Future idea could be to implement a historical data interface, to get the last xx values.

### DataActors
The IDataActor Interface is used to perform actions on the given Data. The first implementation is the RangeDataActor, which can set GPIO to high or low, depending on the data in the datastorage, compared to the range what is configured to the DataActor. It is possible for example to check the Channel "ADSSENSOR.VCC" if it in range from 11.5V to 14.0V, but if it is out of range the GPIO x will switched to HIGH. You get an alarm if the battery is getting low.
There is no restriction for an DataActor to access the DataStorage, it can use all Data that has been stored. This gives the possibility to write an implementation to compare multiple sensors and do an action for them (check inbound humidity and outbound humidity, is outbound less than turn on fan).

## Main.cpp

### SETUP Parts

#### Instanciation
Create all Instanzes of DataCollectors, DataActors and the DataStorage. Depending on implementation already during instanciation some parameters are needed to configure things like channelname etc.

#### Initialisation
Setup will call first for all DataCollectors the ::init(dataStorage) function do setup the datastorage. The Datacollector itself can use this call to activate the sensors or getting a default state for startup.
Setup will call then for all DataActors the ::init() function, which can be used in the same way to activate actors in a default state, or initiate a protocoll for data sending.

### LOOP Parts

#### Reinit DataCollectors
A DataCollector can tell the main loop if it needs to be reinitialized. This will be done in the next loop before collecting new data.
For this the collector can override needsReInit() function returning false if everything is ok, or true if a new initialisation is needed. The init will be done synchronous one collector after the other to prevent thread issues.

#### Update Data
For each collector the ->updateData(); method is called. In this method the DataCollector should connect to its hardware, query the last value, and store it into the dataStorage using its channelname (and extension).

#### Update Actors
For each configured DataActor the action(storage); method is called. The actor can now use the current values to create its own actions, like sending data, or switching GPIO.


## How should I implement something
Given the description above please try not to change the main.cpp file. 
Create a new folder under lib (use an existing lib as example) and add your code.

### You want to use your own sensor?
Write a library like the BMEDataCollector, implemment the IDataCollector interface, use init() to startup your sensor, and update to store your data. Thats all for now.

### You want to create a custom Action?
Write a library like the RangeDataActor, implement the IDataActor interface, use init() to startup your action, and implement your action itself in the action() method. Suggest to use init phase or instanciation phase to configure the actor to an existing channelname.

### you want to keep historical data?
Not possible at the moment, but it could be a way to write an IDataActor, that stores all new values into a ringbuffer. 

### I want to add an HTTP interface ...
good luck ;-)



## Facebook-Group/-Gruppe

Only in german / nur in Deutsch: https://www.facebook.com/groups/opensrv

