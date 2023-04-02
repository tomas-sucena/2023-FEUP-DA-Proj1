#include "Helpy.h"

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>


// cores para o output
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define BREAK   "- - - - - - - - - - - - - - - - - - - - -"

map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2}, {"determine", 2}};
map<string, int> Helpy::target = {{"station", 6}, {"shortest", 8}, {"maximum", 10}, {"most", 12}, {"budget", 14}};
map<string, int> Helpy::what = {{"information", 24}, {"info", 24}, {"route", 27}, {"routes", 27}, {"train", 27},
                                {"trains", 27}, {"path", 27}, {"paths", 27}, {"station", 29}, {"stations", 29}, {"need", 31}};

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
Helpy::Helpy(RailGraph& graph) : graph(graph) {}

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
 * @brief reads the code/name of an station from the console
 * @complexity O(n)
 * @return the code of the station
 */
int Helpy::readStation(){
    int station = 0;
    while (true){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << "station" << RESET << ":" << endl << endl;

        string line; getline(cin >> ws, line);

        properName(line);
        
        if (ids.find(line) != ids.end()){
            station = ids[line];
            break;
        }
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << RED << "Invalid input! The airport you entered does not exist. Please, try again." << RESET << endl;
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
    cout << "* Calculate" << endl;
    cout << "* Determine" << endl;
    cout << endl;

    string s1, s2, s3;
    istringstream s_;

    cin >> s1; lowercase(s1);
    if (s1 == "calculate"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Maximum" << endl;
        cout << "* Most" << endl;
        cout << endl;
    }
    else if (s1 == "quit"){
        goto e2;
    }
    else if (s1 == "determine"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Budget" << endl;
        cout << endl;
    }
    else { // erro
        process_command(s1, s2, s3);
        goto b2;
    }

    cin >> s2; lowercase(s2);

    if (s2 == "maximum"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Trains" << endl;
        cout << endl;
    }
    else if (s2 == "most"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Trains" << endl;
        cout << endl;
    }
    else if (s2 == "budget"){
        cout << endl << YELLOW << BREAK << RESET << endl << endl;
        cout << "* Need" << endl;
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
        case(39) : {
            calculateMaximumTrains();
            break;
        }
        case(41) : {
            determineMostTrains();
            break;
        }
        case(47) : {
            determineBudgetNeed();
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
 * @brief computes the maximum number of trains that go between two stations simultaneously
 * @complexity O(n * |E|)
 * @param start station where we begin
 * @param finish station where we end up
 */
void Helpy::calculateMaximumTrains(){
    int start = readStation();
    int finish = readStation();

    cout << endl << YELLOW << BREAK << RESET << endl;

}


/**
 * @brief computes the pairs of stations (if more than one) that require the most trains when taking full advantage 
 * @complexity O(n * |E|)
 */
void Helpy::determineMostTrains(){
    int airport = readStation();


    cout << endl << YELLOW << BREAK << RESET << endl;

}

/**
 * @brief computes the top k districts (or municipalities) where management should assign a larger budget for the purchase and maintenance of trains
 * @complexity O(n * |E|)
 * @param number number of districts (or municipalities) to show
 */
void Helpy::determineBudgetNeed(){
    int number = readStation();

    cout << endl << YELLOW << BREAK << RESET << endl;
}
