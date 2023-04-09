#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include "network/RailGraph.h"
#include "Reader.h"
#include "Utils.hpp"

using std::string;

class Helpy {
/* ATTRIBUTES */
private:
    Reader reader;
    uMap<string, int> stationIDs;
    uMap<int, string> stationNames;

    RailGraph graph;
    uMap<string, uSet<string>> districts, municipalities, trainLines;

    // maps used to process commands
    static std::map<string, int> command, target, what;

/* CONSTRUCTOR */
public:
    Helpy();

/* METHODS */
private:
    void fetchData();

    static string readInput(const string& instruction, uSet<string>& options);
    static double readNumber(const string& instruction);
    static void readInputFromTable(std::list<std::pair<int,int>>& edges, std::vector<Edge*> ref, int station);
    string readStation();
    string readLocation(const string& instruction);
    string readLocation();

    void advanced_mode();
    void guided_mode();
    bool process_command(string& s1, string& s2, string& s3);

    std::vector<Edge*> printEdges(int station);
    void printPath(Path& p);
    double getTrainsBetweenStations(int src, int sink);

    // commands
    void displayAllStations();
    void displayDataDirectory();
    void displayBusiest(string& s);
    void displayBusiestPairs();
    void displayRailwaySources();
    void displayRailwaySinks();

    void changeDataDirectory();
    void calculateMaximumTrains();
    void determineAffectedStations();
    void changeRailwayNetwork();
    void changeRailwaySources();
    void changeRailwaySinks();

public:
    void terminal();
};

#endif
