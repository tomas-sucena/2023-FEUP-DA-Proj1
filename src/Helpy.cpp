#include "Helpy.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "libfort/fort.hpp"

// output colors
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

// line breaks
#define BREAK   std::endl << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << RESET << std::endl << std::endl

std::map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2},
                                        {"determine", 2}, {"change", 3}, {"switch", 3}, {"toggle", 3}};

std::map<string, int> Helpy::target = {{"all", 4}, {"station", 6}, {"shortest", 8}, {"maximum", 10},
                                       {"most", 12}, {"budget", 14}, {"affected", 17}, {"operating", 19},
                                       {"busiest", 21}};

std::map<string, int> Helpy::what = {{"information", 24}, {"info", 24}, {"route", 27},{"routes", 27},
                                     {"train", 27}, {"trains", 27}, {"pair", 27}, {"pairs", 27},
                                     {"station", 29},{"stations", 29}, {"district", 29},{"districts", 29},
                                     {"municipality", 29},{"municipalities", 29}, {"need", 31}, {"mode", 33}};

/**
 * @brief takes a user inputted string and modifies it so that it becomes well-written
 * @complexity O(n)
 * @param s string to be modified
 */
void Helpy::properName(string& s){
    string name;

    bool first = true;
    for (std::istringstream s_(s); s_ >> s;){
        s[0] = (char) toupper(s[0]);

        for (int i = 1; i < s.size(); i++){
            s[i] = (char) tolower(s[i]);
        }

        if (!first) name += ' ';

        name += s;
        first = false;
    }

    s = name;
}

/**
 * @brief Construct a new Helpy:: Helpy object
 * @param graph graph that contains all the data regarding Stations and Trips between stations
 * @param ids unordered map that contains information regarding stations (for search purposes)
 */
Helpy::Helpy() : reader("../data", ';') {
    fetchData();
}

/**
 * @brief reads and parses the data files
 */
void Helpy::fetchData() {
    // create the graph
    graph = reader.read();
    graph.networkSources = reader.getNetworkSources();
    graph.networkSinks = reader.getNetworkSinks();

    original = &graph;
    stationIDs = reader.getStations();
}

/**
 * @brief reads a line of user input
 * @complexity O(n^2)
 * @param instruction the instruction that will be displayed before prompting the user to type
 * @param options the options that will be displayed to the user
 * @return read input
 */
string Helpy::readInput(const string& instruction, uSet<string>& options){
    string res;
    bool valid = false;

    while (true){
        std::cout << BREAK;
        std::cout << instruction << std::endl << std::endl;

        string line; getline(std::cin >> std::ws, line);
        Utils::lowercase(line);

        std::istringstream line_(line);

        while (line_ >> res){
            if (options.find(res) == options.end())
                continue;

            valid = true;
            break;
        }

        if (valid) break;

        std::cout << BREAK;
        std::cout << RED << "Invalid command! Please, try again." << RESET << std::endl;
    }

    return res;
}

/**
 * @brief reads a number from the console
 * @complexity O(n^2)
 * @param instruction the instruction that will be displayed before prompting the user to input the number
 * @return the number inputted by the user
 */
double Helpy::readNumber(const string &instruction){
    double res;
    bool valid = false;

    while (true){
        std::cout << BREAK;
        std::cout << instruction << std::endl << std::endl;

        string line; getline(std::cin >> std::ws, line);
        Utils::lowercase(line);

        std::istringstream line_(line);

        string temp;
        while (line_ >> temp){
            try {
                res = stod(temp);
            }
            catch(...){
                continue;
            }

            valid = true;
            break;
        }

        if (valid) break;

        std::cout << BREAK;
        std::cout << RED << "Invalid input! Please, try again." << RESET << std::endl;
    }

    return res;
}

/**
 * @brief reads the name of a Station from the console
 * @complexity O(n)
 * @return the name of the Station
 */
string Helpy::readStation(){
    string station;

    while (true){
        std::cout << BREAK;
        std::cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << "station" << RESET << ":"
                  << std::endl << std::endl;

        getline(std::cin >> std::ws, station);
        Utils::lowercase(station);
        
        if (stationIDs.find(station) != stationIDs.end()){
            break;
        }

        std::cout << BREAK;
        std::cout << RED << "Invalid input! The station you entered does not exist. Please, try again." << RESET << std::endl;
    }

    return station;
}


/**
 * @brief allows the user to choose the mode of the UI
 * @complexity O(n^2)
 */
void Helpy::terminal(){
    string instruction = "Which mode would you prefer?\n\n"
                         "* Guided\n"
                         "* Advanced";
    uSet<string> options = {"guided", "advanced"};

    if (readInput(instruction, options) == "guided"){
        guided_mode();
    }
    else{
        advanced_mode();
    }
}

/**
 * @brief executes the advanced mode of the UI
 * @complexity O(1)
 */
void Helpy::advanced_mode(){
b1: std::cout << BREAK;
    std::cout << "How can I be of assistance?" << std::endl << std::endl;

    string s1, s2, s3;
    std::istringstream s_;

    std::cin >> s1; Utils::lowercase(s1);

    if (s1 == "quit" || s1 == "no" || s1 == "die"){
        goto e1;
    }

    std::cin >> s2;
    Utils::lowercase(s2);

    std::cin >> s3;
    Utils::lowercase(s3);

    if (!process_command(s1, s2, s3)){
        goto b1;
    }

    std::cout << BREAK;
    std::cout << "Anything else? (Yes/No)" << std::endl << std::endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    Utils::lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y"){
            goto b1;
        }
    }

e1: std::cout << BREAK;
    std::cout << "See you next time!" << std::endl << std::endl;
}

/**
 * @brief executes the guided mode of the UI
 * @complexity O(1)
 */
void Helpy::guided_mode(){
b2: std::cout << BREAK;
    std::cout << "Hello! How can I be of assistance?" << std::endl;
    std::cout << std::endl;
    std::cout << "* Display" << std::endl;
    std::cout << "* Calculate" << std::endl;
    std::cout << "* Determine" << std::endl;
    std::cout << "* Change" << std::endl;
    std::cout << std::endl;

    string s1, s2, s3;
    std::istringstream s_;

    std::cin >> s1; Utils::lowercase(s1);
    std::cout << BREAK;

    if (s1 == "display"){
        std::cout << "* All" << std::endl;
        std::cout << "* Busiest" << std::endl;
        std::cout << "* Operating" << std::endl;
    }
    else if (s1 == "calculate"){
        std::cout << "* Maximum" << std::endl;
        std::cout << "* Most" << std::endl;
    }
    else if (s1 == "determine"){
        std::cout << "* Affected" << std::endl;
        std::cout << "* Busiest" << std::endl;
    }
    else if (s1 == "change"){
        std::cout << "* Operating" << std::endl;
        std::cout << "* Busiest" << std::endl;
    }
    else if (s1 == "quit" || s1 == "die"){
        goto e2;
    }
    else { // error
        process_command(s1, s2, s3);
        goto b2;
    }

    std::cout << std::endl;

    std::cin >> s2; Utils::lowercase(s2);
    std::cout << BREAK;

    if (s2 == "all"){
        std::cout << "* Stations" << std::endl;
    }
    else if (s2 == "maximum" || s2 == "most"){
        std::cout << "* Trains" << std::endl;
    }
    else if (s2 == "affected"){
        std::cout << "* Stations" << std::endl;
    }
    else if (s2 == "busiest"){
        std::cout << "* Stations" << std::endl;
        std::cout << "* Districts" << std::endl;
        std::cout << "* Municipalities" << std::endl;
        std::cout << "* Pairs" << std::endl;
    }
    else if (s2 == "operating"){
        std::cout << "* Mode" << std::endl;
    }
    else if (s2 == "quit" || s2 == "die"){
        goto e2;
    }
    else{ // error
        process_command(s1, s2, s3);
        goto b2;
    }

    std::cout << std::endl;
    std::cin >> s3; Utils::lowercase(s3);

    if (s3 == "quit" || s3 == "die"){
        goto e2;
    }

    if (!process_command(s1, s2, s3)){
        goto b2;
    }

t2: std::cout << BREAK;
    std::cout << "Anything else? (Yes/No)" << std::endl << std::endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    Utils::lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y")
            goto b2;
    }

    std::cout << BREAK;
e2: std::cout << "See you next time!" << std::endl << std::endl;
}

/**
 * @brief processes the commands that were inputted
 * @complexity O(n^2 * log(n))
 * @param s1 first word of the command
 * @param s2 second word of the command
 * @param s3 third word of the command
 * @return 'true' if the command exists, 'false' otherwise
 */
bool Helpy::process_command(string& s1, string& s2, string& s3){
    switch (command[s1] + target[s2] + what[s3]){
        case (34) : {
            displayAllStations();
            break;
        }
        case (39) : {
            calculateMaximumTrains();
            break;
        }
        case (48) : {
            determineAffectedStations();
            break;
        }
        case (49) : {
            displayBusiestPairs();
            break;
        }
        case (51) : {
            displayBusiest(s3);
            break;
        }
        case (52) : {
            displayOperatingMode();
            break;
        }
        case(55) : {
            changeOperatingMode();
            break;
        }
        default : {
            std::cout << BREAK;
            std::cout << RED << "Invalid command! Please, type another command." << RESET << std::endl;

            return false;
        }
    }

    return true;
}

double Helpy::getIncomingTrains(int index, bool display){
    double flow = 0;
    graph.getFullPicture();

    // create the table
    fort::char_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"Station", "Service", "Trains"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    // compute the flow
    for (const Edge* e : graph[index].inEdges()){
        flow += e->getFlow();

        auto s = dynamic_cast<const Railway*>(e);
        if (s == nullptr) continue;

        table << graph[s->getSrc()].getName() << s->getService() << s->getFlow() << fort::endr;
    }

    // print the table
    if (display){
        std::cout << BREAK;
        std::cout << table.to_string();
    }

    return flow;
}

/**
 * @brief displays all the stations that are part of the railway network
 */
void Helpy::displayAllStations(){
    fort::char_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"N", "Name", "District", "Municipality", "Line"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    for(int i = 1; i < graph.getVertices().size(); i++){
        table << i << graph[i].getName() << graph[i].getDistrict() << graph[i].getMunicipality() << graph[i].getLine()
              << fort::endr;
    }

    std::cout << BREAK;
    std::cout << table.to_string();
}

/**
 * @brief displays the "company's" operating mode
 * @complexity O(n^2)
*/
void Helpy::displayOperatingMode(){
    string mode = (graph.profitMode) ? "Profit" : "Standard";
    std::cout << "The current " << BOLD << "operating mode" << RESET << " is " << YELLOW << mode << RESET << "." << std::endl;

    string instruction = "Would you like to change it?";
    uSet<string> options = {"yes", "no"};

    if (readInput(instruction, options) == "yes") changeOperatingMode();
}

/**
 * @brief computes the top-k stations/districts/municipalities with the most amount of trains circulating in them
 * @complexity O(n^2)
 * @param s what to display
*/
void Helpy::displayBusiest(string& s){
    std::ostringstream instr;
    instr << "Please enter the " << BOLD << "number" << RESET << " of " << YELLOW << s << RESET
          << " you would like to display:";

    int k = (int) readNumber(instr.str());

    std::cout << BREAK;

    // compute the top-k
    std::list<std::pair<string, double>> busiest;

    if (s == "station" || s == "stations"){
        busiest = graph.getBusiestStations(k);
        s = "stations";
    }
    else if (s == "district" || s == "districts"){
        busiest = graph.getBusiestDistricts(k);
        s = "districts";
    }
    else{
        busiest = graph.getBusiestMunicipalities(k);
        s = "municipalities";
    }

    // display the top-k
    fort::char_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    uMap<string, string> singular = {{"stations", "Station"}, {"districts", "District"},
                                     {"municipalities", "Municipality"}};
    std::list<string> columnNames = {"N", singular[s], "Trains"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    int i = 1;
    for(auto& p: busiest){
        properName(p.first);
        table << i++ << p.first << p.second << fort::endr;
    }

    std::cout << "These are the results of my search:" << std::endl << std::endl;
    std::cout << table.to_string();
}

/**
 * @brief computes the pairs of stations (if more than one) that require the most trains when taking full advantage of
 * the existing network capacity
 * @complexity O(|V|^3 * |E|^2)
 */
void Helpy::displayBusiestPairs(){
    std::cout << BREAK;
    std::cout << BLUE << "Loading..." << RESET << std::endl << std::endl;

    double flow = 0;
    std::list<std::pair<int,int>> busiestPairs = graph.getBusiestStationPairs(flow);

    fort::char_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"N", "StationA", "StationB", "Trains"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    int i = 1;
    for(auto& p: busiestPairs)
        table << i++ << graph[p.first].getName() << graph[p.second].getName() << flow << fort::endr;

    std::cout << "These are the busiest pairs of stations:" << std::endl << std::endl;
    std::cout << table.to_string();
}

/**
 * @brief changes the operating mode of the RailGraph
 * @complexity O(1)
*/
void Helpy::changeOperatingMode(){
    std::ostringstream instr;
    instr << "Please select the " << BOLD << "operating mode" << RESET << " you would like:\n\n"
          << "* Standard\n"
          << "* Profit";

    uSet<string> modes = {"standard", "profit"};
    string choice = readInput(instr.str(), modes);

    graph.profitMode = (choice == "profit");
}

/**
 * @brief calculates either the maximum amount of trains that can simultaneously travel between two stations or the
 * maximum amount of trains that simultaneously arrive at a station
 * @complexity O(|V| * |E|^2)
 */
void Helpy::calculateMaximumTrains(){
    while (true) {
        std::ostringstream instr;
        instr << "Please enter the " << BOLD << "number" << RESET << " of the option you desire:" << std::endl << std::endl
              << "1. Between two stations" << std::endl
              << "2. That can simultaneously arrive at a station";

        int n = (int) readNumber(instr.str());

        switch (n){
            case (1) : {
                int stationA = stationIDs[readStation()];
                int stationB = stationIDs[readStation()];

                std::cout << BREAK;
                std::cout << "The maximum number of trains that can simultaneously travel between "
                          << graph[stationA].getName() << " and " << graph[stationB].getName() << " is " << BOLD
                          << YELLOW << graph.maximumFlow(stationA, stationB) << RESET << '.' << std::endl;

                return;
            }
            case (2) : {
                int station = stationIDs[readStation()];
                double flow = getIncomingTrains(station, true);

                std::cout << BREAK;
                std::cout << "The maximum number of trains that can simultaneously arrive at " << graph[station].getName()
                          << " is " << BOLD << YELLOW << flow << RESET << '.' << std::endl;

                return;
            }
            default :
                break;
        }

        std::cout << BREAK;
        std::cout << RED << "Invalid number! Please, try again." << RESET << std::endl;
    }
}

/**
 * @brief determines the top-k most affected stations per considered segment removed from the graph
*/
void Helpy::determineAffectedStations(){
    //Railgraph sub = graph.reducedConnectivity();
    //sub.getFullPicture();
    //Iterar pelas stations ate encontrar as mais afetadas
    //Repetir este processo para cada edge a retirar (diria eu)


    //a função mostAffected já faz isso tudo...
    return;
}
