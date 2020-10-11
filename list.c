#include "list.h"
#include <stdlib.h>
#include <stdio.h>

Liste *newList()
{
    Liste *liste = malloc(sizeof(Liste));
    //Element *element = malloc(sizeof(Element));

    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    //element->debut = 0;
    //element->fin = 0;
    //element->suivant = NULL;
    liste->premier = NULL;
    liste->len = 0;

    return liste;
}

void insertList(Liste *liste, long debut, long fin)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->debut = debut;
    nouveau->fin = fin;
    nouveau->suivant = NULL;

    /* Insertion de l'élément en fin de liste */
    // nouveau->suivant = liste->premier;
    //

    Element *tmp = liste->premier;
    if (tmp != NULL)
    {
        while (tmp->suivant != NULL)
        {
            tmp = tmp->suivant;
        }
        tmp->suivant = nouveau;
    }
    else
    {
        liste->premier = nouveau;
    }

    liste->len = liste->len + 1;
}

void deleteList(Liste *liste)
{
    if (liste == NULL)
    {
        return;
    }

    while (liste->premier != NULL)
    {
        Element *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer);
    }
    free(liste);
}

Element *getList(Liste *liste, int pos)
{
    if (pos < 0 || pos > liste->len)
    {
        pos = 0;
    }
    Element *el = liste->premier;
    int i = 0;
    while (el != NULL)
    {
        if (pos == i)
        {
            return el;
        }
        el = el->suivant;
        i++;
    }
}