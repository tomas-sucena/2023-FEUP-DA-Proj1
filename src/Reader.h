#ifndef DA_TRAINS_READER_H
#define DA_TRAINS_READER_H

#include <fstream>
#include <string>
#include <unordered_map>

#include "network/RailGraph.h"

#define uMap std::unordered_map

class Reader {
    // for reading
    std::ifstream reader;
    std::string path;

    // data structures
    uMap<std::string, int> ids;

    void readStations(RailGraph& g);
    void readNetwork(RailGraph& g);

public:
    // constructor
    explicit Reader(std::string path);

    // methods
    RailGraph read();
};

#endif //DA_TRAINS_READER_H
