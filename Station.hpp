#ifndef STATION_HPP
#define STATION_HPP
#include<iostream>
#include<string>
#include<unordered_map>
class Station{
    public:
    std::string station_name;    //for having a name for station
    static int station_id_count; //for having a counter to keep id as unique identifiers
    int station_id;              //this will store the id
    int platformCount;           //to keep a count of platforms

    static std::unordered_map<std::string,Station>stationTable;

    Station( std:: string name);
    Station();

    void addPlatform();

    void giveStationdetails();

    static void printStations();

    // static Station modifyNetwork();

    static Station& createStation(const std::string& name);

    bool operator <(const Station& other) const{
        return station_id < other.station_id;
    }
};

#endif