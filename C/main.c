#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "global.h"
#include "algorytmy.h"

// wywolanie - ./a.out <strategia> <liczba procesorów> <czas wywlaszczania> < <plik wejsciowy>  > <plik wyjsciowy>
// gcc main.c global.h algorytmy.h

int main(int argc, char const *argv[])
{
    int liczba_rdzeni = 0; 
    char buffer[BUFFER_SIZE];    
    int temp_id, temp_priorytet, temp_czas;

    switch (argc)
    {
    case 2: // tylko strategia
        liczba_rdzeni = 1;
        kwant_wyw = 1;
        break;

    case 3: // strategia + ilosc procesorow
        liczba_rdzeni = atoi(argv[2]);  // string na int
        kwant_wyw = 1;
        break;

    case 4: // wszystko
        liczba_rdzeni = atoi(argv[2]);
        kwant_wyw = atoi(argv[3]);
        break;

    default: // nieprawidlowe wywolanie
    {
        printf("nieprawidlowe wywolanie!\n");
        exit(0);
    }
    }

    strategia = atoi(argv[1]);


    if (liczba_rdzeni < 1)
    {
        printf("nieprawidlowe wywolanie: liczba rdzeni < 1\n");
        exit(0);
    }

    if (kwant_wyw < 1)
    {
        printf("nieprawidlowe wywolanie: kwant wywlaszczania < 1\n");
        exit(0);
    }

    procesor = new_cpu(liczba_rdzeni);

    rozmiar_kolejki = 0;    
    
    while (fgets(buffer, BUFFER_SIZE, stdin))
    {
        int offset = 0;     // offset odkad ma byc czytany bufor
        int przeczytane_znaki = 0; 

        sscanf(buffer, " %*d %n", &offset); // timestamp do pominięcia

        // wczytaj id, priorytet, czas wykonywania az do konca linii 
        while (sscanf(buffer + offset, " %d %d %d %n [^\n]", &temp_id, &temp_priorytet, &temp_czas, &przeczytane_znaki) == 3)
        {
            offset += przeczytane_znaki;

            // alokacja dla kolejnego procesu
            kolejka = realloc(kolejka, (rozmiar_kolejki + 1) * sizeof(proces));
            kolejka[rozmiar_kolejki].id = temp_id;
            kolejka[rozmiar_kolejki].priorytet = temp_priorytet;
            kolejka[rozmiar_kolejki].czas_wykonania = temp_czas;

            rozmiar_kolejki++;
        }
    
    szereguj();
    
    stan_procesora();

    }


    while(!procesor_wolny())   //po skonczeniu sie danych szeregujemy zadania do momentu kiedy procesor jest pusty
    {
    szereguj();
    
    stan_procesora();
    }


    return 0;
}
