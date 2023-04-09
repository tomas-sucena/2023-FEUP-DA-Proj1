#include "Helpy.h"

using std::cout;
using std::endl;

// output colors
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

// text
#define DASHED_LINE "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
#define BREAK       endl << YELLOW << DASHED_LINE << RESET << endl << endl
#define YES_NO      " (" << GREEN << "Yes" << RESET << '/' << RED << "No" << RESET << ')'

std::map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2},
                                        {"calc", 2}, {"determine", 2}, {"change", 3}, {"switch", 3}};

std::map<string, int> Helpy::target = {{"all", 4}, {"data", 6}, {"maximum", 8},{"max", 8}, {"affected", 10},
                                       {"operating", 12}, {"busiest", 14}, {"railway",16}};

std::map<string, int> Helpy::what = {{"directory", 17}, {"dir", 17}, {"train", 20}, {"trains", 20},
                                     {"station", 23},{"stations", 23},{"district", 23},{"districts", 23},
                                     {"municipality", 23},{"municipalities", 23}, {"pairs", 26}, {"pair", 26},
                                     {"sources", 30}, {"source", 30},{"sinks", 33},{"sink", 33},
                                     {"network", 36}};

/**
 * @brief creates a new Helpy object
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
    graph.railwaySources = reader.getRailwaySources();
    graph.railwaySinks = reader.getRailwaySinks();

    // fill the data structures
    stationIDs = reader.getStationIDs();
    stationNames = reader.getStationNames();
    districts = reader.getDistricts();
    municipalities = reader.getMunicipalities();
    trainLines = reader.getTrainLines();
}

/**
 * @brief reads a line of user input
 * @param instruction the instruction that will be displayed before prompting the user to type
 * @param options the options that will be displayed to the user
 * @return read input
 */
string Helpy::readInput(const string& instruction, uSet<string>& options){
    string res;
    bool valid = false;

    while (true){
        cout << BREAK;
        cout << instruction << endl << endl;

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

        cout << BREAK;
        cout << RED << "Invalid command! Please, try again." << RESET << endl;
    }

    return res;
}

/**
 * @brief reads a number from the console
 * @param instruction the instruction that will be displayed before prompting the user to input the number
 * @return the number inputted by the user
 */
double Helpy::readNumber(const string &instruction){
    double res;
    bool valid = false;

    while (true){
        cout << BREAK;
        cout << instruction << endl << endl;

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

        cout << BREAK;
        cout << RED << "Invalid input! Please, try again." << RESET << endl;
    }

    return res;
}

void Helpy::readInputFromTable(std::list<std::pair<int,int>>& edges, std::vector<Edge*> ref, int station){
    int size = (int) ref.size();
    std::cout << std::endl << YELLOW << BREAK << RESET;
    std::cout << "Please type the " << BOLD << "indexes" << RESET << " of the " << YELLOW << "edges" << RESET << " you would like to " << RED << "remove"
              << RESET << ", separated by a comma (ex: 0,1,2,7,...).\n";

    // countries to USE
    string line; getline(std::cin, line);
    line += ",";

    std::istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        int k = std::stoi(temp);
        if (k > size) continue;

        edges.emplace_back(station, ref[k]->getDest());
    }
}

/**
 * @brief reads the name of a Station from the console
 * @complexity O(n)
 * @return the name of the Station
 */
string Helpy::readStation(){
    string station;

    while (true){
        cout << BREAK;
        cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << "station" << RESET << ":"
             << endl << endl;

        getline(std::cin >> std::ws, station);
        Utils::lowercase(station);
        
        if (stationIDs.find(station) != stationIDs.end()){
            break;
        }

        cout << BREAK;
        cout << RED << "Invalid input! The station you entered does not exist. Please, try again." << RESET << endl;
    }

    return station;
}

/**
 * @brief reads a location, prints the stations that are situated there and prompts the user to choose one of them
 * @param instruction instruction that will be displayed in the console
 * @return name of the station chosen by the user
 */
string Helpy::readLocation(const string& instruction){
    uSet<string> options = {"name", "district", "municipality", "line"};
    string choice = readInput(instruction, options);

    if (choice == "name")
        return readStation();

    uMap<string, uSet<string>> locations;
    if (choice == "district"){
        locations = districts;
    }
    else if (choice == "municipality"){
        locations = municipalities;
    }
    else{
        choice = "train line";
        locations = trainLines;
    }

    // read the station
    string station;

    while (true){
        cout << BREAK;
        cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << choice << RESET << ':'
             << endl << endl;

        string line; getline(std::cin >> std::ws, line);
        Utils::lowercase(line);

        if (locations.find(line) == locations.end()){
            cout << BREAK;
            cout << RED << "Invalid input! The " << choice << " you entered does not exist. "
                 << "Please, try again." << RESET << endl;

            continue;
        }

        uSet<string> stations = locations[line];

        if (stations.size() == 1){
            station = *stations.begin();
            break;
        }

        Utils::properName(line);

        cout << BREAK;
        cout << BOLD << line << RESET << " has the following " << YELLOW << "stations" << RESET << ':' << endl << endl;

        for (const string& s : stations)
            cout << "* " << s << endl;

        station = readStation();
        break;
    }

    return station;
}

/**
 * @brief reads a location, prints the stations that are situated there and prompts the user to choose one of them
 * @return name of the station chosen by the user
 */
string Helpy::readLocation(){
    std::ostringstream instr;
    instr << "With which of the following would you like to define the desired " << YELLOW << "station"
          << RESET << "?" << endl << endl
          << "* Name" << endl
          << "* District" << endl
          << "* Municipality" << endl
          << "* Line";

    return readLocation(instr.str());
}

/**
 * @brief executes the advanced mode of the UI
 */
void Helpy::advanced_mode(){
b1: cout << BREAK;
    cout << "How can I be of assistance?" << endl << endl;

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

    cout << BREAK;
    cout << "Anything else?" << YES_NO << endl << endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    Utils::lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y"){
            goto b1;
        }
    }

e1: cout << BREAK;
    cout << "See you next time!" << endl << endl;
}

/**
 * @brief executes the guided mode of the UI
 */
void Helpy::guided_mode(){
b2: cout << BREAK;
    cout << "Hello! How can I be of assistance?" << endl;
    cout << endl;
    cout << "* Display" << endl;
    cout << "* Calculate" << endl;
    cout << "* Determine" << endl;
    cout << "* Change" << endl;
    cout << endl;

    string s1, s2, s3;
    std::istringstream s_;

    std::cin >> s1; Utils::lowercase(s1);

    if (s1 == "display"){
        cout << BREAK;
        cout << "* All" << endl;
        cout << "* Busiest" << endl;
        cout << "* Data" << endl;
        cout << "* Railway" << endl;
    }
    else if (s1 == "calculate"){
        cout << BREAK;
        cout << "* Maximum" << endl;
    }
    else if (s1 == "determine"){
        cout << BREAK;
        cout << "* Affected" << endl;
    }
    else if (s1 == "change"){
        cout << BREAK;
        cout << "* Data" << endl;
        cout << "* Railway" << endl;
    }
    else if (s1 == "quit" || s1 == "die"){
        goto e2;
    }
    else { // error
        process_command(s1, s2, s3);
        goto b2;
    }

    cout << endl;

    std::cin >> s2; Utils::lowercase(s2);

    if (s2 == "all" || s2 == "affected"){
        cout << BREAK;
        cout << "* Stations" << endl;
    }
    else if (s2 == "data"){
        cout << BREAK;
        cout << "* Directory" << endl;
    }
    else if (s2 == "maximum"){
        cout << BREAK;
        cout << "* Trains" << endl;
    }
    else if (s2 == "busiest"){
        cout << BREAK;
        cout << "* Stations" << endl;
        cout << "* Districts" << endl;
        cout << "* Municipalities" << endl;
        cout << "* Pairs" << endl;
    }
    else if (s2 == "railway"){
        cout << BREAK;
        cout << "* Network" << endl;
        cout << "* Sources" << endl;
        cout << "* Sinks" << endl;
    }
    else if (s2 == "quit" || s2 == "die"){
        goto e2;
    }
    else{ // error
        process_command(s1, s2, s3);
        goto b2;
    }

    cout << endl;
    std::cin >> s3; Utils::lowercase(s3);

    if (s3 == "quit" || s3 == "die"){
        goto e2;
    }

    if (!process_command(s1, s2, s3)){
        goto b2;
    }

    cout << BREAK;
    cout << "Anything else?" << YES_NO << endl << endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    Utils::lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y")
            goto b2;
    }

    cout << BREAK;
e2: cout << "See you next time!" << endl << endl;
}

/**
 * @brief processes the commands that were inputted
 * @param s1 first word of the command
 * @param s2 second word of the command
 * @param s3 third word of the command
 * @return 'true' if the command exists, 'false' otherwise
 */
bool Helpy::process_command(string& s1, string& s2, string& s3){
    switch (command[s1] + target[s2] + what[s3]){
        case (24) : {
            displayDataDirectory();
            break;
        }
        case (26) : {
            changeDataDirectory();
            break;
        }
        case (28) : {
            displayAllStations();
            break;
        }
        case (30) : {
            calculateMaximumTrains();
            break;
        }
        case (35) : {
            determineAffectedStations();
            break;
        }
        case (38) : {
            displayBusiest(s3);
            break;
        }
        case (41) : {
            displayBusiestPairs();
            break;
        }
        case (47) : {
            displayRailwaySources();
            break;
        }
        case (49) : {
            changeRailwaySources();
            break;
        }
        case (50) : {
            displayRailwaySinks();
            break;
        }
        case (52) : {
            changeRailwaySinks();
            break;
        }
        case (55) : {
            changeRailwayNetwork();
            break;
        }
        default : {
            cout << BREAK;
            cout << RED << "Invalid command! Please, type another command." << RESET << endl;

            return false;
        }
    }

    return true;
}

/**
 * @brief prints the outgoing edges of a station
 * @param station index of the station
*/
std::vector<Edge*> Helpy::printEdges(int station){
    fort::utf8_table table = Utils::createTable({"N", "Source", "Destination"});

    int i = 0;
    std::vector<Edge*> edges;
    for(auto e: graph[station].outEdges()){
        if (!e->valid) continue;

        edges.push_back(e);
        table << i++ << graph[station].getName() << graph[e->getDest()].getName() << fort::endr;
    }

    std::cout << table.to_string();
    return edges;
}

/**
 * @brief prints a table with the information of a path
 * @complexity O(|E|)
 * @param p path to be printed
 */
void Helpy::printPath(Path& p){
    fort::utf8_table table = Utils::createTable({"N", "Source", "Destination", "Service", "Capacity"});

    int i = 1;
    for (const Edge* e : p){
        auto r = (Railway*) e;
        table << i++ << stationNames[r->getSrc()] << stationNames[r->getDest()] << r->getService() << r->getWeight()
              << fort::endr;
    }

    cout << table.to_string();
}

/**
 * @brief computes the maximum amount of trains that can simultaneously travel between two stations
 * @complexity O(|V| * |E|^2)
 * @param src index of the source station
 * @param sink index of the sink station
 * @return maximum amount of trains that can simultaneously travel between the stations
 */
double Helpy::getTrainsBetweenStations(int src, int sink){
    std::ostringstream instr;
    instr << "Would you like to consider " << BOLD << "only" << RESET << " the paths that ensure " << YELLOW
          << "minimum cost" << RESET << " for the company?" << YES_NO;

    uSet<string> options = {"yes", "no"};
    string res = readInput(instr.str(), options);

    // compute the max flow
    double maxTrains, minCost = 0;
    std::list<Path> paths;

    if (res == "no")
        maxTrains = graph.maximumFlow(src, sink, &paths);
    else
        paths = graph.getMinimumCostPaths(src, sink, maxTrains, minCost);

    // display the augmenting paths
    cout << BREAK;
    cout << "These are the results of my search:" << endl;

    int optionNum = 1;
    for (Path& p : paths){
        cout << endl << BOLD << YELLOW << "PATH #" << optionNum++ << RESET << endl;
        printPath(p);
    }

    cout << endl;
    if (res == "yes"){
        cout << YELLOW << "* Minimum cost: " << RESET << minCost << "€" << endl;
        cout << YELLOW << "* Total cost: " << RESET << minCost * maxTrains << "€" << endl << endl;
    }

    return maxTrains;
}

/**
 * @brief displays all the stations that are part of the railway network
 * @complexity O(|V|)
 */
void Helpy::displayAllStations(){
    fort::utf8_table table = Utils::createTable({"N", "Name", "District", "Municipality", "Township", "Line"});

    for (int i = 1; i <= graph.countVertices(); i++){
        table << i << graph[i].getName() << graph[i].getDistrict() << graph[i].getMunicipality() << graph[i].getTownship()
              << graph[i].getLine() << fort::endr;
    }

    cout << BREAK;
    cout << "The railway network has the following " << YELLOW << "stations" << RESET << ':' << endl << endl;
    cout << table.to_string();
}

/**
 * @brief displays the directory where the data files are stored
 */
void Helpy::displayDataDirectory(){
    cout << BREAK;
    cout << "The current data directory is " << BOLD << YELLOW << reader.getPath() << RESET << '.' << endl;
}

/**
 * @brief computes the top-k stations/districts/municipalities with the most amount of trains circulating in them
 * @complexity O(|V| * |E|^2)
 * @param s what to display
*/
void Helpy::displayBusiest(string& s){
    std::ostringstream instr;
    instr << "Please enter the " << BOLD << "number" << RESET << " of " << YELLOW << s << RESET
          << " you would like to display:";

    int k = (int) readNumber(instr.str());

    cout << BREAK;

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
    fort::utf8_table table;

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
        Utils::properName(p.first);
        table << i++ << p.first << p.second << fort::endr;
    }

    cout << "These are the results of my search:" << endl << endl;
    cout << table.to_string();
}

/**
 * @brief computes the pairs of stations (if more than one) that require the most trains when taking full advantage of
 * the existing network capacity
 * @complexity O(|V|^3 * |E|^2)
 */
void Helpy::displayBusiestPairs(){
    cout << BREAK;
    cout << BLUE << "Loading..." << RESET << endl << endl;

    double flow = 0;
    std::list<std::pair<int,int>> busiestPairs = graph.getBusiestStationPairs(flow);

    fort::utf8_table table = Utils::createTable({"N", "StationA", "StationB", "Trains"});

    int i = 1;
    for(auto& p: busiestPairs)
        table << i++ << graph[p.first].getName() << graph[p.second].getName() << flow << fort::endr;

    cout << "These are the busiest pairs of stations:" << endl << endl;
    cout << table.to_string();
}

/**
 * @brief displays the stations which are the sources of the railway network
 * @complexity O(|V|)
 */
void Helpy::displayRailwaySources(){
    fort::utf8_table table = Utils::createTable({"N", "Station"});

    std::list<string> railwaySources;
    for (int i : graph.railwaySources)
        railwaySources.push_back(stationNames[i]);

    std::ostringstream instr;
    instr << "Would you like to order the stations alphabetically?" << YES_NO;

    uSet<string> options = {"yes", "no"};
    string res = readInput(instr.str(), options);

    if (res == "yes") railwaySources.sort();

    for (int i = 1; !railwaySources.empty(); railwaySources.pop_front())
        table << i++ << railwaySources.front() << fort::endr;

    cout << BREAK;
    cout << "The railway network has the following " << BOLD << YELLOW << "sources" << RESET << ':' << endl << endl;
    cout << table.to_string();
}

/**
 * @brief displays the stations which are the sinks of the railway network
 * @brief 0(|V|)
 */
void Helpy::displayRailwaySinks(){
    fort::utf8_table table = Utils::createTable({"N", "Station"});

    std::list<string> railwaySinks;
    for (int i : graph.railwaySinks)
        railwaySinks.push_back(stationNames[i]);

    std::ostringstream instr;
    instr << "Would you like to order the stations alphabetically?" << YES_NO;

    uSet<string> options = {"yes", "no"};
    string res = readInput(instr.str(), options);

    if (res == "yes") railwaySinks.sort();

    for (int i = 1; !railwaySinks.empty(); railwaySinks.pop_front())
        table << i++ << railwaySinks.front() << fort::endr;

    cout << BREAK;
    cout << "The railway network has the following " << BOLD << YELLOW << "sinks" << RESET << ':' << endl << endl;
    cout << table.to_string();
}

/**
 * @brief changes the directory where the Reader will fetch the data files
 */
void Helpy::changeDataDirectory(){
    cout << BREAK;
    cout << "Please type the " << BOLD << "relative path" << RESET << " to the directory where the "
              << BOLD << YELLOW << "data files" << RESET << " are located:" << endl << endl;

    string path;
    getline(std::cin >> std::ws, path);

    reader.setPath(path);
    fetchData();
}

/**
 * @brief calculates either the maximum amount of trains that can simultaneously travel between two stations OR the
 * maximum amount of trains that simultaneously arrive at a station
 * @complexity O(|V| * |E|^2)
 */
void Helpy::calculateMaximumTrains(){
    while (true) {
        std::ostringstream instr;
        instr << "Please enter the " << BOLD << "number" << RESET << " of the option you desire:" << endl << endl
              << "1. Between two stations" << endl
              << "2. That can simultaneously arrive at a station";

        int n = (int) readNumber(instr.str());
        double maxTrains;

        switch (n){
            case (1) : {
                // source
                instr.clear(); instr.str("");
                instr << "With which of the following would you like to define the " << BOLD << YELLOW << "source"
                      << RESET << "?" << endl << endl
                      << "* Name" << endl
                      << "* District" << endl
                      << "* Municipality" << endl
                      << "* Line";

                int stationA = stationIDs[readLocation(instr.str())];

                // destination
                instr.clear(); instr.str("");
                instr << "With which of the following would you like to define the " << BOLD << YELLOW << "destination"
                      << RESET << "?" << endl << endl
                      << "* Name" << endl
                      << "* District" << endl
                      << "* Municipality" << endl
                      << "* Line";

                int stationB = stationIDs[readLocation(instr.str())];

                maxTrains = getTrainsBetweenStations(stationA, stationB);
                cout << "The maximum number of trains that can simultaneously travel between " << BOLD
                     << stationNames[stationA] << RESET << " and " << BOLD << stationNames[stationB] << RESET
                     << " is " << BOLD << YELLOW << maxTrains << RESET << '.' << endl;

                return;
            }
            case (2) : {
                int station = stationIDs[readLocation()];

                fort::utf8_table table;
                maxTrains = graph.getIncomingTrains(station, &table);

                cout << BREAK;
                cout << "These are all the stations that are connected to " << BOLD << stationNames[station] << RESET
                     << ':' << endl << endl;

                cout << table.to_string() << endl;
                cout << "The maximum number of trains that can simultaneously arrive at " << BOLD << stationNames[station]
                     << RESET << " is " << BOLD << YELLOW << maxTrains << RESET << '.' << endl;

                return;
            }
            default :
                break;
        }

        cout << BREAK;
        cout << RED << "Invalid number! Please, try again." << RESET << endl;
    }
}

/**
 * @brief determines the top-k most affected stations per considered segment removed from the graph
 * @complexity O(|V| * |E|^2)
*/
void Helpy::determineAffectedStations(){
    std::ostringstream instr;
    instr << "Please enter the " << BOLD << "number" << RESET << " of " << YELLOW << "stations" << RESET
          << " you would like to display:";

    int k = (int) readNumber(instr.str());

    // compute the flow in the reduced network
    std::vector<std::pair<int, std::pair<double, double>>> beforeAndAfter;

    for(int i = 1; i <= graph.countVertices(); i++)
        beforeAndAfter.push_back({i, {0, graph.getIncomingTrains(i)}});

    // compute the flow in the original network
    std::list<Edge*> removedEdges = graph.underMaintenance;
    graph.restoreNetwork();

    for(int i = 1; i <= graph.countVertices(); i++)
        beforeAndAfter[i].second.first = graph.getIncomingTrains(i);

    graph.reduceConnectivity(removedEdges);

    // order the results
    std::sort(beforeAndAfter.begin(), beforeAndAfter.end(),[](auto &lhs, auto &rhs) {
        return std::abs(lhs.second.first - lhs.second.second) > std::abs(rhs.second.first - rhs.second.second);
    });

    beforeAndAfter.resize(k);

    // create the table
    fort::utf8_table table = Utils::createTable({"N", "Station","Before", "After"});

    int i = 1;
    for (auto& p: beforeAndAfter)
        table << i++ << graph[p.first].getName() << p.second.first << p.second.second << fort::endr;

    cout << table.to_string();
}

/**
 * @brief changes the considered railway network while preserving the original network
*/
void Helpy::changeRailwayNetwork(){
    // check if the user is already working with a reduced network
    if (!graph.underMaintenance.empty()){
        std::ostringstream instr;
        instr << "It seems that you are currently working with a railway network of " << YELLOW << "reduced connectivity"
              << RESET << '.' << endl << "Would you like to revert to the " << BOLD << "original" << RESET << " network?"
              << YES_NO;

        uSet<string> options = {"yes", "no"};
        string choice = readInput(instr.str(), options);

        if (choice == "yes"){
            graph.restoreNetwork();
            return;
        }
    }

    // reduce the current network
    std::list<std::pair<int,int>> edgesToRemove;

    while(true){
        int station = stationIDs[readLocation()];
        std::vector<Edge*> edges = printEdges(station);
        readInputFromTable(edgesToRemove, edges, station);
        cout << "Would you like to select more edges (y/n): \n" << endl;
        std::string s; std::cin >> s;
        if (s == "y" || s == "yes") continue;
        break;
    }

    graph.reduceConnectivity(edgesToRemove);
}

/**
 * @brief changes the stations that are the railway sources
 */
void Helpy::changeRailwaySources(){
    uSet<int>& railwaySources = graph.railwaySources;

    // ADD
    cout << BREAK;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "sources" << RESET
         << " you would like to " << GREEN << "add" << RESET << ", separated by a semicolon (ex: Src A;Src B;...)." << endl
         << "If you do not wish to add any new source, simply type a semicolon and press Enter." << endl << endl;

    string line; getline(std::cin >> std::ws, line);
    Utils::lowercase(line);

    if (line.back() != ';') line += ";";

    std::istringstream line_(line);
    for (string temp; getline(line_, temp, ';');){
        if (stationIDs.find(temp) == stationIDs.end())
            continue;

        graph.fullPicture &= !railwaySources.insert(stationIDs[temp]).second;
    }

    // REMOVE
    cout << BREAK;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "sources" << RESET
         << " you would like to " << RED << "remove" << RESET << ", separated by a semicolon (ex: Src A;Src B;...)." << endl
         << "If you do not wish to remove any source, simply type a semicolon and press Enter." << endl << endl;

    getline(std::cin >> std::ws, line);
    Utils::lowercase(line);

    if (line.back() != ';') line += ";";

    line_.clear(); line_.str(line);
    for (string temp; getline(line_, temp, ';');){
        if (stationIDs.find(temp) == stationIDs.end())
            continue;

        graph.fullPicture &= !railwaySources.erase(stationIDs[temp]);
    }
}

/**
 * @brief changes the stations that are the railway sinks
 */
void Helpy::changeRailwaySinks(){
    uSet<int>& railwaySinks = graph.railwaySinks;

    // ADD
    cout << BREAK;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "sinks" << RESET
         << " you would like to " << GREEN << "add" << RESET << ", separated by a semicolon (ex: Sink A;Sink B;...)." << endl
         << "If you do not wish to add any new sink, simply type a semicolon and press Enter." << endl << endl;

    string line; getline(std::cin >> std::ws, line);
    Utils::lowercase(line);

    if (line.back() != ';') line += ";";

    std::istringstream line_(line);
    for (string temp; getline(line_, temp, ';');){
        if (stationIDs.find(temp) == stationIDs.end())
            continue;

        graph.fullPicture &= !railwaySinks.insert(stationIDs[temp]).second;
    }

    // REMOVE
    cout << BREAK;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "sinks" << RESET
         << " you would like to " << RED << "remove" << RESET << ", separated by a comma (ex: Sink A;Sink B;...)." << endl
         << "If you do not wish to remove any sink, simply type a semicolon and press Enter." << endl << endl;

    getline(std::cin >> std::ws, line);
    Utils::lowercase(line);

    if (line.back() != ';') line += ";";

    line_.clear(); line_.str(line);
    for (string temp; getline(line_, temp, ';');){
        if (stationIDs.find(temp) == stationIDs.end())
            continue;

        graph.fullPicture &= !railwaySinks.erase(stationIDs[temp]);
    }
}

/**
 * @brief allows the user to choose the mode of the UI
 */
void Helpy::terminal(){
    string instruction = "Which mode would you prefer?\n\n"
                         "* Guided\n"
                         "* Advanced";
    uSet<string> options = {"guided", "advanced", "adv"};

    (readInput(instruction, options) == "guided") ? guided_mode() : advanced_mode();
}
