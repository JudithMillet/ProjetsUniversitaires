#include "PiocheDefausse.h"
#include <time.h>
#include <stdlib.h>
#include <algorithm>

PiocheDefausse::PiocheDefausse()
{
    for (int i = 0; i < NBTYPE; i++)
    {
        nbCarteParTypePioche[i] = 0;
        nbCarteParTypeDefausse[i] = 0;
    }
}

std::stack<Carte> PiocheDefausse::getDefausse()
{
    return defausse;
}

std::stack<Carte> PiocheDefausse::getPioche()
{
    return pioche;
}

void PiocheDefausse::remplirPioche(int tab[])
{
    std::vector<Carte> vec;
    for (int i = 0; i < 19; i++)
    {
        nbCarteParTypePioche[i] = tab[i];
    }

    //Remplir la pioche avec toutes les cartes

    for (int i = 0; i < NBTYPE; i++)
    {
        for (int k = 1; k <= nbCarteParTypePioche[i]; k++)
        {
            vec.push_back(i + 1);
        }
    }

    //Mélange pioche toutes cartes

    melangerVector(vec);

    while (!vec.empty())
    {
        pioche.push(vec.back());
        vec.pop_back();
    }
}

int PiocheDefausse::taillePioche() const
{
    int res = 0;
    for (int i = 0; i < NBTYPE; i++)
        res += nbCarteParTypePioche[i];

    return res;
}

int PiocheDefausse::tailleDefausse() const
{
    int res = 0;
    for (int i = 0; i < NBTYPE; i++)
        res += nbCarteParTypeDefausse[i];

    return res;
}

int PiocheDefausse::cartesDistanceRestantes() const
{
    int res = 0;
    for (int i = Borne25 - 1; i <= Borne200 - 1; i++)
    {
        res += nbCarteParTypeDefausse[i];
        res += nbCarteParTypePioche[i];
    }

    return res;
}

int PiocheDefausse::transfereDefausse()
{
    if (tailleDefausse() == 0) //Si il n'y a rien dans la defausse
    {
        return 3;
    }

    if (taillePioche() != 0) //Si la pioche n'est pas vide
    {
        return 4;
    }

    else
    {
        while (tailleDefausse() != 0)
        {
            pioche.push(defausse.top());
            nbCarteParTypePioche[defausse.top().getValeur() - 1]++;
            nbCarteParTypeDefausse[defausse.top().getValeur() - 1]--;
            defausse.pop();
        }
        return 0;
    }
}

void PiocheDefausse::melangerVector(std::vector<Carte> &vec)
{
    std::random_shuffle(vec.begin(), vec.end());
}

int PiocheDefausse::defausserUneCarte(Carte carte)
{
    defausse.push(carte);
    nbCarteParTypeDefausse[defausse.top().getValeur() - 1]++;
    if (defausse.top().getValeur() == carte.getValeur())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int PiocheDefausse::piocher(Carte &carte, bool b)
{
    if ((taillePioche() == 0) && (tailleDefausse() != 0))
        transfereDefausse();

    if (b)
    {
        if (tailleDefausse() != 0) //Si la defausse n'est pas vide
        {
            carte = defausse.top();
            nbCarteParTypeDefausse[carte.getValeur() - 1]--;
            defausse.pop();
            return 0;
        }

        else
        {
            return 1;
        }
    }

    else
    {
        if (taillePioche() != 0) //Si la pioche n'est pas vide
        {
            carte = pioche.top();
            nbCarteParTypePioche[carte.getValeur() - 1]--;
            pioche.pop();
            return 0;
        }

        else
        {
            return 2;
        }
    }
}

PiocheDefausse::~PiocheDefausse()
{
}