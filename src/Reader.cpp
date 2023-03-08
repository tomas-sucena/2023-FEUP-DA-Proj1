#include "Reader.h"

#include <sstream>

#include "network/Station.h"

using std::string;

/**
 * @brief creates a Reader object
 * @param path the path to the directory where the files to be read are
 */
Reader::Reader(string path) {
    if (path.back() != '/'){
        path += '/';
    }

    this->path = path;
}

/**
 * @brief reads all the files
 */
void Reader::read(){
    readStations();
    readNetwork();
}

/**
 * @brief reads the file which contains information about the Stations
 */
void Reader::readStations(){
    reader.open(path + "stations.csv");

    string line;
    getline(reader, line); // header

    for (int i = 1; getline(reader, line); ++i){
        std::istringstream line_(line);

        // read the name
        string name;
        getline(reader, name, ',');

        ids[name] = i;
        
        // read the district
        string district;
        getline(reader, district, ',');
        
        // read the municipality
        string municipality;
        getline(reader, municipality, ',');

        // read the township
        string township;
        getline(reader, township, ',');

        // read the train line
        string trainLine;
        getline(reader, trainLine, '\r');

        auto station = new Station(name, district, municipality, township, line);
    }

    reader.close();
    reader.clear();
}

/**
 * @brief reads the file which contains information about the train network
 */
void Reader::readNetwork(){
    reader.open(path + "stations.csv");
}
