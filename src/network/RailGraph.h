#ifndef DA_TRAINS_RAILGRAPH_H
#define DA_TRAINS_RAILGRAPH_H

#include <list>
#include <string>

#include "../Utils.hpp"
#include "Railway.hpp"
#include "Station.hpp"
#include "UGraph.h"

class RailGraph : public UGraph {
    friend class Helpy;

    int superSourceID, superSinkID;
    bool profitMode;
    uMap<std::string, std::list<Railway*>> districts, municipalities;

public:
    // constructor
    explicit RailGraph(int n = 0);

    // methods
    bool addEdge(int src, int dest, double weight = 1, std::string service = "STANDARD", bool valid = true);
    Station& operator[](int index) override;
    int addSuperSource(), addSuperSink();
    bool addSource(int sourceID), addSink(int sinkID);

    double getFullPicture();
    std::list<std::pair<int, int>> getBusiestStationPairs(double& maxFlow);
    RailGraph subGraph(const list<std::pair<int, int>>& edgesList);
    double reducedConnectivity(int start, int end, RailGraph sub);
    std::vector<std::pair<int,int>> mostAffected(RailGraph sub, int k);
    std::list<string> selectFunction(std::string s, int k);
    std::list<string> getBusiestDistricts(int k);
};

#endif //DA_TRAINS_RAILGRAPH_H
