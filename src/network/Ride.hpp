#ifndef DA_TRAINS_RIDE_HPP
#define DA_TRAINS_RIDE_HPP

#include <string>
#include <utility>

#include "implementation/Edge.hpp"

class Ride : public Edge {
    std::string service;

public:
    // constructor
    Ride(int src, int dest, double weight, bool valid, std::string service)
        : Edge(src, dest, weight, valid), service(std::move(service)) {}

    // methods
    std::string getService() const{
        return service;
    }
};

#endif //DA_TRAINS_RIDE_HPP
