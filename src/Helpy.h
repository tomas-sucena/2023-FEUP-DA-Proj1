#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include <map>
#include <unordered_set>
#include <string>

#include "network/RailGraph.h"
#include "Reader.h"

using namespace std;


class Helpy {
    private:
        RailGraph graph;
        uMap<string, int> stations;
        bool standard = false;

        // maps used to process commands
        static std::map<string, int> command, target, what;
               
    public:
        // constructor
        Helpy(RailGraph& graph, uMap<string, int> ids);

        // methods
        static void lowercase(string& s, bool uppercase = false);
        static void properName(string& s);

        string readInput(const string& instruction, uSet<string>& options);
        double readNumber(const string& instruction);
        int readStation();
        string readCity(bool country = false);
        string readCoordinates();
        string readLocation(string instruction = "");

        template <typename T>
        void printAirports(T airports, const list<string>& columnNames);
        void printFlights(const string& airport);
        void printPath(const Path& p);

        void terminal();
        void advanced_mode();
        void guided_mode();
        bool process_command(string& s1, string& s2, string& s3);

        void chooseMaximumTrains();
        void calculateMaximumTrains();
        void calculateMaximumTrainsTwoStations();
        void determineMostTrains();
        void determineBudgetNeed();
        void determineAffectedStations();
};

#endif
