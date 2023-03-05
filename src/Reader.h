#ifndef DA_TRAINS_READER_H
#define DA_TRAINS_READER_H

#include <fstream>
#include <string>
#include <unordered_map>

#include "../graph/src/DGraph.h"

#define uMap std::unordered_map

class Reader {
    // for reading
    std::ifstream reader;
    std::string path;

    // data structures
    uMap<std::string, int> ids;

public:
    // constructor
    explicit Reader(std::string path);

    // methods
    void readAll();
    void readStations();
    void readNetworks();
};

#endif //DA_TRAINS_READER_H
