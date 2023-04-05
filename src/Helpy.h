#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include <list>
#include <map>
#include <string>

#include "network/RailGraph.h"
#include "Reader.h"
#include "Utils.hpp"

using std::string;

class Helpy {
/* ATTRIBUTES */
private:
    Reader reader;
    RailGraph graph;
    uMap<string, int> stationIDs;

    uSet<string> districts, municipalities;
    uMap<string, string> districtStations, municipalityStations;

    // maps used to process commands
    static std::map<string, int> command, target, what;

/* CONSTRUCTOR */
public:
    Helpy();

/* METHODS */
private:
    void fetchData();
    static void properName(string& s);

    static string readInput(const string& instruction, uSet<string>& options);
    static double readNumber(const string& instruction);
    string readStation();
    string readCity();

    void advanced_mode();
    void guided_mode();
    bool process_command(string& s1, string& s2, string& s3);

    void displayOperatingMode();
    void displayBusiest(string& s);

    void changeOperatingMode();
    void chooseMaximumTrains();
    void calculateMaximumTrains();
    void calculateMaximumTrainsTwoStations();
    void determineMostTrains();
    void determineBudgetNeed();
    void determineAffectedStations();

public:
    void terminal();
};

#endif
