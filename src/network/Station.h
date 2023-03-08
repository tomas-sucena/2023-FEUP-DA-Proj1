#ifndef DA_TRAINS_STATION_H
#define DA_TRAINS_STATION_H

#include <string>

#include "implementation/Vertex.h"

using std::string;

class Station : public Vertex {
    string name;
    string district;
    string municipality;
    string township;
    string line;

public:
    // constructor
    Station(string& name, string& district, string& municipality, string& township, string& line)
        : name(name), district(district), municipality(municipality), township(township), line(line) {}

    // methods
    string getName() const{
        return name;
    }

    string getDistrict() const{
        return district;
    }

    string getMunicipality() const{
        return municipality;
    }

    string getTownship() const{
        return township;
    }

    string getLine() const{
        return line;
    }
};

#endif //DA_TRAINS_STATION_H
