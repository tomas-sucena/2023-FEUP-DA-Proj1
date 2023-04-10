#ifndef DA_TRAINS_RAILWAY_HPP
#define DA_TRAINS_RAILWAY_HPP

#include <string>
#include <utility>

#include "implementation/Edge.hpp"

class Railway : public Edge {
    friend class RailGraph;

/* ATTRIBUTES */
private:
    std::string service;
    Railway* reverse;

/* CONSTRUCTOR */
public:
    /**
     * @brief creates a new Railway object
     * @param src index of the source station
     * @param dest index of the destination station
     * @param weight capacity of the railway
     * @param valid bool that indicates if the railway is operational
     * @param service type of service that is provided in the railway
     */
    Railway(int src, int dest, double weight, bool valid, std::string service)
        : Edge(src, dest, weight, valid), service(std::move(service)), reverse(nullptr) {}

/* METHODS */
public:
    /**
     * @brief creates a copy of the Railway
     * @return pointer to a copy of the Railway
     */
    Edge* clone() const override{
        return new Railway(*this);
    }

    /**
     * @brief returns the type of service that is provided in the railway
     * @return type of service that is provided in the railway
     */
    std::string getService() const{
        return service;
    }
};

#endif //DA_TRAINS_RAILWAY_HPP
