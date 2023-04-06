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
    bool profitMode, fullPicture;

    // for searching
    std::vector<std::string> stationNames;
    uSet<int> networkSources, networkSinks;
    uMap<std::string, std::list<Railway*>> districtRailways, municipalityRailways;

/* CONSTRUCTOR */
public:
    explicit RailGraph(int n = 0);

/* METHODS */
private:
    void getFullPicture();

public:
    void addVertex(Vertex* v = nullptr) override;
    bool addEdge(int src, int dest, double weight = 1, std::string service = "STANDARD", bool valid = true);
    Station& operator[](int index) override;

    std::list<std::pair<int, int>> getBusiestStationPairs(double& maxFlow);
    RailGraph subGraph(const list<std::pair<int, int>>& edgesList);
    double reducedConnectivity(int start, int end, RailGraph sub);
    std::vector<std::pair<int,int>> mostAffected(int k);

    std::list<std::pair<string, double>> getBusiestStations(int k);
    std::list<std::pair<string, double>> getBusiestDistricts(int k);
    std::list<std::pair<string, double>> getBusiestMunicipalities(int k);
};

#endif //DA_TRAINS_RAILGRAPH_H
