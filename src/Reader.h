#ifndef DA_TRAINS_READER_H
#define DA_TRAINS_READER_H

#include "network/RailGraph.h"
#include "Utils.hpp"

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
    void setPath(std::string& path);

    uMap<std::string, int> getStations() const;
    uSet<int> getNetworkSources() const;
    uSet<int> getNetworkSinks() const;
};

#endif //DA_TRAINS_READER_H
