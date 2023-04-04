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
/* ATTRIBUTES */
private:
    // for reading
    std::ifstream reader;
    std::string path;
    char valueDelim, lineDelim; // delimiters

    // data structures
    uMap<std::string, int> stationIDs;
    uSet<int> networkSources, networkSinks;

/* CONSTRUCTOR */
public:
    explicit Reader(std::string path, char valueDelim = ',', char lineDelim = '\n');

/* METHODS */
private:
    void readStations(RailGraph& g);
    void readNetwork(RailGraph& g);

public:
    RailGraph read();
    uMap<std::string, int> getStations();
};

#endif //DA_TRAINS_READER_H
