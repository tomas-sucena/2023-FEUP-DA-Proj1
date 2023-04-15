#include "Reader.h"

#include "network/Station.hpp"

/**
 * @brief creates a Reader object
 * @param path path to the directory where the files to be read are
 */
Reader::Reader(string path, char valueDelim, char lineDelim) : valueDelim(valueDelim), lineDelim(lineDelim) {
    setPath(path);
}

/**
 * @brief extracts the next value of a CSV line
 * @complexity 0(n)
 * @param lineIt iterator of the CSV line
 * @param value string where the extracted value will be placed
 * @param delim character that delimits the value to be read
 * @return value
 */
void Reader::extractValue(std::string::iterator& lineIt, std::string& value, char delim){
    // check if the value is delimited by quotation marks
    if (*lineIt == '"'){
        delim = '"';
        ++lineIt;
    }

    value.clear();
    for (; *lineIt != '\000' && *lineIt != delim; ++lineIt)
        value += *lineIt;

    if (delim == '"') ++lineIt; // skip the quotation mark
    ++lineIt; // skip the delimiter
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
        auto it = line.begin();

        // read the name
        string name;
        extractValue(it, name, valueDelim);

        // read the district
        string district;
        extractValue(it, district, valueDelim);

        // read the municipality
        string municipality;
        extractValue(it, municipality, valueDelim);

        // read the township
        string township;
        extractValue(it, township, valueDelim);

        // read the train line
        string trainLine;
        extractValue(it, trainLine, lineDelim);

        // check if the station is repeated
        string lowercaseName = name; Utils::lowercase(lowercaseName);
        if (!stationIDs.insert({lowercaseName, i}).second) continue;

        // add the station to the graph
        graph.addVertex(new Station(name, district, municipality, township, trainLine));
        stationNames[i] = name;

        railwaySources.insert(i);
        railwaySinks.insert(i);

        Utils::lowercase(district);
        districts[district].insert(name);

        Utils::lowercase(municipality);
        municipalities[municipality].insert(name);

        Utils::lowercase(trainLine);
        trainLines[trainLine].insert(name);
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
        auto it = line.begin();

        // read the first station
        string stationA;
        extractValue(it, stationA, valueDelim);

        Utils::lowercase(stationA);

        // read the second station
        string stationB;
        extractValue(it, stationB, valueDelim);

        Utils::lowercase(stationB);

        // read the capacity
        string capacity;
        extractValue(it, capacity, valueDelim);

        // read the service
        string service;
        extractValue(it, service, lineDelim);

        graph.addEdge(stationIDs[stationA], stationIDs[stationB], std::stod(capacity) / 2, service);

        railwaySources.erase(stationIDs[stationB]);
        railwaySinks.erase(stationIDs[stationA]);
    }

    reader.close();
    reader.clear();
}

/**
 * @brief reads the files that detail the railway network
 * @return directed graph which represents the railway network
 */
RailGraph Reader::read(){
    // reset the data structures
    stationIDs.clear();
    railwaySinks.clear(); railwaySources.clear();
    districts.clear(); municipalities.clear(); trainLines.clear();

    RailGraph graph;
    readStations(graph);
    readNetwork(graph);

    return graph;
}

/**
 * @brief returns the path to the directory where the data files are stored
 * @return path to the directory where the data files are stored
 */
std::string Reader::getPath() const{
    return path;
}

/**
 * @brief changes the path to the directory where the data files are stored
 * @param path path to the directory where the files to be read are
 */
void Reader::setPath(std::string& path){
    if (path.back() != '/')
        path += '/';

    this->path = path;
}


/**
 * @brief returns an unordered map which binds the name of a station to its index
 * @return unordered map which binds the name of a station to its index
 */
uMap<std::string, int> Reader::getStationIDs() const{
    return stationIDs;
}

/**
 * @brief returns an unordered map which binds the index of a station to its name
 * @return unordered map which binds the index of a station to its name
 */
uMap<int, std::string> Reader::getStationNames() const{
    return stationNames;
}

/**
 * @brief returns an unordered set containing the indices of the stations that are the railway network sources
 * @return unordered set containing the indices of the stations that are the railway network sources
 */
uSet<int> Reader::getRailwaySources() const{
    return railwaySources;
}

/**
 * @brief returns an unordered set containing the indices of the stations that are the railway network sinks
 * @return unordered set containing the indices of the stations that are the railway network sinks
 */
uSet<int> Reader::getRailwaySinks() const{
    return railwaySinks;
}

/**
 * @brief returns an unordered map which binds the name of a district to the names of the stations located there
 * @return unordered map which binds the name of a district to the names of the stations located there
 */
uMap<string, uSet<string>> Reader::getDistricts() const{
    return districts;
}

/**
 * @brief returns an unordered map which binds the name of a municipality to the names of the stations located there
 * @return unordered map which binds the name of a municipality to the names of the stations located there
 */
uMap<string, uSet<string>> Reader::getMunicipalities() const{
    return municipalities;
}

/**
 * @brief returns an unordered map which binds the name of a train line to the names of the stations it contains
 * @return unordered map which binds the name of a train line to the names of the stations it contains
 */
uMap<string, uSet<string>> Reader::getTrainLines() const{
    return trainLines;
}
