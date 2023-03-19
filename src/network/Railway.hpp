#ifndef DA_TRAINS_RAILWAY_HPP
#define DA_TRAINS_RAILWAY_HPP

#include <string>
#include <utility>

#include "implementation/Edge.hpp"

class Railway : public Edge {
    std::string service;

public:
    // constructor
    Railway(int src, int dest, double weight, bool valid, std::string service)
        : Edge(src, dest, weight, valid), service(std::move(service)) {}

    // methods
    std::string getService() const{
        return service;
    }
};

#endif //DA_TRAINS_RAILWAY_HPP
