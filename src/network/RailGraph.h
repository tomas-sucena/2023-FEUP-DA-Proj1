#ifndef DA_TRAINS_RAILGRAPH_H
#define DA_TRAINS_RAILGRAPH_H

#include <string>

#include "DGraph.h"
#include "Railway.hpp"
#include "Station.hpp"

class RailGraph : public DGraph {
public:
    bool addEdge(int src, int dest, double weight = 1, std::string service = "STANDARD", bool valid = true);
};


#endif //DA_TRAINS_RAILGRAPH_H
