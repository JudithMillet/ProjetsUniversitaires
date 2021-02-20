
#include <core/Jeu.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    Carte testCarte;
    testCarte.testRegression();
    Joueur testJoueur;
    testJoueur.testRegression();
    std::cout << "C'est bon" << std::endl;
    return 0;
}