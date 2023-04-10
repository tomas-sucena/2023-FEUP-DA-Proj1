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
    /**
     * @brief creates a new Station object
     * @param name name of the station
     * @param district district where the station is located
     * @param municipality municipality where the station is located
     * @param township township where the station is located
     * @param line train line that the station belongs to
     */
    Station(string& name, string& district, string& municipality, string& township, string& line)
        : name(name), district(district), municipality(municipality), township(township), line(line) {}

/* METHODS */
public:
    /**
     * @brief creates a copy of the Station
     * @return pointer to a copy of the Station
     */
    Vertex* clone() const override{
        return new Station(*this);
    }

    /**
     * @brief returns the name of the station
     * @return name of the station
     */
    string getName() const{
        return name;
    }

    /**
     * @brief returns the district
     * @return district where the station is located
     */
    string getDistrict() const{
        return district;
    }

    /**
     * @brief returns the municipality where the station is located
     * @return municipality where the station is located
     */
    string getMunicipality() const{
        return municipality;
    }

    /**
     * @brief returns the township where the station is located
     * @return township where the station is located
     */
    string getTownship() const{
        return township;
    }

    /**
     * @brief returns the train line that the station belongs to
     * @return train line that the station belongs to
     */
    string getLine() const{
        return line;
    }
};

#endif //DA_TRAINS_STATION_HPP
