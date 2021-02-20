#include <ncurses.h>
#include <string>
#include "txtJeu.h"

int askint(const int nbrChiffre)
{
    char str[nbrChiffre + 1];
    getnstr(str, nbrChiffre);
    return atoi(str);
}

std::string askstr(const int nbrCar)
{
    char num[nbrCar + 1];
    getnstr(num, nbrCar);
    return std::string(num);
}

void txtInit(Jeu *jeu)
{
    std::string c;
    bool valide;
    int tab[19] = {1, 1, 1, 1, 5, 4, 3, 3, 3, 14, 6, 6, 6, 6, 10, 10, 10, 12, 4};
    int temp;

    printw("Voulez vous personnalisez votre paquet ? [o/n] \n");
    c = askstr(1);
    if ((c == "o") || (c == "O") || (c == "y") || (c == "Y"))
    {
        printw("\n");
        int valeur_total = 0;
        for (int i = 0; i < 19; i++)
        {
            printw(("Combien de " + Carte(i + 1).getNom() + " voulez-vous ? \n").c_str());
            do
            {
                valide = false;
                temp = askint(2);
                if (temp >= 0)
                {
                    tab[i] = temp;
                    printw("La valeur est valide, la(les) carte(s) a(ont) été ajouté \n");
                    valeur_total += temp;
                    valide = true;
                }
                else
                    printw("Valeur invalide, reccomencez svp \n");
            } while (valide == false);
        }
    }
    jeu->initialiserLaPartie(tab);
}

void txtBoucle(Jeu *jeu)
{
    std::vector<int> classement;
    bool ok = true;
    int carte, cible, ret, p;
    int fin;
    do
    {
        //jeu afficher
        /*
        #ifdef _WIN32
        Sleep(100);
		#else
		usleep(100000);
        #endif // WIN32
		*/
        //C'est pas de la programation évènementielle foudra que je change ça
        if (jeu->getAQuiLeTour() == 0)
        {
            if ((jeu->getTableauRegle().at(0) == 1) && (!jeu->getPD().getDefausse().empty()))
            {
                printw("\nPiocher dans la pioche (0) ou la défausse (1) ? \n");
                refresh();
                p = askint(1);
                if (p == 0)
                    jeu->piocherUneCarte(0, false);
                else
                    jeu->piocherUneCarte(0, true);
            }

            else
                jeu->piocherUneCarte(0, false);
            do
            {
                do
                {
                    txtAffichageJeu(jeu);
                    printw("\nChoisis une carte que tu veux jouer stp \n");
                    refresh();
                    carte = askint(1);
                    printw("\nCimer \n");
                    refresh();
                } while (carte > 6 || carte < 0);
                do
                {
                    printw("\nChoisis une cible sur qui tu veux jouer ta carte (0 pour toi même, -1 pour la défausser) \n");
                    refresh();
                    cible = askint(2);
                    printw("\nCimer \n");
                    refresh();
                } while (cible > jeu->getnbrJoueurs() || cible < -1);
                clear();
                ret = jeu->poserUneCarte(carte, jeu->getAQuiLeTour(), cible);
            } while (ret != 0);
            txtAffichageJeu(jeu);

            /* 			switch (c)
			{
			case '1':
				//onfait un truc
				break;
			case 'q':
				ok = false;
				break;
			} */
        }
        else
        {
            txtAffichageJeu(jeu);
            jeu->jouerBot(jeu->getAQuiLeTour());
        }

        txtAffichageJeu(jeu);
        classement = jeu->gagnant(false);
        if (!classement.empty())
        {
            printw(jeu->jeuAffichageClassement(false).c_str());
            refresh();
            fin = askint(1);
            ok = !ok;
        }

    } while (ok);
}

void txtAffichageJeu(Jeu *p)
{
    clear();
    /*
    clear(); --> nettoyer la fenetre
    int printw(string) --> afficher un haut à gauche
    int mvaddchstr(int y, int x, const chtype *chstr) --> afficher un string à la position x,y
    int move(int y, int x) --> bouger à la position x,y
    int refresh(); --> refresh la fenetre
   */

    //WINDOW * fenetre;
    //fenetre = newwin(250,200,4,0);
    printw("\n");
    for (int k = 0; k < COLS; k++)
    {
        printw("_");
    }
    printw("\n");
    for (int i = 0; i < p->getnbrJoueurs(); i++)
    {
        if (p->getAQuiLeTour() == i)
            printw("C'est ton tour ");

        printw(p->getJoueurs().at(i).getNom().c_str());
        printw("\n");
        printw(p->getJoueurs().at(i).joueurAffichageConsole().c_str());
        printw("\n");
        if (!(p->getJoueurs().at(i).estBot()))
        {
            printw(p->getJoueurs().at(i).joueurAffichageMain().c_str());
            printw("\n");
        }
        printw("\n");
    }

    if (!p->getPD().getDefausse().empty())
    {
        printw("\nDefausse :\n");
        printw(p->getPD().getDefausse().top().getNomDisplay().c_str());
        printw("\n");
    }

    for (int k = 0; k < COLS; k++)
    {
        printw("|");
    }
    refresh();
    //delwin(fenetre);
}