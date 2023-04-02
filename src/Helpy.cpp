#include "Helpy.h"

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>


#define uSet unordered_set
#define uMap unordered_map

// cores para o output
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define BREAK   "- - - - - - - - - - - - - - - - - - - - -"

map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2}, {"determine", 2}};
map<string, int> Helpy::target = {{"station", 6}, {"shortest", 8}, {"maximum", 10}, {"number", 12}, {"budget", 14}};
map<string, int> Helpy::what = {{"information", 24}, {"info", 24}, {"route", 27}, {"routes", 27}, {"flight", 27},
                                {"trains", 27}, {"path", 27}, {"paths", 27}, {"airport", 29}, {"airports", 29}};

/**
 * @brief turns all the characters of a string into lowercase or uppercase
 * @complexity O(n)
 * @param s string to be modified
 * @param uppercase if true turns all the characters of the string to uppercase; if false turns all the characters of the string to lowercase
 */
void Helpy::lowercase(string& s, bool uppercase){
    for (char& c : s){
        c = (uppercase) ? (char) toupper(c) : (char) tolower(c);
    }
}

/**
 * @brief takes a user inputted string and modifies it so that it becomes well-written
 * @complexity O(n)
 * @param s string to be modified
 */
void Helpy::properName(string& s){
    string name;

    bool first = true;
    for (istringstream s_(s); s_ >> s;){
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
 * @param graph graph that contains all the data regarding Airports, Airlines and flights
 */
Helpy::Helpy(AirGraph& graph) : graph(graph) {}

/**
 * @brief adds airports to the Helpy database
 * @param codes codes of the airports
 * @param names names of the airports
 */
void Helpy::setAirports(const uSet<string>& codes, const uMap<string, string>& names){
    this->airportCodes = codes;
    this->airportNames = names;
}

/**
 * @brief adds airlines to the Helpy database
 * @param codes codes of the airlines
 */
void Helpy::setAirlines(const uSet<string>& codes){
    this->airlineCodes = codes;
}

/**
 * @brief adds cities and countries to the Helpy database
 * @param cities names of the cities (mapped to the codes of their respective airports)
 * @param countries names of the countries (mapped to the codes of their respective airports)
 */
void Helpy::setLocations(const uMap<string, uSet<string>>& cities, const uMap<string, uSet<string>>& countries){
    this->cityNames = cities;
    this->countryNames = countries;
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
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << instruction << endl << endl;

        string line; getline(cin >> ws, line);
        lowercase(line);

        istringstream line_(line);

        while (line_ >> res){
            if (options.find(res) != options.end()){
                valid = true;
                break;
            }
        }

        if (valid) break;

        cout << endl << YELLOW << BREAK << RESET << endl << endl;
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
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << instruction << endl << endl;

        string line; getline(cin >> ws, line);
        lowercase(line);

        istringstream line_(line);

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

        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << RED << "Invalid input! Please, try again." << RESET << endl;
    }

    return res;
}

/**
 * @brief reads the code/name of an airport from the console
 * @complexity O(n)
 * @return the code of the airport
 */
string Helpy::readAirport(){
    string airport;

    while (true){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "Please type the " << BOLD << "code" << RESET << " or the " << BOLD << "name" << RESET
             << " of the " << YELLOW << "airport" << RESET << ":" << endl << endl;

        string line; getline(cin >> ws, line);

        bool nameFound = false, codeFound = false;
        if (line.size() > 3){
            properName(line);
            nameFound = (airportNames.find(line) != airportNames.end());
        }
        else{
            lowercase(line, true);
            codeFound = (airportCodes.find(line) != airportCodes.end());
        }

        if (nameFound){
            airport = airportNames[line];
            break;
        }
        else if (codeFound){
            airport = line;
            break;
        }

        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << RED << "Invalid input! The airport you entered does not exist. Please, try again." << RESET << endl;
    }

    return airport;
}

/**
 * @brief reads the name of a city/country from the console
 * @complexity O(n^2)
 * @param country bool that establishes if the user will input the name of a city or a country
 * @return the code of the airport the user selects
 */
string Helpy::readCity(bool country){
    string airport;
    string where = (country) ? "country" : "city";

    while (true){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << where << RESET << ':' << endl << endl;

        string line; getline(cin >> ws, line);
        properName(line);

        bool countryFound = (country && countryNames.find(line) != countryNames.end());
        bool cityFound = (!country && cityNames.find(line) != cityNames.end());

        if (!countryFound && !cityFound){
            cout << endl << YELLOW << BREAK << RESET << endl << endl;
            cout << RED << "Invalid input! The " << where << " you entered does not exist. "
                 << "Please, try again." << RESET << endl;

            continue;
        }

        uSet<string> airports = (country) ? countryNames[line] : cityNames[line];

        if (airports.size() == 1){
            airport = airportNames[*airports.begin()];
            break;
        }

        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << BOLD << line << RESET << " has the following airports: " << RESET << endl << endl;

        for (const string& s : airports){
            cout << "* " << s << " (" << airportNames[s] << ')' << endl;
        }

        airport = readAirport();
        break;
    }

    return airport;
}

/**
 * @brief reads coordinates from the console and displays the airports nearest that location
 * @complexity O(n^2)
 * @return the code of the airport the user selects
 */
string Helpy::readCoordinates(){
    map<double, string> air;

    while (true){
        // read the latitude
        ostringstream instr;
        instr << "Please type your " << BOLD << "latitude" << RESET << ':';

        double latitude = readNumber(instr.str());

        // read the longitude
        instr.clear(); instr.str("");
        instr << "Please type your " << BOLD << "longitude" << RESET << ':';

        double longitude = readNumber(instr.str());

        // read the radius
        instr.clear(); instr.str("");
        instr << "Please type the " << BOLD << "radius" << RESET << " in which you are looking for " << YELLOW << "airports" << RESET << ":";

        double radius = readNumber(instr.str());

        // calculate the nearest airports
        air = graph.getNearbyAirports(latitude, longitude, radius);

        if (!air.empty()) break;

        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << RED << "Error! No airports were found near that location. Please, try again." << RESET << endl;
    }

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "These are the " << BOLD << "airports" << RESET << " that are nearest your location:" << endl << endl;

    for (const auto& p : air){
        cout << "* " << p.second << " (" << airportNames[p.second] << ") -> " << p.first << " km away" << endl;
    }

    return readAirport();
}

/**
 * @brief reads a location based on an airport OR a city OR a country OR coordinates
 * @complexity O(n^2)
 * @return the code of the airport situated in said location
 */
string Helpy::readLocation(string instruction){
    if (instruction.empty()){
        ostringstream instr;
        instr << "With which of the following would you like to define the desired " << BOLD << "location"
              << RESET << "?" << endl << endl
              << "* Airport" << endl
              << "* City" << endl
              << "* Country" << endl
              << "* Coordinates";

        instruction = instr.str();
    }

    uSet<string> options = {"airport", "city", "country", "coordinates"};
    string choice = readInput(instruction, options);

    if (choice == "airport"){
        return readAirport();
    }
    else if (choice == "city"){
        return readCity();
    }
    else if (choice == "country"){
        return readCity(true);
    }

    return readCoordinates();
}

/**
 * @brief reads from the console the selection of airlines to use
 * @complexity O(n)
 * @return the codes of the selected airlines
 */
uSet<string> Helpy::readUsableAirlines(){
    uSet<string> airlines;

    cout << "Please type the " << BOLD << "codes" << RESET << " of the " << YELLOW << "airlines" << RESET
         << " you would like to " << GREEN << "use" << RESET << ", separated by a comma (ex: TAP,RYN,...).\n"
         << "If there is no airline you would particularly like to use, press Enter.\n\n";

    // airlines to USE
    string line; getline(cin, line);
    lowercase(line, true); line += ",";

    istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        if (airlineCodes.find(temp) != airlineCodes.end()){
            airlines.insert(temp);
        }
    }

    if (!airlines.empty()) return airlines;

    // airlines to AVOID
    airlines = airlineCodes;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "codes" << RESET << " of the " << YELLOW << "airlines" << RESET
         << " you would like to " << RED << "avoid" << RESET << ", separated by a comma (ex: TAP,RYN,...).\n"
         << "If there is no airline you wish to avoid, simply press Enter.\n\n";

    getline(cin, line);
    lowercase(line, true); line += ",";

    line_.clear(); line_.str(line);

    for (string temp; getline(line_, temp, ',');){
        auto it = airlines.find(temp);

        if (it != airlines.end()){
            airlines.erase(it);
        }
    }

    return airlines;
}

/**
 * @brief reads from the console the selection of cities to consider
 * @complexity O(n^2)
 * @return the codes of the airports that are in the considered cities
 */
uSet<string> Helpy::readUsableCities(){
    uSet<string> airports;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "cities" << RESET << " you would like to " << GREEN << "use"
         << RESET << ", separated by a comma (ex: Porto,Lisbon,...).\n"
         << "If there is no city you would particularly like to visit, press Enter.\n\n";

    // cities to USE
    string line; getline(cin, line);
    lowercase(line, true); line += ",";

    istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        properName(temp);

        if (cityNames.find(temp) == cityNames.end()){
            continue;
        }

        for (const string& s : cityNames[temp]){
            airports.insert(airportNames[s]);
        }
    }

    if (!airports.empty()) return airports;

    // cities to AVOID
    airports = airportCodes;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "cities" << RESET
         << " you would like to " << RED << "avoid" << RESET << ", separated by a comma (ex: Porto,Lisbon,...).\n"
         << "If there is no city you wish to avoid, press Enter.\n\n";

    getline(cin, line);
    lowercase(line, true); line += ",";

    line_.clear(); line_.str(line);

    for (string temp; getline(line_, temp, ',');){
        properName(temp);

        if (cityNames.find(temp) == cityNames.end()){
            continue;
        }

        for (const string& s : cityNames[temp]){
            airports.erase(airportNames[s]);
        }
    }

    return airports;
}

/**
 * @brief reads from the console the selection of countries to consider
 * @complexity O(n^2)
 * @return the codes of the airports that are in the considered countries
 */
uSet<string> Helpy::readUsableCountries(){
    uSet<string> airports;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "countries" << RESET << " you would like to " << GREEN << "use"
         << RESET << ", separated by a comma (ex: Portugal,Spain,...).\n"
         << "If there is no country you would particularly like to visit, press Enter.\n\n";

    // countries to USE
    string line; getline(cin, line);
    lowercase(line, true); line += ",";

    istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        properName(temp);

        if (countryNames.find(temp) == countryNames.end()){
            continue;
        }

        for (const string& s : countryNames[temp]){
            airports.insert(airportNames[s]);
        }
    }

    if (!airports.empty()) return airports;

    // countries to AVOID
    airports = airportCodes;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "names" << RESET << " of the " << YELLOW << "countries" << RESET
         << " you would like to " << RED << "avoid" << RESET << ", separated by a comma (ex: Brazil,Mexico,...).\n"
         << "If there is no country you wish to avoid, press Enter.\n\n";

    getline(cin, line);
    lowercase(line, true); line += ",";

    line_.clear(); line_.str(line);

    for (string temp; getline(line_, temp, ',');){
        properName(temp);

        if (countryNames.find(temp) == countryNames.end()){
            continue;
        }

        for (const string& s : countryNames[temp]){
            airports.erase(airportNames[s]);
        }
    }

    return airports;
}

/**
 * @brief reads the airports that the user wants to use
 * @complexity O(n^2)
 * @return the codes of the airports the user selects
 */
uSet<string> Helpy::readUsableAirports(){
    uSet<string> airports;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "codes" << RESET << " of the " << YELLOW << "airports" << RESET << " you would like to "
         << GREEN << "use" << RESET << ", separated by a comma (ex: OPO,PXO,...).\n"
         << "If there is no airport you would particularly like to use, press Enter.\n\n";

    // airports to USE
    string line; getline(cin, line);
    lowercase(line, true); line += ",";

    istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        if (temp.size() > 3){
            properName(temp);
            temp = airportNames[temp];
        }
        else{
            lowercase(temp, true);
        }

        if (airportCodes.find(temp) != airlineCodes.end()){
            airports.insert(temp);
        }
    }

    if (!airports.empty()) return airports;

    // airlines to AVOID
    airports = airportCodes;

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Please type the " << BOLD << "codes" << RESET << " of the " << YELLOW << "airports" << RESET << " you would like to "
         << RED << "avoid" << RESET << ", separated by a comma (ex: OPO,PXO,...).\n"
         << "If there is no airport you wish to avoid, simply press Enter.\n\n";

    getline(cin, line);
    lowercase(line, true); line += ",";

    line_.clear(); line_.str(line);

    for (string temp; getline(line_, temp, ',');){
        if (temp.size() > 3) temp = airportNames[temp];

        auto it = airports.find(temp);

        if (it != airports.end()){
            airports.erase(it);
        }
    }

    // read cities and countries to use
    uSet<string> cityAirports = readUsableCities();
    uSet<string> countryAirports = readUsableCountries();

    for (auto it = airports.begin(); it != airports.end();){
        bool foundCity = (cityAirports.find(*it) != cityAirports.end());
        bool foundCountry = (countryAirports.find(*it) != countryAirports.end());

        if (foundCity && foundCountry){
            it++;
            continue;
        }

        it = airports.erase(it);
    }

    return airports;
}

/**
 * @brief reads the Airlines and the Airports the user would like to use
 * @complexity O(n^2)
 * @return array of two unordered_sets of strings, both of which containing codes (of Airlines and Airports, respectively)
 */
uSet<string>* Helpy::readRestrictions(){
    auto use = new uSet<string>[2];

    ostringstream instr;
    instr << "To better meet your requirements, I will now ask you some questions regarding which " << YELLOW << "airlines" << RESET
          << ", " << YELLOW << "airports" << RESET << " and " << YELLOW << "countries" << RESET << " you would like to use or avoid.\n"
          << "However, if you do not want to have such restrictions, I can skip this part.\n\n"
          << "Would you like to answer the questions? (Yes/No)";

    uSet<string> options = {"yes", "no", "y", "n"};
    string ans = readInput(instr.str(), options);

    if (ans == "no" || ans == "n"){
        return nullptr;
    }

    cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "Very well. Let us start.\n\n";

    use[0] = readUsableAirlines();
    use[1] = readUsableAirports();

    return use;
}

/**
 * @brief prints in a table the name, city and country of the Airports in the argument list
 * @complexity O(n)
 * @param airports container with the Airports whose information will be printed
 * @param columnNames list with the names of each table column
 */
template <typename T>
void Helpy::printAirports(T airports, const list<string>& columnNames){
    cout << endl << YELLOW << BREAK << RESET << endl;

    if (airports.empty()){
        cout << endl << "It appears there are no results that match your request..." << endl;
        return;
    }

    cout << endl << "These are the results of my search:" << endl << endl;

    // setting up the table
    fort::char_table table;
    table.set_border_style(FT_NICE_STYLE);

    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header;

    auto namesIt = columnNames.begin();
    for (int i = 0; i < (int) columnNames.size(); i++){
        table << *namesIt++;
        table.column(i).set_cell_text_align(fort::text_align::center);
    }

    table << fort::endr;

    // filling the table with data
    uSet<string> citiesReached, countriesReached;

    for (const auto& p : airports){
        const Airport& a = p.first;

        table << a.getName() << p.second << fort::endr;

        string airportInfo = '(' + a.getCity() + ", " + a.getCountry() + ')';

        citiesReached.insert(a.getCity());
        countriesReached.insert(a.getCountry());

        table << airportInfo << fort::endr;

        table << fort::separator;
    }

    cout << table.to_string();

    cout << endl << "In total:" << endl
         << "* " << YELLOW << "Cities" << RESET " reached: " << citiesReached.size() << endl
         << "* " << YELLOW << "Countries" << RESET << " reached: " << countriesReached.size() << endl;
}

/**
 * @brief prints all the flights of an Airport in a table
 * @complexity O(n * |E|)
 * @param airport the code of the Airport whose flights will be printed
 */
void Helpy::printFlights(const string& airport){
    // setting up the table
    fort::char_table table;
    table.set_border_style(FT_NICE_STYLE);

    for (int i = 0; i < 2; i++)
        table.column(i).set_cell_text_align(fort::text_align::center);

    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    table << fort::header
          << "Airport" << "Airlines" << fort::endr;

    // filling the table with data
    int flightNum = 0;
    uSet<string> citiesReached, countriesReached;

    for (const auto e : graph.getFlights(airport)){
        const Airport& dest = e->dest;
        auto it = e->airlines.begin();

        table << dest.getName() << it++->getName() << fort::endr;

        string destInfo = '(' + dest.getCity() + ", " + dest.getCountry() + ')';

        flightNum++;
        citiesReached.insert(dest.getCity());
        countriesReached.insert(dest.getCountry());

        if (it == e->airlines.end()){
            table << destInfo << "" << fort::endr;
        }
        else{
            table << destInfo << it++->getName() << fort::endr;
        }

        while (it != e->airlines.end()){
            table << "" << it++->getName() << fort::endr;
        }

        table << fort::separator;
    }

    cout << table.to_string();

    cout << endl << "In total:" << endl
         << "* Number of " << YELLOW << "flights" << RESET ": " << flightNum << endl
         << "* " << YELLOW << "Cities" << RESET " reached: " << citiesReached.size() << endl
         << "* " << YELLOW << "Countries" << RESET << " reached: " << countriesReached.size() << endl;
}

/**
 * @brief prints a table with the information of a path
 * @complexity O(n * |E|), being 'n' the number of Airlines of the edge
 * @param p path to be printed
 */
void Helpy::printPath(const Path& p){
    // setting up the table
    fort::char_table table;
    table.set_border_style(FT_NICE_STYLE);

    table.row(0).set_cell_content_text_style(fort::text_style::bold);
    table.row(0).set_cell_content_fg_color(fort::color::yellow);
    for (int i = 0; i < 4; i++)
        table.column(i).set_cell_text_align(fort::text_align::center);

    table << fort::header
          << "N" << "Airport (start)" << "Airport (destination)" << "Airlines" << fort::endr;

    // filling the table with data
    int order = 1;
    for (const auto* e : p){
        const Airport& src = e->src;
        const Airport& dest = e->dest;
        auto it = e->airlines.begin();

        table << order++ << src.getName() << dest.getName() << it++->getName() << fort::endr;

        string srcInfo = "(" + src.getCity() + ", " + src.getCountry() + ")";
        string destInfo = "(" + dest.getCity() + ", " + dest.getCountry() + ")";

        if (it == e->airlines.end()){
            table << "" << srcInfo << destInfo << "" << fort::endr;
        }
        else{
            table << "" << srcInfo << destInfo << it++->getName() << fort::endr;
        }

        while (it != e->airlines.end()){
            table << "" << "" << "" << it++->getName() << fort::endr;
        }

        table << fort::separator;
    }

    cout << table.to_string();
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
b1: cout << endl << YELLOW << BREAK << RESET << endl;
    cout << endl << "How can I be of assistance?" << endl << endl;

    string s1, s2, s3;
    istringstream s_;

    cin >> s1; lowercase(s1);

    if (s1 == "quit" || s1 == "no" || s1 == "die"){
        goto e1;
    }

    cin >> s2;
    lowercase(s2);

    cin >> s3;
    lowercase(s3);

    // processar o comando
    if (!process_command(s1, s2, s3)){
        goto b1;
    }

    cout << endl << YELLOW << BREAK << RESET << endl;
    cout << endl << "Anything else? (Yes/No)" << endl << endl;

    s1.clear(); getline(cin >> ws, s1);
    lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y"){
            goto b1;
        }
    }

e1: cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "See you next time!" << endl << endl;
}

/**
 * @brief executes the guided mode of the UI
 * @complexity O(1)
 */
void Helpy::guided_mode(){
b2: cout << endl << YELLOW << BREAK << RESET << endl;
    cout << endl << "Hello! How can I be of assistance?" << endl;
    cout << endl;
    cout << "* Display" << endl;
    cout << endl;

    string s1, s2, s3;
    istringstream s_;

    cin >> s1; lowercase(s1);
    if (s1 == "display"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Airport" << endl;
        cout << "* Shortest" << endl;
        cout << "* Reachable" << endl;
        cout << endl;
    }
    else if (s1 == "quit"){
        goto e2;
    }
    else { // erro
        process_command(s1, s2, s3);
        goto b2;
    }

    cin >> s2; lowercase(s2);

    if (s2 == "airport"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Information" << endl;
        cout << endl;
    }
    else if (s2 == "shortest"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Routes" << endl;
        cout << endl;
    }
    else if (s2 == "reachable"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Airports" << endl;
        cout << endl;
    }
    else if (s2 == "quit"){
        goto e2;
    }
    else{ // erro
        process_command(s1, s2, s3);
        goto b2;
    }

    cin >> s3; lowercase(s3);

    if (s3 == "quit"){
        goto e2;
    }

    // processar o comando
    if (!process_command(s1, s2, s3)){
        goto b2;
    }

t2: cout << endl << YELLOW << BREAK << RESET << endl;
    cout << endl << "Anything else? (Yes/No)" << endl;

    s1.clear(); getline(cin >> ws, s1);
    lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y"){
            goto b2;
        }
    }

e2: cout << endl << YELLOW << BREAK << RESET << endl << endl;
    cout << "See you next time!" << endl << endl;
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
        case(31) : {
            displayAirportInformation();
            break;
        }
        case(36) : {
            displayShortestRoutes();
            break;
        }
        case (40) : {
            displayReachableAirports();
            break;
        }
        default : {
            cout << endl << YELLOW << BREAK << RESET << endl;
            cout << endl << RED << "Invalid command! Please, type another command." << RESET << endl;

            return false;
        }
    }

    return true;
}

/**
 * @brief displays all the flights you can take on a given Airport, as well as the Airlines that make said flights
 * @complexity O(n * |E|)
 * @param airport code of the Airport
 */
void Helpy::displayAirportInformation(){
    string airport = readAirport();
    const Airport& a = graph.getAirport(airport);

    cout << endl << YELLOW << BREAK << RESET << endl;
    cout << endl << "The airport " << a.getName() << " (" << a.getCity() << ", " << a.getCountry()
         << ") has the following flights:" << endl << endl;

    printFlights(airport);
}

/**
 * @brief displays all the Airports you can reach from a starting point, given either a number of flights or a flight distance
 * @complexity O(n^2 + |V| + |E|)
 */
void Helpy::displayReachableAirports(){
    string airport = readLocation();

    ostringstream instr;
    instr << "Which of the following would you like to use to define where you can get?" << endl << endl
          << "* Number of flights" << endl
          << "* Flight distance";

    uSet<string> options = {"number", "flights", "distance"};
    string choice = readInput(instr.str(), options);

    // calculate reachable airports
    list<string> columnNames = {"Airport"};

    if (choice == "distance"){
        double distance = readNumber("Please type the flight distance you would like to consider:");
        columnNames.push_back("Distance (km)");

        printAirports(graph.getReachableAirports(airport, distance, readRestrictions()), columnNames);

        return;
    }

    int flights = (int) readNumber("Please type the number of flights you want to take:");
    columnNames.push_back("Flights");

    printAirports(graph.getReachableAirports(airport, flights, readRestrictions()), columnNames);
}

/**
 * @brief displays the shortest routes you can take from an Airport to another
 * @complexity O(n^2 + |V| + |E|)
 */
void Helpy::displayShortestRoutes(){
    string airportA;
    string airportB;

    // starting point
    ostringstream instr;
    instr << "With which of the following would you like to define the " << BOLD << "starting point"
          << RESET << "?" << endl << endl
          << "* Airport" << endl
          << "* City" << endl
          << "* Country" << endl
          << "* Coordinates";

    string instruction = instr.str();
    airportA = readLocation(instruction);

    // destination
    instr.clear(); instr.str("");
    instr << "With which of the following would you like to define the " << BOLD << "destination"
          << RESET << "?" << endl << endl
          << "* Airport" << endl
          << "* City" << endl
          << "* Country" << endl
          << "* Coordinates";

    instruction = instr.str();
    airportB = readLocation(instruction);

    // calculate paths
    list<Path> allPaths = graph.getPaths(airportA, airportB, readRestrictions());

    cout << endl << YELLOW << BREAK << RESET << endl;

    if (allPaths.empty()){
        cout << endl << "It appears there are no results that match your request..." << endl;
        return;
    }

    cout << endl << "These are the results of my search:";

    int optionNum = 1;
    for (Path p : allPaths){
        cout << endl << endl << BOLD << "OPTION #" << optionNum++ << RESET << endl << endl;

        printPath(p);
    }
}