#ifndef ALG_H
#define ALG_H

#include <stdlib.h> //dla qsort
#include <stdio.h>
#include "global.h"

#define WOLNY -1

/*
0 - strategia FCFS (bez wywłaszczania)
1 - strategia SJF (bez wywłaszczania)
3 - strategia RR (z wywłaszczaniem)
5 - szeregowanie priorytetowe z wywłaszczaniem, zadania o takich samych priorytetach szeregowane algorytmem SRTF
*/


int sortuj_czas(const void* a, const void* b)
{
    proces *_a = (proces *) a;
    proces *_b = (proces *) b;

    if (_a->czas_wykonania < _b->czas_wykonania) return -1;
    else if (_a->czas_wykonania > _b->czas_wykonania) return 1;
    return 0;   //przypadek gdy ani to ani to nie rozstrzyga
}


int sortuj_priorytet_czas(const void* a, const void* b)
{
    proces *_a = (proces *) a;
    proces *_b = (proces *) b;

    if (_a->priorytet < _b->priorytet) return -1;
    else if (_a->priorytet > _b->priorytet) return 1;
    else    //jesli priorytet nie rozstrzyga, to decyduje czas
    {
        if (_a->czas_wykonania < _b->czas_wykonania) return -1;
        else if (_a->czas_wykonania > _b->czas_wykonania) return 1;
    }
    return 0;   //przypadek gdy ani to ani to nie rozstrzyga
}

void stan_procesora()
{
    printf("%d ", procesor->takt_procesora);
    for (int i = 0; i < procesor->liczba_rdzeni; ++i)
    {
        printf("%d ", procesor->rdzen[i].id);
    }
    printf("\n");
}

void dodaj_na_procesor()
{
    for (int i = 0; i < procesor->liczba_rdzeni; ++i)
    {
        if (procesor->rdzen[i].id == WOLNY) //jeśli rdzeń jest wolny dodajemy zadanie z kolejki...
        {
            if(rozmiar_kolejki > 0)         //...chyba ze rowniez ona jest pusta
            {
                procesor->rdzen[i] = kolejka[0];
                for(int b = 0; b < rozmiar_kolejki ; b++) //"przesuwamy" kolejke
                {
                    kolejka[b] = kolejka[b + 1];
                }                
                rozmiar_kolejki--;
            }
        }   
    }
}

int procesor_wolny()
{
    for (int i = 0; i < procesor->liczba_rdzeni; ++i)
    {
        if (procesor->rdzen[i].id != WOLNY)
        {
            return 0;
        }
    }
    return 1;
}


void rr()
{
    
    //wywlaszczamy zadania z procesora co kwant czasu 
    if ((procesor->takt_procesora % kwant_wyw) == 0)
    {
        for (int i = 0; i < procesor->liczba_rdzeni; ++i)
        {
            if (procesor->rdzen[i].id != WOLNY) //jesli na ktoryms procesorze mamy zadanie
            {
                // alokacja dla procesu zrzucanego z procesora    
                kolejka = realloc(kolejka, (rozmiar_kolejki + 1) * sizeof(proces));                
                kolejka[rozmiar_kolejki] = procesor->rdzen[i]; 
                procesor->rdzen[i].id = WOLNY;
                procesor->rdzen[i].czas_wykonania= WOLNY;
                procesor->rdzen[i].priorytet = WOLNY;
                rozmiar_kolejki++;
            }   
        }
    }

    dodaj_na_procesor();
}


void sjf()  //dodajemy zadania o najkrotszym czasie wykonania
{
    qsort(kolejka, rozmiar_kolejki, sizeof(proces), sortuj_czas); //sortujemy zadania pod wzgledem czasu 

    dodaj_na_procesor();
}

void fcfs() //jeśli są wolne rdzenie to dodajemy zadanie
{
    dodaj_na_procesor();
}

void psrtf()    //najpierw decyduje priorytet, jak jest remis to czas
{
    //wywlaszczamy procesy
    if ((procesor->takt_procesora % kwant_wyw) == 0)
    {
        for (int i = 0; i < procesor->liczba_rdzeni; ++i)
        {
            if (procesor->rdzen[i].id != WOLNY) //jesli na ktoryms procesorze mamy zadanie
            {
                // alokacja dla procesu zrzucanego z procesora    
                kolejka = realloc(kolejka, (rozmiar_kolejki + 1) * sizeof(proces));                
                kolejka[rozmiar_kolejki] = procesor->rdzen[i]; 
                procesor->rdzen[i].id = WOLNY;
                procesor->rdzen[i].czas_wykonania= WOLNY;
                procesor->rdzen[i].priorytet = WOLNY;
                rozmiar_kolejki++;
            }   
        }
    }
    
    //sortujemy kolejke
    qsort(kolejka, rozmiar_kolejki, sizeof(proces), sortuj_priorytet_czas);  
    
    //dodajemy zadania 
    dodaj_na_procesor();
}

void szereguj()
{
    procesor->takt_procesora++;

    for (int i = 0; i < procesor->liczba_rdzeni; ++i)
    {
        if (procesor->rdzen[i].czas_wykonania == 0) //zadanie sie wykonalo, zatem zwalniamy rdzen
        {
            procesor->rdzen[i].id = WOLNY;
            procesor->rdzen[i].czas_wykonania = WOLNY;
            procesor->rdzen[i].priorytet = WOLNY;
        }
    }

    for (int i = 0; i < procesor->liczba_rdzeni; ++i)
    {
        if (procesor->rdzen[i].id == WOLNY) //wolny rdzen na ktory mozemy przeniesc zadania z dalszych rdzeni
        {
            for (int j = i; j < procesor->liczba_rdzeni; ++j)
            {
                if (procesor->rdzen[j].id != WOLNY)     //jesli jest jakies zadanie to przerzucamy je 
                {
                    proces temp = procesor->rdzen[i];
                    procesor->rdzen[i] = procesor->rdzen[j];
                    procesor->rdzen[j] = temp;
                }
            }
        }
    }

    switch (strategia)
    {
    case 0:
        fcfs();
        break;

    case 1:
        sjf();
        break;

    case 3:
        rr();
        break;

    case 5:
        psrtf();
        break;
    
    default:
        exit(0);
        break;
    }

    for (int i = 0; i < procesor->liczba_rdzeni; ++i)
    {
        if (procesor->rdzen[i].id != WOLNY)
        {
            procesor->rdzen[i].czas_wykonania--;
        }
    }
    
}


#endif