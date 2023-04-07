#ifndef DA_TRAINS_STATION_HPP
#define DA_TRAINS_STATION_HPP

#include <string>

#include "implementation/Vertex.hpp"

using std::string;

class Station : public Vertex {
    friend class RailGraph;

/* ATTRIBUTES */
private:
    string name;
    string district;
    string municipality;
    string township;
    string line;

/* CONSTRUCTOR */
public:
    Station(string& name, string& district, string& municipality, string& township, string& line)
        : name(name), district(district), municipality(municipality), township(township), line(line) {}

/* METHODS */
public:
    Vertex* clone() const override{
        return new Station(*this);
    }

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

#endif //DA_TRAINS_STATION_HPP
