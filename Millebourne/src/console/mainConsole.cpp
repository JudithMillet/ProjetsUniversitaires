#define NBREGLES 3

#include <ncurses.h>
#include <time.h>

#include "txtJeu.h"

using namespace std;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "fr_FR.UTF-8");
    srand(unsigned(time(NULL)));
    initscr();                 /* Start curses mode 		  */
    printw("Hello World !!!"); /* Print Hello World		  */
    printw("\n");
    printw("Votre nom ?\n");
    echo();
    string nom = askstr(20);
    printw("Combien de joueurs ?\n");
    int num = askint(2);

    int tab[NBREGLES] = {0, 0, 0};
    printw("Utiliser les regles par defaut? [o/n] \n");
    std::string c = askstr(1);
    if ((c == "n") || (c == "N"))
    {
        printw("Voulez-vous pouvoir piocher dans la defausse [o/n] \n");
        c = askstr(1);
        if ((c == "n") || (c == "N"))
            tab[0] = 0;
        else
            tab[0] = 1;

        printw("Ne pas poser un feu vert après une carte parade ? [o/n] \n");
        c = askstr(1);
        if ((c == "n") || (c == "N"))
            tab[1] = 0;
        else
            tab[1] = 1;
    }

    //C moche pas de chance
    //On différencie pas les bots et les joueurs humain ca pourrait poser des probs

    Jeu *jouer = new Jeu(num, nom, tab);
    txtInit(jouer);
    txtBoucle(jouer);
    endwin();
    delete jouer;
    return 0;
}
