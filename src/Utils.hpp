#ifndef DA_TRAINS_UTILS_HPP
#define DA_TRAINS_UTILS_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>

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
};

#endif //DA_TRAINS_UTILS_HPP
