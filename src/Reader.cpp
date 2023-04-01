#include "Reader.h"

#include <sstream>

#include "network/Station.hpp"

using std::string;

/**
 * @brief creates a Reader object
 * @param path the path to the directory where the files to be read are
 */
Reader::Reader(string path) {
    if (path.back() != '/')
        path += '/';

    this->path = path;
}

/**
 * @brief reads the files that detail the railway network
 * @return directed graph which represents the railway network
 */
RailGraph Reader::read(){
    RailGraph graph;
    readStations(graph);
    readNetwork(graph);

    return graph;
}

/**
 * @brief reads the file which contains information about the Stations
 * @param graph directed graph that will be modelled based on the read information
 */
void Reader::readStations(RailGraph& graph){
    reader.open(path + "stations.csv");

    string line;
    getline(reader, line); // header

    for (int i = 1; getline(reader, line); ++i){
        std::istringstream line_(line);

        // read the name
        string name;
        getline(line_, name, ',');

        ids[name] = i;

        // read the district
        string district;
        getline(line_, district, ',');

        // read the municipality
        string municipality;
        getline(line_, municipality, ',');

        // read the township
        string township;
        getline(line_, township, ',');

        // read the train line
        string trainLine;
        getline(line_, trainLine, '\n');

        // add the station to the graph
        auto station = new Station(name, district, municipality, township, line);
        graph.addVertex(station);

        networkSources.insert(i);
        networkSinks.insert(i);
    }
    
    reader.close();
    reader.clear();
}

/**
 * @brief reads the file which contains information about the train network
 * @param graph directed graph that will be modelled based on the read information
 */
void Reader::readNetwork(RailGraph& graph){
    reader.open(path + "network.csv");

    string line;
    getline(reader, line); // header

    while (getline(reader, line)){
        std::istringstream line_(line);

        // read the first station
        string stationA;
        getline(line_, stationA, ',');

        // read the second station
        string stationB;
        getline(line_, stationB, ',');

        // read the capacity
        string capacity;
        getline(line_, capacity, ',');

        // read the service
        string service;
        getline(line_, service, '\n');

        graph.addEdge(ids[stationA], ids[stationB], std::stod(capacity), service);

        networkSources.erase(ids[stationB]);
        networkSinks.erase(ids[stationA]);
    }

    // add the super source
    graph.addSuperSource();

    for (int sourceID : networkSources)
        graph.addSource(sourceID);
    
    // add the super sink
    graph.addSuperSink();

    for (int sinkID : networkSinks)
        graph.addSink(sinkID);

    reader.close();
    reader.clear();
}
