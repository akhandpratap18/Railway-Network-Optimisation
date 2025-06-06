#ifndef TRACK_HPP
#define TRACK_HPP
#include<iostream>
#include<unordered_map>

struct pair_hash {
    size_t operator()(const std::pair<int, int>& p) const {
        // Uses the hash of both elements and combines them
        size_t h1 = std::hash<int>()(p.first);
        size_t h2 = std::hash<int>()(p.second);

        // A common hash combining technique
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
}; // Closing brace for pair_hash

class Track{
    public: 
    int track_length,track_id,src_id,dest_id;
    static int track_id_count;
    
    static std::unordered_map<std::pair<int, int>, Track, pair_hash> trackTable; // Declaration only
    

    Track(int src, int dest, int w);

    static Track constructTrack(int src, int dest, int w);

    void displayTrackProperty();

    bool operator <(const Track& other) const{
        return track_id < other.track_id;
    }
};
#endif