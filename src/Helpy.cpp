#include "Helpy.h"

#include "libfort/fort.hpp"

using std::cout;
using std::endl;

// output colors
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

// line breaks
#define BREAK   endl << YELLOW << "- - - - - - - - - - - - - - - - - - - - - - - - - -" << RESET << endl << endl

std::map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2},
                                        {"determine", 2}, {"change", 3}, {"switch", 3}, {"toggle", 3}};

std::map<string, int> Helpy::target = {{"all", 4}, {"data", 6}, {"shortest", 8}, {"maximum", 10},
                                       {"most", 12}, {"budget", 14}, {"affected", 17}, {"operating", 19},
                                       {"busiest", 21}, {"railway",23}};

std::map<string, int> Helpy::what = {{"directory", 24},{"train", 27}, {"trains", 27}, {"pair", 27},
                                     {"pairs", 27},{"station", 29},{"stations", 29}, {"district", 29},
                                     {"districts", 29},{"municipality", 29},{"municipalities", 29},
                                     {"need", 31}, {"mode", 33}, {"network", 35}};

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
 * @complexity O(n^2)
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

void Helpy::readInputFromTable(std::list<std::pair<int,int>>& edges, std::vector<Edge*> ref, int station){
    int size = ref.size();
    std::cout << std::endl << YELLOW << BREAK << RESET;
    std::cout << "Please type the " << BOLD << "indexes" << RESET << " of the " << YELLOW << "edges" << RESET << " you would like to " << RED << "remove"
        << RESET << ", separated by a comma (ex: 0,1,2,7,...).\n";

    // countries to USE
    string line; getline(std::cin, line);
    line += ",";

    std::istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        int k = std::stoi(temp);

        if(k > size) return; //preguiça depois faz-se

        edges.push_back({station, ref[k]->getDest()});
    }
}

/**
 * @brief allows the user to choose the mode of the UI
 * @complexity O(n^2)
 */
void Helpy::terminal(){
    string instruction = "Which mode would you prefer?\n\n"
                         "* Guided\n"
                         "* Advanced";
    uSet<string> options = {"guided", "advanced", "adv"};

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
    cout << "Anything else? (Yes/No)" << endl << endl;

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
 * @complexity O(1)
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
    cout << BREAK;

    if (s1 == "display"){
        cout << "* All" << endl;
        cout << "* Busiest" << endl;
        cout << "* Data" << endl;
        cout << "* Operating" << endl;
    }
    else if (s1 == "calculate"){
        cout << "* Maximum" << endl;
        cout << "* Most" << endl;
    }
    else if (s1 == "determine"){
        cout << "* Affected" << endl;
        cout << "* Busiest" << endl;
    }
    else if (s1 == "change"){
        cout << "* Data" << endl;
        cout << "* Operating" << endl;
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
    cout << BREAK;

    if (s2 == "all"){
        cout << "* Stations" << endl;
    }
    else if (s2 == "data"){
        cout << "* Directory" << endl;
    }
    else if (s2 == "maximum" || s2 == "most"){
        cout << "* Trains" << endl;
    }
    else if (s2 == "affected"){
        cout << "* Stations" << endl;
    }
    else if (s2 == "busiest"){
        cout << "* Stations" << endl;
        cout << "* Districts" << endl;
        cout << "* Municipalities" << endl;
        cout << "* Pairs" << endl;
    }
    else if (s2 == "operating"){
        cout << "* Mode" << endl;
    }
    else if (s2 == "railway"){
        cout << "* Network" << endl;
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

t2: cout << BREAK;
    cout << "Anything else? (Yes/No)" << endl << endl;

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
 * @complexity O(n^2 * log(n))
 * @param s1 first word of the command
 * @param s2 second word of the command
 * @param s3 third word of the command
 * @return 'true' if the command exists, 'false' otherwise
 */
bool Helpy::process_command(string& s1, string& s2, string& s3){
    switch (command[s1] + target[s2] + what[s3]){
        case (31) : {
            displayDataDirectory();
            break;
        }
        case (33) : {
            changeDataDirectory();
            break;
        }
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
        case(61) : {
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

void Helpy::printPath(Path& p){
    fort::utf8_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"Source", "Destination", "Service"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    //for ()
}

double Helpy::getIncomingTrains(int index, bool display, bool ori){
    if(ori){
        double flow = 0;
        (*original).getFullPicture();

        // create the table
        fort::utf8_table table;

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
        for (const Edge* e : (*original)[index].inEdges()){
            flow += e->getFlow();

            auto s = dynamic_cast<const Railway*>(e);
            if (s == nullptr) continue;

            table << (*original)[s->getSrc()].getName() << s->getService() << s->getFlow() << fort::endr;
        }

        // print the table
        if (display){
            std::cout << BREAK;
            std::cout << table.to_string();
        }

        return flow;
    }
    else {
        double flow = 0;
        graph.getFullPicture();

        // create the table
        fort::utf8_table table;

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
}

double Helpy::getTrainsBetweenStations(int src, int sink){
    std::ostringstream instr;
    instr << "Would you like to consider " << BOLD << "only" << RESET << " the paths that ensure " << YELLOW
          << "minimum cost" << RESET << "for the company? (Yes/no)";

    uSet<string> options = {"yes", "no"};
    string res = readInput(instr.str(), options);

    if (res == "yes") return graph.maximumFlow(src, sink);

    // display the minimum cost paths
    std::list<Path> paths = graph.getMinimumCostPaths(src, sink);

    int optionNum = 1;
    for (Path& p : paths){
        cout << endl << endl << BOLD << "OPTION #" << optionNum++ << RESET << endl << endl;
        printPath(p);
    }
}

/**
 * @brief display the directory where the data files are stored
 */
void Helpy::displayDataDirectory(){
    cout << BREAK;
    cout << "The current data directory is " << BOLD << YELLOW << reader.getPath() << RESET << '.' << endl;
}

/**
 * @brief displays all the stations that are part of the railway network
 */
void Helpy::displayAllStations(){
    fort::utf8_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"N", "Name", "District", "Municipality", "Township", "Line"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    for(int i = 1; i <= graph.countVertices(); i++){
        table << i << graph[i].getName() << graph[i].getDistrict() << graph[i].getMunicipality() << graph[i].getTownship()
              << graph[i].getLine() << fort::endr;
    }

    cout << BREAK;
    cout << "The railway network has the following " << YELLOW << "stations" << RESET << ':' << endl << endl;
    cout << table.to_string();
}

/**
 * @brief displays the "company's" operating mode
 * @complexity O(n^2)
*/
void Helpy::displayOperatingMode(){
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

    fort::utf8_table table;

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

    cout << "These are the busiest pairs of stations:" << endl << endl;
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
 * @brief changes the operating mode of the RailGraph
 * @complexity O(1)
*/
void Helpy::changeOperatingMode(){
}

/**
 * @brief calculates either the maximum amount of trains that can simultaneously travel between two stations or the
 * maximum amount of trains that simultaneously arrive at a station
 * @complexity O(|V| * |E|^2)
 */
void Helpy::calculateMaximumTrains(){
    while (true) {
        std::ostringstream instr;
        instr << "Please enter the " << BOLD << "number" << RESET << " of the option you desire:" << endl << endl
              << "1. Between two stations" << endl
              << "2. That can simultaneously arrive at a station"
              << "3. Between two stations, with minimum cost";

        int n = (int) readNumber(instr.str());
        double maxTrains;

        switch (n){
            case (1) : {
                int stationA = stationIDs[readStation()];
                int stationB = stationIDs[readStation()];

                maxTrains = getTrainsBetweenStations(stationA, stationB);

                cout << BREAK;
                cout << "The maximum number of trains that can simultaneously travel between "
                     << graph[stationA].getName() << " and " << graph[stationB].getName() << " is " << BOLD
                     << YELLOW << maxTrains << RESET << '.' << endl;

                return;
            }
            case (2) : {
                int station = stationIDs[readStation()];
                maxTrains = getIncomingTrains(station, true);

                cout << BREAK;
                cout << "The maximum number of trains that can simultaneously arrive at " << graph[station].getName()
                     << " is " << BOLD << YELLOW << maxTrains << RESET << '.' << endl;

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
*/
void Helpy::determineAffectedStations(){
    std::ostringstream instr;
    instr << "Please enter the " << BOLD << "number" << RESET << " of " << YELLOW << "stations" << RESET
          << " you would like to display:";

    int k = (int) readNumber(instr.str());

    graph.getFullPicture();
    std::vector<std::pair<std::pair<double,double>, int>> beforeAndAfter;
    for(int i = 1; i <= graph.countVertices(); i++){
        beforeAndAfter.push_back({{getIncomingTrains(i, false, true),0},i});
    }
    for(int i = 1; i <= graph.countVertices(); i++){
        beforeAndAfter[i].first.second = getIncomingTrains(i);
    }
    std::sort(beforeAndAfter.begin(), beforeAndAfter.end(),[](auto &left, auto &right) {
        return std::abs(left.first.first - left.first.second) > std::abs(right.first.first - right.first.second);
    });
    beforeAndAfter.resize(k);

    // create the table
    fort::utf8_table table;

    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"N", "Station","Before", "After"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    int i = 1;
    for(auto& p: beforeAndAfter){
        table << i++ << graph[p.second].getName() << p.first.first << p.first.second << fort::endr;
    }

    cout << table.to_string();
}

/**
 * @brief Prints the out edges of a station
 * @param station considered station
*/
std::vector<Edge*> Helpy::printEdges(int station){
    fort::utf8_table table;
    table.set_border_style(FT_NICE_STYLE);
    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    std::list<string> columnNames = {"N", "Source","Destination"};

    auto it = columnNames.begin();
    for (int i = 0; it != columnNames.end(); ++i){
        table << *it++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    int i = 0;
    std::vector<Edge*> edges;
    for(auto e: graph[station].outEdges()){
        edges.push_back(e);
        table << i++ << graph[station].getName() << graph[e->getDest()].getName() << fort::endr;
    }
    std::cout << table.to_string();
    return edges;
}


/**
 * @brief changes the considered railway network while preserving the original network
*/
void Helpy::changeRailwayNetwork(){
    std::list<std::pair<int,int>> edgesToRem;
    while(true){
        int station = stationIDs[readStation()];
        std::vector<Edge*> edges = printEdges(station);
        readInputFromTable(edgesToRem, edges, station);
        std::cout << "Would you like to select more edges (y/n): \n" << std::endl;
        std::string s; std::cin >> s;
        if(s == "y" || s == "yes") continue;
        break;

    }
    *original = graph;
    graph = graph.subGraph(edgesToRem);

}
