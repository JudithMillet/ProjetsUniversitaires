#include "Jeu.h"

Jeu::Jeu(int nombreJoueurs, std::string nom, int tab[])
{
    aQuiLeTour = 0;
    nbrJoueurs = nombreJoueurs;
    for (int i = 0; i < NBREGLES; i++)
        regles[i] = tab[i];
    //TODO:check nbrJoueur sup à 1
    bool vert = (regles[1] == 0) ? false : true;
    joueurs.push_back(Joueur(nom, false, vert));
    for (int i = 1; i < nbrJoueurs; i++)
    {
        joueurs.push_back(Joueur("Joueur", true, vert));
    }
}

Jeu::~Jeu()
{
    //détruire joueurs
}

std::vector<dataJoueur> Jeu::getScoreJoueurs() const
{
    std::vector<dataJoueur> tabJoueursScore;
    for (int i = 0; i < nbrJoueurs; i++)
    {
        struct dataJoueur dj;
        dj.id = i;
        dj.score = joueurs.at(i).getScore();
        tabJoueursScore.push_back(dj);
    }

    std::sort(tabJoueursScore.begin(), tabJoueursScore.end());
    return tabJoueursScore;
}

std::vector<Joueur> Jeu::getJoueurs()
{
    return joueurs;
}

int Jeu::getnbrJoueurs() const
{
    return nbrJoueurs;
}

PiocheDefausse Jeu::getPD() const
{
    return PD;
}

std::array<int, 3> Jeu::getTableauRegle() const
{
    return regles;
}

int Jeu::initialiserLaPartie(int tab[])
{
    Carte c;
    PD.remplirPioche(tab);
    int r = -1;
    for (int i = 0; i < nbrJoueurs; i++)
    {
        for (int j = 0; j < 6; j++) //distribuer 6 cartes à chaque joueur
        {
            r = PD.piocher(c, false);
            if (r == 0)
            {
                r = joueurs.at(i).ajouterCarteMain(c);
            }
            else
                return r;
        }
    }
    return r;
}

int Jeu::poserUneCarte(int p, int x, int y)
{
    //int p --> Position dans la main
    //int x --> Joueur
    //int y --> Destinataire
    // Quand y == -1 -> On pose dans la defausse
    //en fonction d'où elle va être posée --> on l'enlève de la pioche
    //si bool = true --> appel de DefausseCarte(c) return 0;
    //-53 Joueur impossible
    //-54 Destinataire impossible
    //+55 Carte impossible
    if (x > nbrJoueurs - 1 || x < 0)
        return -53;
    if (y > nbrJoueurs - 1 || y < -1)
        return -54;
    if (p > 6 || p < -1)
        return -55;
    std::array<Carte, 7> cem = joueurs[x].getCarteEnMain();
    Carte c = cem[p];
    int val = c.getValeur();
    if (val == Vide)
        return -56;

    // int score = joueurs.at(x).getScore();
    bool botte = false;
    int ret = -50;

    //N'IMPORTE QUI PEUT JOUER UNE BOTTE N'IMPORTE QUAND
    if ((val == BotteAsDuVolant) || (val == BotteCiterne) || (val == BotteIncrevable) || (val == BottePrioritaire)) //Carte Botte
    {
        if (x == y)
        {
            ret = joueurs.at(y).ajouterCarteBotte(c);
            if (ret == 0)
                botte = true;
        }
        else
            return -51;
    }

    else if (x == aQuiLeTour)
    {

        if (y == -1)
        {
            ret = PD.defausserUneCarte(c);
        }

        else if ((val == AttaqueFeuRouge) || (val == AttaquePanneEssence) || (val == AttaqueCrevaison) || (val == AttaqueAccident)) //Carte Attaque
        {
            if (x != y)
                ret = joueurs.at(y).ajouterCarteAttaque(c);
            else
                return -52;
        }
        else if ((val == ParadeFeuVert) || (val == ParadeEssence) || (val == ParadeRoueDeSecours) || (val == ParadeReparations)) //Carte Parade
        {
            if (x == y)
                ret = joueurs.at(y).ajouterCarteParade(c);
            else
                return -51;
        }
        else if (val == AttaqueLimiteDeVitesse) //Carte Limitation de Vitesse
        {
            if (x != y)
                ret = joueurs.at(y).ajouterCarteLimitationVitesse(c);
            else
                return -52;
        }
        else if (val == ParadeFinDeLimite) //Carte Fin de Limitation
        {
            if (x == y)
                ret = joueurs.at(y).ajouterCarteFinLimitationVitesse(c);
            else
                return -51;
        }

        else if ((val == Borne25) || (val == Borne50) || (val == Borne75) || (val == Borne100) || (val == Borne200))
        {
            if (x == y)
                ret = joueurs.at(y).ajouterCarteDistance(c);
            else
                return -51;
        }
    }

    if (ret == 0)
    {
        if ((botte) && (x != aQuiLeTour))
            aQuiLeTour = x;
        else
        {
            aQuiLeTour++;
            aQuiLeTour = aQuiLeTour % nbrJoueurs;
        }

        return joueurs.at(x).supprimerCarteMain(c); //vérification que la carte a bien été posée
    }

    return ret;
}

int Jeu::piocherUneCarte(int x, bool b)
{
    //enlever la dernière Carte de la pioche
    int ret;
    Carte nvlCarte;
    ret = PD.piocher(nvlCarte, b);

    if (ret == 0)
        return joueurs.at(x).ajouterCarteMain(nvlCarte);
    return ret;
}

std::vector<int> Jeu::gagnant(bool fin) //Il faudra verifier à chaque tour si gagnant != 0;
{
    std::vector<int> gagnant;

    for (int i = 0; i < nbrJoueurs; i++)
    {
        if (joueurs.at(i).getScore() == 1000)
            gagnant.push_back(i);
    }

    if (gagnant.empty())
    {
        int cartesDist = 0;
        for (int i = 0; i < nbrJoueurs; i++)
        {
            for (unsigned int j = 0; j < joueurs.at(i).getCarteEnMain().size(); j++)
            {
                int carteMain = joueurs.at(i).getCarteEnMain().at(j).getValeur();
                if ((carteMain == Borne25) || (carteMain == Borne50) || (carteMain == Borne75) || (carteMain == Borne100) || (carteMain == Borne200))
                    cartesDist++;
            }
        }

        cartesDist += PD.cartesDistanceRestantes();
        if ((cartesDist == 0) || ((PD.tailleDefausse() == 0) && (PD.taillePioche() == 0)) || fin)
        {
            int max = 0;
            for (int i = 0; i < nbrJoueurs; i++)
            {
                if (max == joueurs.at(i).getScore())
                {
                    gagnant.push_back(i);
                }

                else if (max < joueurs.at(i).getScore())
                {
                    max = joueurs.at(i).getScore();
                    gagnant.clear();
                    gagnant.push_back(i);
                }
            }
        }
    }
    return gagnant;
}

int Jeu::jouerBot(int numeroBot)
{
    if (joueurs.at(numeroBot).estBot())
    {
        int ret;
        int retPioche;
        bool prio = false;
        bool citerne = false;
        bool increv = false;
        bool asVolant = false;

        retPioche = piocherUneCarte(numeroBot, false);
        if (retPioche != 0)
            return retPioche;

        std::array<Carte, 7> cartesEnMain = joueurs.at(numeroBot).getCarteEnMain();

        Carte cPoseeBataille = joueurs.at(numeroBot).getCartePoseeBataille();
        std::vector<Carte> cPoseeBotte = joueurs.at(numeroBot).getCartePoseeBotte();

        for (std::vector<Carte>::iterator it = cPoseeBotte.begin(); it != cPoseeBotte.end(); ++it)
        {
            if (it->getValeur() == BotteAsDuVolant)
            {
                prio = true;
            }
        }

        for (std::vector<Carte>::iterator it = cPoseeBotte.begin(); it != cPoseeBotte.end(); ++it)
        {
            if (it->getValeur() == BotteCiterne)
            {
                citerne = true;
            }
        }

        for (std::vector<Carte>::iterator it = cPoseeBotte.begin(); it != cPoseeBotte.end(); ++it)
        {
            if (it->getValeur() == BotteIncrevable)
            {
                increv = true;
            }
        }

        for (std::vector<Carte>::iterator it = cPoseeBotte.begin(); it != cPoseeBotte.end(); ++it)
        {
            if (it->getValeur() == BotteAsDuVolant)
            {
                asVolant = true;
            }
        }

        ///Si carte BOTTE
        for (int k = 0; k < 7; k++)
        {
            if ((cartesEnMain[k].getValeur() == BotteAsDuVolant) || (cartesEnMain[k].getValeur() == BotteCiterne) || (cartesEnMain[k].getValeur() == BotteIncrevable) || (cartesEnMain[k].getValeur() == BotteAsDuVolant))
            {
                ret = poserUneCarte(k, numeroBot, numeroBot);
                if (ret == 0)
                    return 1;
            }
        }

        //si feu vert
        //si pile de bataille vide

        if ((cPoseeBataille.getValeur() == Vide) || ((cPoseeBataille.getValeur() == ParadeEssence) || (cPoseeBataille.getValeur() == ParadeRoueDeSecours) || (cPoseeBataille.getValeur() == ParadeReparations) && regles[1] == 0) && (!prio))
        {
            for (int n = 0; n < 7; n++)
            {
                if (cartesEnMain[n].getValeur() == ParadeFeuVert)
                {
                    ret = poserUneCarte(n, numeroBot, numeroBot);
                }
            }
        }

        ///Sinon si carte attaque posee --> carte PARADE
        for (int j = 0; j < 7; j++)
        {
            switch (cPoseeBataille.getValeur())
            {
            case AttaqueFeuRouge:
                if (cartesEnMain[j] == ParadeFeuVert)
                {
                    ret = poserUneCarte(j, numeroBot, numeroBot);
                    if (ret == 0)
                        return 10;
                }
                break;

            case AttaqueLimiteDeVitesse:
                if (!prio)
                    if (cartesEnMain[j] == ParadeFinDeLimite)
                    {
                        ret = poserUneCarte(j, numeroBot, numeroBot);
                        if (ret == 0)
                            return 11;
                    }
                break;

            case AttaquePanneEssence:
                if (!citerne)
                    if (cartesEnMain[j] == ParadeEssence)
                    {
                        ret = poserUneCarte(j, numeroBot, numeroBot);
                        if (ret == 0)
                            return 12;
                    }
                break;

            case AttaqueCrevaison:
                if (!increv)
                    if (cartesEnMain[j] == ParadeRoueDeSecours)
                    {
                        ret = poserUneCarte(j, numeroBot, numeroBot);
                        if (ret == 0)
                            return 13;
                    }
                break;

            case AttaqueAccident:
                if (!asVolant)
                    if (cartesEnMain[j] == ParadeReparations)
                    {
                        ret = poserUneCarte(j, numeroBot, numeroBot);
                        if (ret == 0)
                            return 14;
                    }
                break;
            }
        }

        ///Sinon si carte distance --> pose la plus grande sans dépasser 1000
        if ((cPoseeBataille.getValeur() == ParadeFeuVert) || prio || regles[1] == 1)
        {
            for (int i = 0; i < 7; i++)
            {
                switch (cartesEnMain[i].getValeur())
                {
                case Borne200:
                    if (joueurs.at(numeroBot).getScore() <= 800)
                    {
                        ret = poserUneCarte(i, numeroBot, numeroBot);
                        if (ret == 0)
                            return 19;
                    }
                    break;

                case Borne100:
                    if (joueurs.at(numeroBot).getScore() <= 900)
                    {
                        ret = poserUneCarte(i, numeroBot, numeroBot);
                        if (ret == 0)
                            return 18;
                    }
                    break;

                case Borne75:
                    if (joueurs.at(numeroBot).getScore() <= 925)
                    {
                        ret = poserUneCarte(i, numeroBot, numeroBot);
                        if (ret == 0)
                            return 17;
                    }
                    break;

                case Borne50:
                    if (joueurs.at(numeroBot).getScore() <= 950)
                    {
                        ret = poserUneCarte(i, numeroBot, numeroBot);
                        if (ret == 0)
                            return 16;
                    }
                    break;

                case Borne25:
                    if (joueurs.at(numeroBot).getScore() <= 975)
                    {
                        ret = poserUneCarte(i, numeroBot, numeroBot);
                        if (ret == 0)
                            return 15;
                    }
                    break;
                }
            }
        }
        //else
        {

            ///Sinon si carte attaque --> on attaque celui qui a le plus grand score
            std::vector<dataJoueur> ordreDesScores = getScoreJoueurs();

            int meilleur = 0;

            //TODO modifier pour d'abord regarder les cartes en main
            while (meilleur != nbrJoueurs)
            {
                if (numeroBot == ordreDesScores.at(meilleur).id)
                {
                    meilleur++;
                }
                else
                {
                    if ((joueurs.at(meilleur).getCartePoseeBataille() != AttaqueFeuRouge) && (joueurs.at(meilleur).getCartePoseeBataille() != AttaqueLimiteDeVitesse) && (joueurs.at(meilleur).getCartePoseeBataille() != AttaquePanneEssence) && (joueurs.at(meilleur).getCartePoseeBataille() != AttaqueCrevaison) && (joueurs.at(meilleur).getCartePoseeBataille() != AttaqueAccident))
                    {
                        for (int m = 0; m < 7; m++)
                        {
                            if ((cartesEnMain[m].getValeur() == AttaqueFeuRouge) || (cartesEnMain[m].getValeur() == AttaqueLimiteDeVitesse) || (cartesEnMain[m].getValeur() == AttaquePanneEssence) || (cartesEnMain[m].getValeur() == AttaqueCrevaison) || (cartesEnMain[m].getValeur() == AttaqueAccident))
                            {
                                ret = poserUneCarte(m, numeroBot, meilleur);
                                if (ret == 0)
                                    return 5;
                            }
                        }
                    }

                    meilleur++;
                }
            }

            //Sinon se défausse --- TODO gestion ordre defausse
            int carte = 0;
            do
            {
                ret = poserUneCarte(carte, numeroBot, -1);
                carte++;
            } while ((ret != 0) && (carte < 7));

            return ret;
        }
    }
    return -100;
}

/*
    verifie que c'est un bot sinon return ret;
    piocherune carte
    si botte pose en priorité
    sil doit se défendre --> sil a la carte défense --> il l'a pose

    AGRESSIVITE ?
    
    si distance --> pose la plus grande sans dépasser 1000

    si attaque --> attaque celui qui a le plus grand score
    si personne a deja une attaque --> il la pose à une autre tant que tout le monde à une attaque
    sinon se defausse d'une carte en double ou petite distance 
    ou si botte --> jeter carte defense correspondante



    retour :
        -1 : le joueur n'est pas un bot
        0 : rien à faire 
        1 : carte BOTTE posée
        5 : carte ATTAQUE posée
        10 : carte ParadeFeuVert posée
        11 : carte ParadeFinDeLimite posée
        12 : carte ParadeEssence posée
        13 : carte ParadeRoueDeSecours posée
        14 : carte ParadeReparations posée
        15 : carte Borne25 posée
        16 : carte Borne50 posée
        17 : carte Borne75 posée
        18 : carte Borne100 posée
        19 : carte Borne200 posée

*/

int Jeu::getAQuiLeTour() const
{
    return aQuiLeTour;
}

std::string Jeu::jeuAffichageClassement( bool fin)
{
    std::string chaine;
    chaine = "Classement : ";
    for (unsigned int i = 0; i < gagnant(fin).size(); i++)
    {
        chaine += "Numero" + std::to_string(i + 1) + " " + joueurs.at(gagnant(fin).at(i)).getNom().c_str() + "  ";
    }

    return chaine;
}