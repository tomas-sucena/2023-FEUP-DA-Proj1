#include "RailGraph.h"

#include <unordered_set>
#include <algorithm>

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
    auto r = new Railway(src, dest, weight, valid, std::move(service));
    if (!UGraph::addEdge(r)) return false;

    // update the districts map
    std::string srcDistrict = (*this)[src].getDistrict();
    std::string destDistrict = (*this)[dest].getDistrict();

    districts[srcDistrict].push_back(r);
    if (srcDistrict != destDistrict) districts[destDistrict].push_back(r);

    // update the municipalities map
    std::string srcMunicipality = (*this)[src].getMunicipality();
    std::string destMunicipality = (*this)[dest].getMunicipality();

    municipalities[srcMunicipality].push_back(r);
    if (srcMunicipality != destMunicipality) municipalities[destMunicipality].push_back(r);
    
    return true;
}

/**
 * @brief accesses a vertex (i.e. a Station) of the Graph and allows modifications to be made to it
 * @param index index of the vertex
 * @return reference of the vertex
 */
Station& RailGraph::operator[](int index){
    return (Station&) Graph::operator[](index);
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

RailGraph RailGraph::subGraph(const list<std::pair<int, int>>& edgesList) {
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

double RailGraph::reducedConnectivity(int start, int end, RailGraph sub) {
    return sub.edmondsKarp(start, end);
}

std::vector<std::pair<int, int>> RailGraph::mostAffected(RailGraph sub, int k) {
    std::vector<std::pair<int, int>> out;
    sub.getFullPicture();
    for(int i = 1; i < vertices.size(); i++){
        out.emplace_back(i, sub[i].inDegree());
    }
    std::sort(out.begin(), out.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
    out.resize(k);
    return out;
}
