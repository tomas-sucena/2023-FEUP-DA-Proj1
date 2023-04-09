#ifndef DA_TRAINS_READER_H
#define DA_TRAINS_READER_H

#include "network/RailGraph.h"
#include "Utils.hpp"

using std::string;

class Reader {
/* ATTRIBUTES */
private:
    // for reading
    std::ifstream reader;
    string path;
    char valueDelim, lineDelim; // delimiters

    // data structures
    uMap<string, int> stationIDs;
    uMap<int, string> stationNames;
    uSet<int> railwaySources, railwaySinks;
    uMap<string, uSet<string>> districts, municipalities, trainLines;

/* CONSTRUCTOR */
public:
    explicit Reader(std::string path, char valueDelim = ',', char lineDelim = '\n');

/* METHODS */
private:
    void readStations(RailGraph& g);
    void readNetwork(RailGraph& g);

public:
    RailGraph read();
    std::string getPath() const;
    void setPath(std::string& path);

    uMap<std::string, int> getStationIDs() const;
    uMap<int, std::string> getStationNames() const;
    uSet<int> getRailwaySources() const;
    uSet<int> getRailwaySinks() const;
    uMap<string, uSet<string>> getDistricts() const;
    uMap<string, uSet<string>> getMunicipalities() const;
    uMap<string, uSet<string>> getTrainLines() const;
};

#endif //DA_TRAINS_READER_H
