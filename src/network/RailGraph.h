#ifndef DA_TRAINS_RAILGRAPH_H
#define DA_TRAINS_RAILGRAPH_H

#include <list>
#include <string>

#include "DGraph.h"
#include "Railway.hpp"
#include "Station.hpp"

class RailGraph : public DGraph {
    int superSourceID, superSinkID;

public:
    // constructor
    RailGraph();

    bool addEdge(int src, int dest, double weight = 1, std::string service = "STANDARD", bool valid = true);
    int addSuperSource(), addSuperSink();
    bool addSource(int sourceID), addSink(int sinkID);

    // flow functions
    double getFullPicture();
};


#endif //DA_TRAINS_RAILGRAPH_H
