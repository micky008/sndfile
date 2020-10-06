#include "list.h"
#include <stdlib.h>
#include <stdio.h>

Liste *newList()
{
    Liste *liste = malloc(sizeof(Liste));
    Element *element = malloc(sizeof(Element));

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->debut = 0;
    element->fin = 0;
    element->suivant = NULL;
    liste->dernier = element;
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

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->dernier;
    liste->dernier = nouveau;
    liste->len = liste->len++;
}

void deleteList(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->dernier != NULL)
    {
        Element *aSupprimer = liste->dernier;
        liste->dernier = liste->dernier->suivant;
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
    Element *el = liste->dernier;
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