#ifndef GLOBAL_H
#define GLOBAL_H    //definicja zmiennych globalnych oraz struktur używanych w programie

#include <stdlib.h>
#define BUFFER_SIZE 1024

int strategia;
int kwant_wyw;

typedef struct proces
{
    int id;              
    int priorytet;        
    int czas_wykonania; 
} proces;

proces *kolejka;   //kolejka procesów czekających na wykonanie, być może do zmiany na proces * 
int rozmiar_kolejki;

typedef struct cpu
{
    proces *rdzen;
    int takt_procesora;
    int liczba_rdzeni; 
} cpu;

cpu* new_cpu (int n)
{
    cpu *c = malloc(sizeof(cpu));
    c->takt_procesora = -1;
    c->liczba_rdzeni = n;
    c->rdzen = calloc(n, sizeof(proces));
    for (int i = 0; i < n; ++i)
    {
        c->rdzen[i].czas_wykonania = -1;
        c->rdzen[i].id = -1;
        c->rdzen[i].priorytet = -1;
    }

    return c;    
}

cpu* procesor;

#endif  