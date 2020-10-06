#ifndef __MA_LISTE
#define __MA_LISTE

typedef struct Element Element;
struct Element
{
    long debut;
    long fin;
    Element *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    Element *dernier;
    int len;
};

Liste *newList();
void insertList(Liste *liste, long debut, long fin);

//Free les elements ET la liste.
void deleteList(Liste *liste);

//si pos <= 0  || pos > liste->len  retourne le dernier de la liste.
Element *getList(Liste *liste, int pos);

#endif