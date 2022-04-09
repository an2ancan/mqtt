#pragma once
#include <iostream>
#include <iomanip>

namespace position_topic{

constexpr auto TOPIC_NAME = "tag_position"; 
struct Position {
    Position(){};
    Position(
        const double _time_stamp, 
        const int _tag_id, 
        const double _x, 
        const double _y,
        const double _z):
            time_stamp(_time_stamp),
            tag_id(_tag_id),
            x(_x),
            y(_y),
            z(_z){}
    double time_stamp = 0.0;   //s
    int tag_id = 0;
    double x = 0;   // m
    double y =0;   // m
    double z =0;   // m
};

std::ostream& operator<< (std::ostream& os, const Position& pos ){
    os << "{ time_stamp: " /*<< std::setprecision (2) */<< pos.time_stamp <<"s\t tag_id: "
        << pos.tag_id << "\t x, y, z: " << pos.x << " " << pos.y << " " << pos.z << "}";
    return os;
}
} // position_topic
