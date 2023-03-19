#include "RailGraph.h"

bool RailGraph::addEdge(int src, int dest, double weight, std::string service, bool valid){
    return DGraph::addEdge(new Railway(src, dest, weight, valid, std::move(service)));
}
