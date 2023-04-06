#ifndef DA_TRAINS_UTILS_HPP
#define DA_TRAINS_UTILS_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#define uMap std::unordered_map
#define uSet std::unordered_set

class Utils {
public:
    /**
    * @brief turns all the characters of a string into lowercase or uppercase
    * @complexity O(n)
    * @param s string to be modified
    * @param uppercase bool that indicates if the string should be converted to uppercase
    */
    static void lowercase(std::string& s, bool uppercase = false){
        for (char& c : s){
            c = (uppercase) ? (char) toupper(c) : (char) tolower(c);
        }
    }

    /**
     * @brief takes a user inputted string and modifies it so that it becomes well-written
     * @complexity O(n)
     * @param s string to be modified
     */
    static void properName(std::string& s){
        std::string name;

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
};

#endif //DA_TRAINS_UTILS_HPP
