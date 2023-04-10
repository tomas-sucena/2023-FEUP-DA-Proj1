#ifndef DA_TRAINS_RAILGRAPH_H
#define DA_TRAINS_RAILGRAPH_H

#include "../Utils.hpp"
#include "Railway.hpp"
#include "Station.hpp"
#include "UGraph.h"

class RailGraph : public UGraph {
    friend class Helpy;

/* ATTRIBUTES */
private:
    bool fullPicture;
    uMap<std::string, double> servicePrices;
    std::list<Edge*> underMaintenance;

    // for searching
    uSet<int> railwaySources, railwaySinks;
    uMap<std::string, std::list<Railway*>> districtRailways, municipalityRailways;

/* CONSTRUCTOR */
public:
    explicit RailGraph(int n = 0);

/* METHODS */
private:
    void getFullPicture();

public:
    bool addEdge(int src, int dest, double weight = 1, std::string service = "STANDARD", bool valid = true);
    Station& operator[](int index) override;
    void reduceConnectivity(const list<Edge*>& edgesToRemove);
    void restoreNetwork();

    std::list<std::pair<string, double>> getBusiestStations(int k);
    std::list<std::pair<string, double>> getBusiestDistricts(int k);
    std::list<std::pair<string, double>> getBusiestMunicipalities(int k);
    std::list<std::pair<int, int>> getBusiestStationPairs(double& maxTrains);

    double getIncomingTrains(int index, fort::utf8_table* table = nullptr);
    std::list<Path> getMinimumCostPaths(int src, int sink, double& maxTrains, double& minCost);
};

#endif //DA_TRAINS_RAILGRAPH_H
