#include "Station.hpp"

int Station :: station_id_count = 0;
std::unordered_map<std::string,Station> Station::stationTable;
    
    Station::Station(){}
    Station :: Station(std :: string name){
        this->station_name = name;
        this->station_id = ++station_id_count;
        this->platformCount = 0;
    }
    
    void Station::addPlatform() {
    Station ob = stationTable[this->station_name];
    // Update the station in the map to reflect the new platform count
     ob.platformCount = ++ob.platformCount;
    stationTable[this->station_name] = ob;
}


    void Station :: giveStationdetails(){
        std::cout<< "Station Name: "<<station_name<<std::endl;
        std::cout<< "Station ID: "<<station_id<<std::endl;
        std::cout<< "Number of platforms: "<<platformCount<<std::endl;
    }

     void Station::printStations(){
        for (auto it: stationTable){
            std::cout<< it.first<<" - ";
            std::cout<<"Station ID: "<<it.second.station_id<<" Platforms: "<<it.second.platformCount<<std::endl;
        }
    }

    // Station Station :: modifyNetwork(){
    // std::string new_station;
    // std::cout << "Enter new station name: ";
    // std::getline(std::cin >> std::ws, new_station);  // flush leading whitespace
    // Station ob(new_station);
    // return ob;
    // }

   Station& Station::createStation(const std::string& name) {
    auto it = stationTable.find(name);

    if (it != stationTable.end()) {
        // If the station exists, just return the reference to it
        return it->second;
    }

    // Otherwise, create a new station, add it to the map and return the reference
    Station newStation(name);
    stationTable.insert({name, newStation});
    return stationTable[name]; // returns the reference to the newly inserted station
}





