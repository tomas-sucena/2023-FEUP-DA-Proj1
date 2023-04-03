#include <iostream>

#include "network/RailGraph.h"
#include "Reader.h"
#include "Helpy.h"

int main() {
    Reader reader("../data");
    RailGraph network = reader.read();
    std::cout << network.getFullPicture();
    Helpy GUI = Helpy(network, reader.getStations());
    GUI.terminal();

    return 0;
}
