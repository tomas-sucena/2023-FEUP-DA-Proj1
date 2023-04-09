#include "RailGraph.h"

#include <algorithm>
#include <map>

/**
 * @brief computes the maximum amount of trains that can circulate (i.e. max flow), considering the entire railway network
 * @complexity O(|V| * |E|^2)
 */
void RailGraph::getFullPicture() {
    if (fullPicture) return;
    fullPicture = true;

    // create the super source
    UGraph::addVertex();
    int superSource = countVertices();

    for (int src : railwaySources)
        (*this)[superSource].out.push_back(new Edge(superSource, src, INF, true));

    // create the super sink
    UGraph::addVertex();
    int superSink = countVertices();

    for (int sink : railwaySinks)
        (*this)[sink].out.push_back(new Edge(sink, superSink, INF, true));

    // compute the maximum flow of the entire network
    maximumFlow(superSource, superSink);

    // remove the super sink
    vertices.pop_back();

    for (int sink : railwaySinks)
        (*this)[sink].out.pop_back();

    // remove the super source
    vertices.pop_back();
}

/**
 * @brief creates a new RailGraph object
 * @param n number of vertices (i.e. Stations) the RailGraph will be initialized with
 */
RailGraph::RailGraph(int n) : UGraph(n), fullPicture(false) {
    servicePrices["STANDARD"] = 2;
    servicePrices["ALFA PENDULAR"] = 4;

    // ensure invalid Railways are not reset
    autoResetSettings.edgeValid = false;
}

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

    // set the reverse edges
    r->reverse = (Railway*) (*this)[src].inEdges().back();
    r->reverse->reverse = r;

    // update the districts map
    std::string srcDistrict = (*this)[src].getDistrict();
    std::string destDistrict = (*this)[dest].getDistrict();

    districtRailways[srcDistrict].push_back(r);
    if (srcDistrict != destDistrict) districtRailways[destDistrict].push_back(r);

    // update the municipalities map
    std::string srcMunicipality = (*this)[src].getMunicipality();
    std::string destMunicipality = (*this)[dest].getMunicipality();

    municipalityRailways[srcMunicipality].push_back(r);
    if (srcMunicipality != destMunicipality) municipalityRailways[destMunicipality].push_back(r);

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

/**
 * @brief returns a railway network with reduced connectivity (i.e. a few Railways are invalid)
 * @complexity O(|E|^2)
 * @param edgesToRemove list containing the indices of the source and destination vertices of the edges that will be invalidated
 * @return railway network with reduced connectivity (subgraph)
 */
RailGraph RailGraph::getSubgraph(const list<std::pair<int, int>>& edgesToRemove) {
    RailGraph sub = *this;

    for (auto& p : edgesToRemove){
        for (auto e: sub[p.first].outEdges()){
            auto r = (Railway*) e;
            if (r->getDest() != p.second) continue;

            r->valid = false;
            r->reverse->valid = false;

            break;
        }
    }

    return sub;
}

/**
 * computes the top-k stations with the most trains circulating in them (i.e. most flow)
 * @param k number of stations that will be selected
 * @return list with the names and the number of trains circulating in the top-k stations
 */
std::list<std::pair<string, double>> RailGraph::getBusiestStations(int k){
    getFullPicture();
    k = (k > countVertices()) ? countVertices() : k;

    // compute the total flow of each station
    std::vector<std::pair<int, double>> stationsFlow(countVertices());
    for (const Edge* e : edges){
        auto& p = stationsFlow[e->getDest() - 1];

        p.first = e->getDest();
        p.second += e->getFlow();
    }

    // sort the stations by descending order of flow
    std::sort(stationsFlow.begin(), stationsFlow.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    std::list<std::pair<string, double>> busiestStations;
    for (int i = 0; i < k; ++i){
        auto& p = stationsFlow[i];
        busiestStations.emplace_back((*this)[p.first].getName(), p.second);
    }

    return busiestStations;
}

/**
 * computes the top-k districts with the most trains circulating in them (i.e. most flow)
 * @param k number of districts that will be selected
 * @return list with the names and the number of trains circulating in the top-k districts
 */
std::list<std::pair<string, double>> RailGraph::getBusiestDistricts(int k){
    getFullPicture();
    k = (k > districtRailways.size()) ? (int) districtRailways.size() : k;
    
    // compute the total flow of each district
    std::vector<std::pair<std::string, double>> districtsFlow;

    for(auto& p: districtRailways){
        double flow = 0;
        for (const Edge* e : p.second)
            flow += e->getFlow();

        districtsFlow.emplace_back(p.first, flow);
    }

    // sort the districts by descending order of flow
    std::sort(districtsFlow.begin(), districtsFlow.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    std::list<std::pair<string, double>> busiestDistricts;
    for (int i = 0; i < k; ++i){
        busiestDistricts.emplace_back(districtsFlow[i].first, districtsFlow[i].second);
    }

    return busiestDistricts;
}

/**
 * computes the top-k municipalities with the most trains circulating in them (i.e. most flow)
 * @param k number of municipalities that will be selected
 * @return list with the names and the number of trains circulating in the top-k municipalities
 */
std::list<std::pair<string, double>> RailGraph::getBusiestMunicipalities(int k){
    getFullPicture();
    k = (k > municipalityRailways.size()) ? (int) municipalityRailways.size() : k;
    
    // compute the total flow of each municipality
    std::vector<std::pair<std::string, double>> municipalitiesFlow;

    for(auto& p: municipalityRailways){
        double flow = 0;
        for (const Edge* e : p.second){
            flow += e->getFlow();
        }

        municipalitiesFlow.emplace_back(p.first, flow);
    }

    // sort the municipalities by descending order of flow
    std::sort(municipalitiesFlow.begin(), municipalitiesFlow.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    std::list<std::pair<string, double>> busiestMunicipalities;
    for(int i = 0; i < k; ++i)
        busiestMunicipalities.emplace_back(municipalitiesFlow[i].first, municipalitiesFlow[i].second);

    return busiestMunicipalities;
}

/**
 * computes, from all pairs of stations, which ones require the most amount of trains when taking full advantage of the
 * existing network capacity
 * @param maxTrains attribute whose value will be the maximum number of trains that the pairs of stations require
 * @return list containing the pairs of stations that require the most amount of trains
 */
std::list<std::pair<int, int>> RailGraph::getBusiestStationPairs(double& maxTrains){
    std::list<std::pair<int, int>> busiestPairs;

    autoReset = false;
    fullPicture = false;

    resetAll();

    for (int i = 1; i < countVertices(); ++i){
        for (int j = i + 1; j <= countVertices(); ++j){
            resetEdges();

            double flow = maximumFlow(i, j);
            if (flow < maxTrains) continue;

            if (flow > maxTrains){
                busiestPairs.clear();
                maxTrains = flow;
            }

            busiestPairs.emplace_back(i, j);
        }
    }

    autoReset = true;
    return busiestPairs;
}

double RailGraph::getIncomingTrains(int index, fort::utf8_table* table){
    double flow = 0;
    getFullPicture();

    // create the table
    if (table != nullptr) *table = Utils::createTable({"Station", "Service", "Capacity", "Trains"});

    // compute the flow
    for (const Edge* e : (*this)[index].inEdges()){
        flow += e->getFlow();

        if (table == nullptr) continue;

        auto r = (Railway*) e;
        *table << (*this)[r->getSrc()].getName() << r->getService() << r->getWeight() << r->getFlow() << fort::endr;
    }

    return flow;
}

/**
 * @brief computes the minimum cost paths between two Stations
 * @brief O(|V| * |E|^2)
 * @param src index of the source Station
 * @param sink index of the destination Station
 * @param maxTrains variable that will store the maximum amount of trains that can simultaneously travel between the stations
 * @param minCost variable that will store the price of maintaining each minimum cost path
 * @return list containing the minimum cost paths
 */
std::list<Path> RailGraph::getMinimumCostPaths(int src, int sink, double& maxTrains, double& minCost){
    // compute the maximum flow
    std::list<Path> paths;
    maximumFlow(src, sink, &paths);

    // compute the flow and cost of each path
    minCost = INF;
    std::list<std::pair<double, double>> flowAndCost;

    for (auto it = paths.begin(); it != paths.end();){
        double pathFlow = INF;
        double pathCost = 0;

        for (const Edge* e : *it){
            auto r = (Railway*) e;

            pathFlow = std::min(r->getFlow(), pathFlow);
            pathCost += servicePrices[r->getService()];
        }

        if (pathCost > minCost){
            it = paths.erase(it);
            continue;
        }

        minCost = pathCost;
        flowAndCost.emplace_back(pathFlow, pathCost);

        ++it;
    }

    // delete the paths that don't have minimum cost
    auto it = paths.begin();

    for (auto& p : flowAndCost){
        if (p.second > minCost){
            it = paths.erase(it);
            continue;
        }

        maxTrains += p.first;
        ++it;
    }

    return paths;
}
