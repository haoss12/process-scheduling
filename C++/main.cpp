#include <sstream>
#include <string>
#include <vector>
#include "strategies.hh"


int main(int argc, char const *argv[])
{
    std::string line;
    int strategy = 0;
    int processors_number = 1;
    int dispossession_time = 1;
 
    switch (argc)
    {
    case 2:
        strategy = atoi(argv[1]);
        break;

    case 3:
        strategy = atoi(argv[1]);
        processors_number = atoi(argv[2]);
        break;

    case 4:
        strategy = atoi(argv[1]);
        processors_number = atoi(argv[2]);
        dispossession_time = atoi(argv[3]);
        break;

    default:
        std::cerr << "incorrect program call!" << std::endl;
        return -1;
        break;
    }

    if (strategy > 6 || strategy < 0)
    {
        printf("incorrect program call: strategy > 6 or < 1\n");
        return -1;
    }

    if (processors_number < 1)
    {
        printf("incorrect program call: number of cpus < 1\n");
        return -1;
    }

    if (dispossession_time < 1)
    {
        printf("wywolania: wprowadzono kwant wywlaszczania < 1\n");
        return -1;
    }
 

    scheduler schd(processors_number, strategy, dispossession_time);
    schd.run();


    return 0;
}


/*

time    p1  p2  p3
0       -1  1   4


*/