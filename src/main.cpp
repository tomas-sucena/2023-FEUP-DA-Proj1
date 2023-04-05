#include <iostream>

#include "Helpy.h"

int main() {
    time_t start, end;
    time(&start);

    Helpy GUI = Helpy();
    GUI.terminal();

    time(&end);
    std::cout << "Execution time: " << double(end - start) << std::endl;

    return 0;
}
