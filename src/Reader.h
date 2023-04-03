#ifndef DA_TRAINS_READER_H
#define DA_TRAINS_READER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "network/RailGraph.h"

#define uMap std::unordered_map
#define uSet std::unordered_set

class Reader {
    // for reading
    std::ifstream reader;
    std::string path;

    // data structures
    uMap<std::string, int> stations;
    uSet<int> networkSources, networkSinks;

    void readStations(RailGraph& g);
    void readNetwork(RailGraph& g);

public:
    // constructor
    explicit Reader(std::string path);

    // methods
    RailGraph read();
    uMap<std::string, int> getStations();
};

#endif //DA_TRAINS_READER_H
