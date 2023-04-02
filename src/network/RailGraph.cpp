#include "RailGraph.h"

#include <unordered_set>

#define uSet std::unordered_set

RailGraph::RailGraph(int n) : UGraph(n), superSourceID(0), superSinkID(0) {}

/**
 * adds an edge (i.e. a Railway) to the RailGraph
 * @param src index of the source Station
 * @param dest index of the destination Station
 * @param weight number of trains that can simultaneously travel on the new Railway
 * @param service type of service that is provided in the Railway
 * @param valid bool indicating if the Railway should be considered in the algorithms
 * @return 'true' if the insertion occurs, 'false' otherwise
 */
bool RailGraph::addEdge(int src, int dest, double weight, std::string service, bool valid){
    return UGraph::addEdge(new Railway(src, dest, weight, valid, std::move(service)));
}

int RailGraph::addSuperSource() {
    addVertex();
    superSourceID = countVertices();

    return superSourceID;
}

int RailGraph::addSuperSink() {
    addVertex();
    superSinkID = countVertices();

    return superSinkID;
}

bool RailGraph::addSource(int sourceID){
    // super source has not been set
    if (!superSourceID) return false;

    return addEdge(superSourceID, sourceID, INF);
}

bool RailGraph::addSink(int sinkID){
    // super sink has not been set
    if (!superSinkID) return false;

    return addEdge(sinkID, superSinkID, INF);
}

double RailGraph::getFullPicture(){
    return edmondsKarp(superSourceID, superSinkID);
}
