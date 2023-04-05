#include "Helpy.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

// output colors
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"

// line breaks
#define BREAK   std::endl << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << RESET << std::endl << std::endl

std::map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2},
                                        {"determine", 2}, {"change", 3}, {"switch", 3}, {"toggle", 3}};

std::map<string, int> Helpy::target = {{"station", 6}, {"shortest", 8}, {"maximum", 10}, {"most", 12},
                                       {"budget", 14}, {"affected", 17}, {"operating", 19}};

std::map<string, int> Helpy::what = {{" ", 22},{"information", 24}, {"info", 24}, {"route", 27},
                                     {"routes", 27}, {"train", 27}, {"trains", 27}, {"path", 27},
                                     {"paths", 27}, {"station", 29}, {"stations", 29}, {"need", 31},
                                     {"mode", 33}};

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
Helpy::Helpy(RailGraph& graph, uMap<string, int> stationIDs) : graph(graph), stationIDs(std::move(stationIDs)) {}

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
        lowercase(line);

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
        lowercase(line);

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
 * @brief reads the name of an station from the console
 * @complexity O(n)
 * @return the index of the station
 */
int Helpy::readStation(){
    int station = 0;
    while (true){
        std::cout << BREAK;
        std::cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << "station" << RESET << ":"
                  << std::endl << std::endl;

        string line; getline(std::cin >> std::ws, line);

        properName(line);
        
        if (stationIDs.find(line) != stationIDs.end()){
            station = stationIDs[line];
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

    std::cin >> s1; lowercase(s1);

    if (s1 == "quit" || s1 == "no" || s1 == "die"){
        goto e1;
    }

    std::cin >> s2;
    lowercase(s2);

    std::cin >> s3;
    lowercase(s3);

    if (!process_command(s1, s2, s3)){
        goto b1;
    }

    std::cout << BREAK;
    std::cout << "Anything else? (Yes/No)" << std::endl << std::endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    lowercase(s1);

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

    std::cin >> s1; lowercase(s1);

    if (s1 == "calculate"){
        std::cout << BREAK;
        std::cout << "* Maximum" << std::endl;
        std::cout << "* Most" << std::endl;
        std::cout << std::endl;
    }
    else if (s1 == "quit"){
        goto e2;
    }
    else if (s1 == "determine"){
        std::cout << BREAK;
        std::cout << "* Budget" << std::endl;
        std::cout << "* Affected" << std::endl;
        std::cout << std::endl;
    }
    else if (s1 == "change" || s1 == "display"){
        std::cout << BREAK;
        std::cout << "* Operating" << std::endl;
        std::cout << std::endl;
    }
    else { // erro
        process_command(s1, s2, s3);
        goto b2;
    }

    std::cin >> s2; lowercase(s2);

    if (s2 == "maximum"){
        std::cout << BREAK;
        std::cout << "* Trains" << std::endl;
        std::cout << std::endl;
    }
    else if (s2 == "most"){
        std::cout << BREAK;
        std::cout << "* Trains" << std::endl;
        std::cout << std::endl;
    }
    else if (s2 == "budget"){
        std::cout << BREAK;
        std::cout << "* Need" << std::endl;
        std::cout << std::endl;
    }
    else if (s2 == "affected"){
        std::cout << BREAK;
        std::cout << "* Stations" << std::endl;
        std::cout << std::endl;
    }
    else if (s2 == "operating"){
        std::cout << BREAK;
        std::cout << "* Mode" << std::endl;
        std::cout << std::endl;
    }
    else if (s2 == "quit"){
        goto e2;
    }
    else{ // erro
        process_command(s1, s2, s3);
        goto b2;
    }

    std::cin >> s3; lowercase(s3);

    if (s3 == "quit"){
        goto e2;
    }

    // processar o comando
    if (!process_command(s1, s2, s3)){
        goto b2;
    }

t2: std::cout << BREAK;
    std::cout << "Anything else? (Yes/No)" << std::endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y"){
            goto b2;
        }
    }

e2: std::cout << BREAK;
    std::cout << "See you next time!" << std::endl << std::endl;
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
        case(39) : {
            chooseMaximumTrains();
            break;
        }
        case(41) : {
            determineMostTrains();
            break;
        }
        case(47) : {
            determineBudgetNeed();
            break;
        }
        case(48) : {
            determineAffectedStations();
            break;
        }
        case (53) : {
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

void Helpy::displayOperatingMode(){
    string mode = (graph.profitMode) ? "Profit" : "Standard";
    std::cout << "The current " << BOLD << "operating mode" << RESET << " is " << YELLOW << mode << RESET << "." << std::endl;

    string instruction = "Would you like to change it?";
    uSet<string> options = {"yes", "no"};

    if (readInput(instruction, options) == "yes") changeOperatingMode();
}

/**
 * @brief chooses between calling calculateMaximumTrainsTwoStations or calculateMaximumTrains
 * @complexity O(n * |E|)
 */
void Helpy::chooseMaximumTrains(){
    std::cout << BREAK;
    std::cout << "1. Between two stations" << std::endl;
    std::cout << "2. That can simultaneously arrive at a station" << std::endl;
    std::cout << std::endl;
    string s;
    std::cin >> s;
    if(s == "1"){
        return calculateMaximumTrainsTwoStations();
    } else if (s == "2"){
        return calculateMaximumTrains();
    }
    std::cout << BREAK;
    std::cout << std::endl << RED << "Invalid number! Please, try again." << RESET << std::endl;
    return chooseMaximumTrains();
}

/**
 * @brief computes the pairs of stations (if more than one) that require the most trains when taking full advantage of the existing network capacity
 * @complexity O(n * |E|)
 */
void Helpy::determineMostTrains(){
    int airport = readStation();
    std::cout << BREAK;

}

/**
 * @brief computes the maximum number of trains that go between two stations simultaneously
 * @complexity O(n * |E|)
 * @param start station where we begin
 * @param finish station where we end up
 */
void Helpy::calculateMaximumTrainsTwoStations(){
    int airport = readStation();


    std::cout << BREAK;

}

/**
 * @brief computes the maximum number of trains that can arrive at a station simultaneously
 * @complexity O(n * |E|)
 * @param start station that we want to test
 */
void Helpy::calculateMaximumTrains(){
    int airport = readStation();


    std::cout << BREAK;

}

/**
 * @brief computes the top k districts (or municipalities) where management should assign a larger budget for the purchase and maintenance of trains
 * @complexity O(n * |E|)
 * @param number number of districts (or municipalities) to show
 */
void Helpy::determineBudgetNeed(){
    int number = readStation();

    std::cout << BREAK;
}

/**
 * @brief computes the stations that are most affected by each segment failure
 * @complexity O(n * |E|)
 */
void Helpy::determineAffectedStations(){
    int airport = readStation();


    std::cout << BREAK;

}
