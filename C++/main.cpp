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
 
    switch (argc)   //wczytanie argumentów w zależności od tego ile ich zostało podanych 
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
        std::cerr << "niepoprawne wywołanie" << std::endl;
        return -1;
        break;
    }

    if (strategy > 6 || strategy < 0)
    {
        printf("niepoprawne wywołanie: strategia > 6 lub < 1\n");
        return -1;
    }

    if (processors_number < 1)
    {
        printf("niepoprawne wywołanie: liczba rdzeni < 1\n");
        return -1;
    }

    if (dispossession_time < 1)
    {
        printf("niepoprawne wywołanie:: wprowadzono kwant wywlaszczania < 1\n");
        return -1;
    }
 

    scheduler schd(processors_number, strategy, dispossession_time);    //stworzenie schedulera
    schd.run(); //uruchomienie go


    return 0;
}
