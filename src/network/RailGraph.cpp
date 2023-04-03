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

std::list<std::pair<int, int>> RailGraph::getBusiestStationPairs(double& maxFlow){
    std::list<std::pair<int, int>> busiestPairs;

    reset = false;
    resetAll();

    // invalidate the superSource and the superSink
    (*this)[superSourceID].valid = false;
    (*this)[superSinkID].valid = false;

    for (int i = 1; i <= countVertices() - 2; ++i){
        for (int j = i + 1; j <= countVertices() - 2; ++j){
            resetEdges();

            double flow = maximumFlow(i, j);
            if (flow < maxFlow) continue;

            if (flow > maxFlow){
                busiestPairs.clear();
                maxFlow = flow;
            }

            busiestPairs.emplace_back(i, j);
        }
    }

    return busiestPairs;
}

RailGraph RailGraph::subGraph(list<std::pair<int, int>> edgesList) {
    RailGraph copy = *this;
    for(auto i : edgesList){
        for(auto e: copy[i.first].outEdges()){
            if(e->getDest() == i.second){
                e->valid = false;
                break;
            }
        }
    }
    return copy;
}
