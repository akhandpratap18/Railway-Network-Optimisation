#include "Station.hpp"
#include "Track.hpp"
#include <vector>
#include <utility>
#include<map>
#include <queue>
#include <unordered_map>
#include <set>
#include<algorithm>


std::map<Station, std::vector<std::pair<Track,Station>>>railNetwork;   //in order to have a map that stores complete network


 void addTracks(std::string s1, std::string s2, int weight){
    Station ob1,ob2;
    if(Station::stationTable.find(s1) != Station::stationTable.end()){
        ob1 = Station::stationTable[s1];
    }
    else{
        ob1 = Station::createStation(s1);
    }

    if(Station::stationTable.find(s2) != Station::stationTable.end()){
        ob2 = Station::stationTable[s2];
    }
    else{
        ob2 = Station::createStation(s2);
    }

    Track ob = Track::constructTrack(ob1.station_id,ob2.station_id,weight);
    railNetwork[ob1].push_back(std::make_pair(ob, ob2));
    railNetwork[ob2].push_back(std::make_pair(ob,ob1));
}

void displayNetwork(){
    for(auto it: railNetwork){
        std::cout<< it.first.station_name <<" - {";
        std::vector<std::pair<Track,Station>>vec = it.second;
        for(auto i:vec){
            Track ob = i.first;
            Station obj = i.second;
            std::cout<<"{"<<ob.track_id <<", "<<obj.station_name<<" },";
        }
        std::cout<<std::endl;
    }
}



// -----A* Search--------
struct AStarNode {
    Station station;
    int f_cost;

    bool operator>(const AStarNode& other) const {
        return f_cost > other.f_cost;
    }
};

// Simple heuristic function: returns 0 (Dijkstra's algorithm). Replace with a better heuristic if available.
int heuristic(const Station& a, const Station& b) {
    return 0;
}

std::vector<std::string> aStarSearch(const std::string& startName, const std::string& goalName) {
    if (Station::stationTable.find(startName) == Station::stationTable.end() ||
        Station::stationTable.find(goalName) == Station::stationTable.end()) {
        std::cout << "Start or goal station not found!" << std::endl;
        return {};
    }

    Station start = Station::stationTable[startName];
    Station goal = Station::stationTable[goalName];

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<>> openSet;
    std::unordered_map<std::string, int> gScore;
    std::unordered_map<std::string, std::string> cameFrom;
    std::set<std::string> visited;

    gScore[start.station_name] = 0;
    openSet.push({start, heuristic(start, goal)});

    while (!openSet.empty()) {
        Station current = openSet.top().station;
        openSet.pop();

        if (current.station_name == goal.station_name) {
            // reconstruct path
            std::vector<std::string> path;
            std::string curr = goal.station_name;
            while (curr != start.station_name) {
                path.push_back(curr);
                curr = cameFrom[curr];
            }
            path.push_back(start.station_name);
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (visited.count(current.station_name)) continue;
        visited.insert(current.station_name);

        for (auto& neighborPair : railNetwork[current]) {
            Track t = neighborPair.first;
            Station neighbor = neighborPair.second;

            int tentative_gScore = gScore[current.station_name] + t.track_length;

            if (!gScore.count(neighbor.station_name) || tentative_gScore < gScore[neighbor.station_name]) {
                cameFrom[neighbor.station_name] = current.station_name;
                gScore[neighbor.station_name] = tentative_gScore;
                int fScore = tentative_gScore + heuristic(neighbor, goal);
                openSet.push({neighbor, fScore});
            }
        }
    }

    std::cout << "No path found.\n";
    return {};
}





//-------Dijkstra-------------////////

void dijkstra(const std::string& startStationName) {
    if (Station::stationTable.find(startStationName) == Station::stationTable.end()) {
        std::cout << "Start station not found.\n";
        return;
    }

    Station startStation = Station::stationTable[startStationName];

    std::map<Station, int> distances;
    std::map<Station, Station> previous;

    for (std::map<Station, std::vector<std::pair<Track, Station>>>::iterator it = railNetwork.begin(); it != railNetwork.end(); ++it) {
        distances[it->first] = std::numeric_limits<int>::max();
    }
    distances[startStation] = 0;

    typedef std::pair<int, Station> P; // distance, station
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push(std::make_pair(0, startStation));

    while (!pq.empty()) {
        P top = pq.top(); pq.pop();
        int dist = top.first;
        Station current = top.second;

        if (dist > distances[current]) continue;

        std::vector<std::pair<Track, Station>> neighbors = railNetwork[current];
        for (size_t i = 0; i < neighbors.size(); ++i) {
            Track track = neighbors[i].first;
            Station neighbor = neighbors[i].second;
            int weight = track.track_length;
            int newDist = distances[current] + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = current;
                pq.push(std::make_pair(newDist, neighbor));
            }
        }
    }

    std::cout << "\nDijkstra's result from: " << startStationName << "\n";
    for (std::map<Station, int>::iterator it = distances.begin(); it != distances.end(); ++it) {
        std::cout << "To " << it->first.station_name << ": ";
        if (it->second == std::numeric_limits<int>::max())
            std::cout << "Unreachable\n";
        else
            std::cout << it->second << "\n";
    }
}


void bfsMinNodes(const std::string& startStationName, const std::string& endStationName) {
    if (Station::stationTable.find(startStationName) == Station::stationTable.end() ||
        Station::stationTable.find(endStationName) == Station::stationTable.end()) {
        std::cout << "Start or end station not found.\n";
        return;
    }

    Station startStation = Station::stationTable[startStationName];
    Station endStation = Station::stationTable[endStationName];

    std::map<int, Station> previous;  // station_id -> previous station
    std::set<int> visited;
    std::queue<Station> q;

    visited.insert(startStation.station_id);
    q.push(startStation);

    bool found = false;

    while (!q.empty()) {
        Station current = q.front();
        q.pop();

        if (current.station_id == endStation.station_id) {
            found = true;
            break;
        }

        for (const auto& neighborPair : railNetwork[current]) {
            Station neighbor = neighborPair.second;
            if (visited.find(neighbor.station_id) == visited.end()) {
                visited.insert(neighbor.station_id);
                previous[neighbor.station_id] = current;
                q.push(neighbor);
            }
        }
    }

    if (!found) {
        std::cout << "No path found between " << startStationName << " and " << endStationName << "\n";
        return;
    }

    // Reconstruct path
    std::vector<std::string> path;
    Station current = endStation;
    while (current.station_id != startStation.station_id) {
        path.push_back(current.station_name);
        current = previous[current.station_id];
    }
    path.push_back(startStation.station_name);

    std::reverse(path.begin(), path.end());

    std::cout << "\nShortest path (min nodes) from " << startStationName << " to " << endStationName << ":\n";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i != path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
}

void dfsAllPathsUtil(const Station& current, const Station& destination, 
    std::set<int>& visited, std::vector<std::string>& path) {
visited.insert(current.station_id);
path.push_back(current.station_name);

if (current.station_id == destination.station_id) {
// Print the current path
for (size_t i = 0; i < path.size(); ++i) {
std::cout << path[i];
if (i != path.size() - 1) std::cout << " -> ";
}
std::cout << "\n";
} else {
for (const auto& neighborPair : railNetwork[current]) {
const Station& neighbor = neighborPair.second;
if (visited.find(neighbor.station_id) == visited.end()) {
dfsAllPathsUtil(neighbor, destination, visited, path);
}
}
}

// Backtrack
visited.erase(current.station_id);
path.pop_back();
}
void printAllPathsDFS(const std::string& startName, const std::string& endName) {
    if (Station::stationTable.find(startName) == Station::stationTable.end() ||
        Station::stationTable.find(endName) == Station::stationTable.end()) {
        std::cout << "One or both stations not found.\n";
        return;
    }

    Station start = Station::stationTable[startName];
    Station end = Station::stationTable[endName];
    std::set<int> visited;
    std::vector<std::string> path;

    std::cout << "All paths from " << startName << " to " << endName << ":\n";
    dfsAllPathsUtil(start, end, visited, path);
}



// int main(){
    
//     Station s = Station::createStation("Lucknow");
//     s.giveStationdetails();
//     Station s1 = Station::createStation("Dehradun");
//     s1.addPlatform();
//     s1.giveStationdetails();
//     Station s2 = Station::createStation("Dehradun");
//     s2.addPlatform();
//     s1.addPlatform();
//     s2.giveStationdetails();
//     s.addPlatform();

//     Station::printStations();

//     addTracks("Lucknow","Dehradun",525);
//     addTracks("Lucknow","Kanpur",200);
//     addTracks("Dehradun","Haridwar",65);
//     addTracks("Dehradun","Chandigarh",220);
//     addTracks("Kanpur","Chandigarh",200);

//     displayNetwork();

//     std::vector<std::string>ans = aStarSearch("Lucknow","Chandigarh");
//     std::cout<<"A Search: "<<std::endl;
//     for(auto it: ans){
//         std::cout<<it<<std::endl;
//     }


//     dijkstra("Lucknow");
//     bfsMinNodes("Lucknow", "Haridwar");
//     printAllPathsDFS("Lucknow", "Haridwar"); 

// }