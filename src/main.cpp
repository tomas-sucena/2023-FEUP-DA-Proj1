#include "network/RailGraph.h"
#include "Reader.h"

int main() {
    Reader reader("../data");
    RailGraph network = reader.read();
    network.getFullPicture();

    return 0;
}
