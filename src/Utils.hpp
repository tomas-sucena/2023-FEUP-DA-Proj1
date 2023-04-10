#ifndef DA_TRAINS_UTILS_HPP
#define DA_TRAINS_UTILS_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "libfort/fort.hpp"

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
            // prepositions
            if (s.size() == 2 && s[0] == 'd'){
                name += ' ' + s;
                continue;
            }

            s[0] = (char) toupper(s[0]);

            for (int i = 1; i < s.size(); i++)
                s[i] = (char) tolower(s[i]);

            if (!first) name += ' ';

            name += s;
            first = false;
        }

        s = name;
    }

    /**
     * @brief creates a fort::utf8_table used for displaying information in the terminal
     * @param columnNames list containing the name of each column of the table
     * @return fort::utf8_table object
     */
    static fort::utf8_table createTable(const std::list<std::string>& columnNames){
        fort::utf8_table table;

        table.set_border_style(FT_NICE_STYLE);
        table.row(0).set_cell_content_text_style(fort::text_style::bold);
        table.row(0).set_cell_content_fg_color(fort::color::yellow);
        table << fort::header;

        auto it = columnNames.begin();
        for (int i = 0; it != columnNames.end(); ++i){
            table << *it++;
            table.column(i).set_cell_text_align(fort::text_align::center);
        }

        table << fort::endr;
        return table;
    }
};

#endif //DA_TRAINS_UTILS_HPP
