#include "Reader.h"

#include <sstream>
#include <string>

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
void Reader::readAll(){
    readStations();
    readNetworks();
}

/**
 * @brief reads the file which contains information about the Stations
 */
void Reader::readStations(){
    reader.open(path + "stations.csv");

    string line;
    getline(reader, line); // header

    while (getline(reader, line)){
        std::istringstream line_(line);

        // read the name
        string name;
        getline(reader, name, ',');
        
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
    }

    reader.close();
    reader.clear();
}
