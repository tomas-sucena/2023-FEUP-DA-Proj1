#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include <list>
#include <map>
#include <string>
#include <unordered_set>

#include "network/RailGraph.h"
#include "Reader.h"

#define uSet std::unordered_set
#define uMap std::unordered_map

using std::string;

class Helpy {
    private:
        RailGraph graph;
        uMap<string, int> stationIDs;
        bool standard = false;

        // maps used to process commands
        static std::map<string, int> command, target, what;
               
    public:
        // constructor
        Helpy(RailGraph& graph, uMap<string, int>  stationIDs);

        // methods
        static void lowercase(string& s, bool uppercase = false);
        static void properName(string& s);

        static string readInput(const string& instruction, uSet<string>& options);
        static double readNumber(const string& instruction);
        int readStation();
        string readCity();

        template <typename T>
        void printAirports(T airports, const list<string>& columnNames);
        void printFlights(const string& airport);
        void printPath(const Path& p);

        void terminal();
        void advanced_mode();
        void guided_mode();
        bool process_command(string& s1, string& s2, string& s3);

        void changeOperatingMode();
        void displayOperatingMode();
        void chooseMaximumTrains();
        void calculateMaximumTrains();
        void calculateMaximumTrainsTwoStations();
        void determineMostTrains();
        void determineBudgetNeed();
        void determineAffectedStations();
};

#endif
