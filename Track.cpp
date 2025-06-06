#include "Track.hpp"
#include<iostream>
#include<unordered_map>

int Track :: track_id_count = 0;
std::unordered_map<std::pair<int,int>,Track,pair_hash> Track:: trackTable;

Track::Track(int src,int dest,int distance){
    this->src_id = src;
    this->dest_id = dest;
    this->track_length = distance;
    this->track_id = ++track_id_count;
}

void Track::displayTrackProperty(){
    std::cout<<"Source Station: "<< src_id<<std::endl;
    std::cout<<"Destination Station: "<< dest_id<<std::endl;
    std::cout<<"Track ID: "<< track_id<<std::endl;
    std::cout<<"Track length: "<<track_length<<std::endl;
}

Track Track:: constructTrack(int src_id,int dest_id,int distance){
    if(src_id==dest_id){
        throw std::invalid_argument("Source and Destination can't be same");
    }
    auto it = trackTable.find({src_id,dest_id});
    if(it!=trackTable.end()){
        return it->second;
    }
    Track ob(src_id,dest_id,distance);
    trackTable.insert({std::make_pair(src_id,dest_id),ob});
    return ob;
}