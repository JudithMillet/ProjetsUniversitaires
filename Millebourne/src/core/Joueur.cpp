#include "Joueur.h"

//Contructeur par défaut
Joueur::Joueur()
{
    nom = "Joueur"; // met le nom par défaut du joueur sur Joueur
    std::array<Carte, 7> carteEnMain;
    carteEnMain.fill(Carte());
    bot = true; // Par défaut, on supposera que c'est un bot
    regleVert = false;
}

//Constructeur avec paramètre d'initialisation
Joueur::Joueur(std::string leNom, bool BOOL, bool vert)
{

    nom = leNom; // Met le nom passé en paramètre
    std::array<Carte, 7> carteEnMain;
    carteEnMain.fill(Carte());

    bot = BOOL; // Désigne si oui ou non c'est un bot
    regleVert = vert;
}

//Le destructeur
Joueur::~Joueur()
{
}

bool Joueur::estBot()
{
    return bot;
}

int Joueur::getNbrCarteEnMain() const
{
    int res = 0; // on crée la variable du résultat
    for (int i = 0; i < 6; i++)
    {
        if (carteEnMain[i].getValeur() != Vide)
            res += 1; //si la case du tableau a une valeur différente de -1 c'est qu'il y a une carte donc on peut rajouter 1 .a res
    }
    return res;
}

std::array<Carte, 7> Joueur::getCarteEnMain() const
{
    return carteEnMain;
}

std::vector<Carte> Joueur::getCartePoseeBotte()
{
    if (cartePoseeBotte.empty())
    {
        std::vector<Carte> temp;
        temp.push_back(Carte());
        return temp;
    }
    else
        return cartePoseeBotte;
}

Carte Joueur::getCartePoseeLimitationVitesse() const
{
    if (cartePoseeLimitationVitesse.empty())
    {
        return Carte();
    }
    else
        return cartePoseeLimitationVitesse.back();
}

Carte Joueur::getCartePoseeBataille() const
{
    if (cartePoseeBataille.empty())
    {
        return Carte();
    }
    else
        return cartePoseeBataille.back();
}

std::vector<Carte> Joueur::getCartePoseeDistance()
{
    if (cartePoseeDistance.empty())
    {
        std::vector<Carte> tempvec;
        tempvec.push_back(Carte());
        return tempvec;
    }
    else
        return cartePoseeDistance;
}

bool Joueur::peutJouerUneCarte(const Carte c)
{
    int id = c.getValeur();
    Carte derniere_bataille = getCartePoseeBataille();
    int id_db = derniere_bataille.getValeur();
    Carte derniere_vitesse = getCartePoseeLimitationVitesse();
    int id_dv = derniere_vitesse.getValeur();
    std::vector<Carte> distance = getCartePoseeDistance();
    int score = getScore();

    bool citerne = false;
    bool asVolant = false;
    bool increv = false;
    bool prio = false;

    for (std::vector<Carte>::iterator it = cartePoseeBotte.begin(); it != cartePoseeBotte.end(); ++it)
    {
        if (it->getValeur() == BotteCiterne)
        {
            citerne = true;
        }
    }

    for (std::vector<Carte>::iterator it = cartePoseeBotte.begin(); it != cartePoseeBotte.end(); ++it)
    {
        if (it->getValeur() == BotteAsDuVolant)
        {
            asVolant = true;
        }
    }

    for (std::vector<Carte>::iterator it = cartePoseeBotte.begin(); it != cartePoseeBotte.end(); ++it)
    {
        if (it->getValeur() == BotteIncrevable)
        {
            increv = true;
        }
    }

    for (std::vector<Carte>::iterator it = cartePoseeBotte.begin(); it != cartePoseeBotte.end(); ++it)
    {
        if (it->getValeur() == BottePrioritaire)
        {
            prio = true;
        }
    }
    if ((id == BottePrioritaire) || (id == BotteCiterne) || (id == BotteIncrevable) || (id == BotteAsDuVolant))
    {
        return true;
    }
    if (id == AttaqueFeuRouge)
    {
        if (prio)
            return false;

        else if ((id_db == ParadeFeuVert) || (((id_db == ParadeEssence) || (id_db == ParadeReparations) || (id_db == ParadeRoueDeSecours)) && regleVert))
            return true;
        else
            return false;
    }
    if (id == AttaqueLimiteDeVitesse)
    {
        if (prio)
            return false;
        else if ((id_dv == ParadeFinDeLimite) || (cartePoseeLimitationVitesse.size() == 0))
        {
            return true;
        }
    }

    bool testAttaque = false;
    if ((id_db == ParadeFeuVert) || (((id_db == ParadeReparations) || (id_db == ParadeRoueDeSecours) || id_db == ParadeEssence) && (regleVert || prio)))
        testAttaque = true;

    if (id == AttaquePanneEssence)
    {
        if (citerne)
            return false;
        else if (testAttaque)
            return true;
        else
            return false;
    }
    if (id == AttaqueCrevaison)
    {
        if (increv)
            return false;
        else if (testAttaque)
            return true;
        else
            return false;
    }
    if (id == AttaqueAccident)
    {
        if (asVolant)
            return false;
        else if (testAttaque)
            return true;
        else
            return false;
    }
    if (id == ParadeFeuVert)
    {
        if (cartePoseeBataille.size() == 0)
            return true;
        else if ((prio) && (id_db != AttaqueFeuRouge))
            return false;
        else if ((((id_db > ParadeFeuVert) && (id_db <= ParadeReparations)) && !regleVert) || (id_db == AttaqueFeuRouge))
            return true;
        else
            return false;
    }
    if (id == ParadeFinDeLimite)
    {
        if (id_dv == AttaqueLimiteDeVitesse)
            return true;
        else
            return false;
    }
    if (id == ParadeEssence)
    {
        if (id_db == AttaquePanneEssence)
            return true;
        else
            return false;
    }
    if (id == ParadeRoueDeSecours)
    {
        if (id_db == AttaqueCrevaison)
            return true;
        else
            return false;
    }
    if (id == ParadeReparations)
    {
        if (id_db == AttaqueAccident)
            return true;
        else
            return false;
    }

    bool testPetiteBorne = false;
    if ((id_db == ParadeFeuVert) || (((id_db == ParadeEssence) || (id_db == ParadeReparations) || (id_db == ParadeRoueDeSecours)) && (regleVert || prio)))
        testPetiteBorne = true;

    if (id == Borne25)
    {
        if (score <= 975)
        {
            if (testPetiteBorne)
                return true;
        }
        else
            return false;
    }

    if (id == Borne50)
    {
        if (score <= 950)
        {
            if (testPetiteBorne)
                return true;
        }
        else
            return false;
    }

    bool testGrandeBorne = false;
    if ((id_db == ParadeFeuVert) && ((cartePoseeLimitationVitesse.size() == 0) || (id_dv == ParadeFinDeLimite)) || (((id_db == ParadeEssence) || (id_db == ParadeReparations) || (id_db == ParadeRoueDeSecours)) && ((regleVert && ((cartePoseeLimitationVitesse.size() == 0) || (id_dv == ParadeFinDeLimite))) || prio)))
        testGrandeBorne = true;

    if (id == Borne75)
    {
        if (score <= 925)
        {
            if (testGrandeBorne)
                return true;
        }
        else
            return false;
    }

    if (id == Borne100)
    {
        if (score <= 900)
        {
            if (testGrandeBorne)
                return true;
        }
        else
            return false;
    }
    if (id == Borne200)
    {
        if (score <= 800)
        {

            if (testGrandeBorne)
                return true;
        }
        else
            return false;
    }

    return false; // On l'atteind jamais mais ca fait plaiz au compilo
}

int Joueur::ajouterCarteMain(const Carte c)
{
    for (int i = 0; i < 7; i++)
    {
        if (carteEnMain[i].getValeur() == Vide)
        {
            carteEnMain[i] = c;
            return 0;
        }
    }
    return -1;
}

int Joueur::supprimerCarteMain(const Carte c)
{
    for (int i = 0; i < 7; i++)
    {
        if (carteEnMain[i] == c)
        {
            carteEnMain[i] = Carte();
            if (carteEnMain[i].getValeur() == Vide)
                return 0;
            else
                return -2; //c'est osé de supposer qu'une égalité ne va pas marcher
        }
    }
    return -2;
}

int Joueur::ajouterCarteAttaque(const Carte c)
{
    if (peutJouerUneCarte(c))
    {
        cartePoseeBataille.push_back(c);
        if (cartePoseeBataille.back() == c)
            return 0;
    }
    return -3;
}

int Joueur::ajouterCarteParade(const Carte c)
{
    if (peutJouerUneCarte(c))
    {
        cartePoseeBataille.push_back(c);
        if (cartePoseeBataille.back() == c)
            return 0;
    }
    return -4;
}

int Joueur::ajouterCarteLimitationVitesse(const Carte c)
{
    if (peutJouerUneCarte(c))
    {
        cartePoseeLimitationVitesse.push_back(c);
        if (cartePoseeLimitationVitesse.back() == c)
            return 0;
    }
    return -5;
}

int Joueur::ajouterCarteFinLimitationVitesse(const Carte c)
{
    if (peutJouerUneCarte(c))
    {
        cartePoseeLimitationVitesse.push_back(c);
        if (cartePoseeLimitationVitesse.back() == c)
            return 0;
    }
    return -6;
}

int Joueur::ajouterCarteBotte(const Carte c)
{
    if (peutJouerUneCarte(c))
    {
        cartePoseeBotte.push_back(c);
        if (cartePoseeBotte.back() == c)
            return 0;
    }
    return -7;
}

int Joueur::ajouterCarteDistance(const Carte c)
{
    if (peutJouerUneCarte(c))
    {
        cartePoseeDistance.push_back(c);
        if (cartePoseeDistance.back() == c)
            return 0;
    }
    return -8;
}

std::string Joueur::getNom() const
{
    return nom;
}

int Joueur::getScore() const
{
    int score = 0;
    for (int i = 0; i < (int)cartePoseeDistance.size(); i++)
    {
        switch (cartePoseeDistance.at(i).getValeur())
        {
        case 15:
            score += 25;
            break;
        case 16:
            score += 50;
            break;
        case 17:
            score += 75;
            break;
        case 18:
            score += 100;
            break;
        case 19:
            score += 200;
            break;
        }
    }

    return score;
}

std::string Joueur::joueurAffichageConsole()
{
    std::string chaine;
    chaine = "Carte bottes : ";
    for (unsigned int i = 0; i < cartePoseeBotte.size(); i++)
    {
        chaine += getCartePoseeBotte().at(i).getNomDisplay();
    }
    Carte lv = getCartePoseeLimitationVitesse();
    Carte pb = getCartePoseeBataille();
    chaine += "\n Carte limitation :";
    chaine += lv.getNomDisplay();
    chaine += "\n Carte bataille : ";
    chaine += pb.getNomDisplay();
    chaine += "\n Score : ";
    chaine += std::to_string(getScore());

    return chaine;
}

std::string Joueur::joueurAffichageMain()
{
    if (bot)
    {
        return "Pas de triche \n";
    }
    else
    {
        std::string chaine = "Cartes Main : ";

        for (unsigned int i = 0; i < 7; i++)
        {
            if (getCarteEnMain()[i].getValeur() != Vide)
            {
                chaine += "n°" + std::to_string(i) + ": ";
                chaine += getCarteEnMain()[i].getNomDisplay();
                chaine += "  ";
            }
        }

        return chaine;
    }
}

void Joueur::testRegression()
{
    //test Joueur
    std::string nomTest = std::to_string(rand() % 100000);
    Joueur test(nomTest, false, false);
    Joueur testBot;
    Joueur testDistance;
    Joueur testCarteMain;
    assert(test.getNom() == nomTest);
    assert(test.estBot() == false);
    assert(testBot.estBot() == true);
    //TEST BOTTE
    Joueur testBotte;
    std::vector<Carte> carteBottes;
    assert(testBotte.ajouterCarteBotte(Carte(BotteAsDuVolant)) == 0);
    carteBottes = testBotte.getCartePoseeBotte();
    assert(std::find(carteBottes.begin(), carteBottes.end(), Carte(BotteAsDuVolant)) != carteBottes.end());
    assert(testBotte.ajouterCarteBotte(Carte(BotteCiterne)) == 0);
    carteBottes = testBotte.getCartePoseeBotte();
    assert(std::find(carteBottes.begin(), carteBottes.end(), Carte(BotteCiterne)) != carteBottes.end());
    assert(testBotte.ajouterCarteBotte(Carte(BotteIncrevable)) == 0);
    carteBottes = testBotte.getCartePoseeBotte();
    assert(std::find(carteBottes.begin(), carteBottes.end(), Carte(BotteIncrevable)) != carteBottes.end());
    assert(testBotte.ajouterCarteBotte(Carte(BottePrioritaire)) == 0);
    carteBottes = testBotte.getCartePoseeBotte();
    assert(std::find(carteBottes.begin(), carteBottes.end(), Carte(BottePrioritaire)) != carteBottes.end());
    //TEST BATAILLE
    Carte carteAttaque;
    //on peut pas en poser
    assert(test.ajouterCarteAttaque(Carte(AttaqueFeuRouge)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueAccident)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueCrevaison)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaquePanneEssence)) != 0);
    assert(test.ajouterCarteAttaque(Carte(ParadeEssence)) != 0);
    assert(test.ajouterCarteAttaque(Carte(ParadeRoueDeSecours)) != 0);
    assert(test.ajouterCarteAttaque(Carte(ParadeReparations)) != 0);
    assert(test.ajouterCarteAttaque(Carte(ParadeFeuVert)) != 0);
    assert(test.ajouterCarteParade(Carte(AttaqueFeuRouge)) != 0);
    assert(test.ajouterCarteParade(Carte(AttaqueAccident)) != 0);
    assert(test.ajouterCarteParade(Carte(AttaqueCrevaison)) != 0);
    assert(test.ajouterCarteParade(Carte(AttaquePanneEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeRoueDeSecours)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeReparations)) != 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte());
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeFeuVert));
    //on peut en poser une
    assert(test.ajouterCarteAttaque(Carte(AttaqueFeuRouge)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaqueFeuRouge));
    assert(test.ajouterCarteAttaque(Carte(AttaqueFeuRouge)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueAccident)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueCrevaison)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaquePanneEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeRoueDeSecours)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeReparations)) != 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaqueFeuRouge));
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeFeuVert));

    assert(test.ajouterCarteAttaque(Carte(AttaqueAccident)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaqueAccident));
    assert(test.ajouterCarteAttaque(Carte(AttaqueFeuRouge)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueAccident)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueCrevaison)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaquePanneEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeRoueDeSecours)) != 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaqueAccident));
    assert(test.ajouterCarteParade(Carte(ParadeReparations)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeReparations));
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeFeuVert));

    assert(test.ajouterCarteAttaque(Carte(AttaquePanneEssence)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaquePanneEssence));
    assert(test.ajouterCarteAttaque(Carte(AttaqueFeuRouge)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueAccident)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueCrevaison)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaquePanneEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeRoueDeSecours)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeReparations)) != 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaquePanneEssence));
    assert(test.ajouterCarteParade(Carte(ParadeEssence)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeEssence));
    assert(test.ajouterCarteAttaque(Carte(ParadeFeuVert)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeFeuVert));

    assert(test.ajouterCarteAttaque(Carte(AttaqueCrevaison)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaqueCrevaison));
    assert(test.ajouterCarteAttaque(Carte(AttaqueFeuRouge)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueAccident)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaqueCrevaison)) != 0);
    assert(test.ajouterCarteAttaque(Carte(AttaquePanneEssence)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeReparations)) != 0);
    assert(test.ajouterCarteParade(Carte(ParadeEssence)) != 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(AttaqueCrevaison));
    assert(test.ajouterCarteParade(Carte(ParadeRoueDeSecours)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeRoueDeSecours));
    assert(test.ajouterCarteParade(Carte(ParadeFeuVert)) == 0);
    carteAttaque = test.getCartePoseeBataille();
    assert(carteAttaque == Carte(ParadeFeuVert));

    carteAttaque = test.getCartePoseeLimitationVitesse();
    assert(carteAttaque == Carte());
    assert(test.ajouterCarteLimitationVitesse(AttaqueLimiteDeVitesse) == 0);
    carteAttaque = test.getCartePoseeLimitationVitesse();
    assert(carteAttaque == Carte(AttaqueLimiteDeVitesse));
    assert(test.ajouterCarteFinLimitationVitesse(Carte(ParadeFinDeLimite)));
    carteAttaque = test.getCartePoseeLimitationVitesse();
    assert(carteAttaque == Carte(ParadeFinDeLimite));

    assert(testDistance.ajouterCarteDistance(Carte(Borne25)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne50)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne75)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne100)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne200)) != 0);
    assert(testDistance.ajouterCarteAttaque(Carte(ParadeFeuVert)) == 0);
    assert(testDistance.getScore() == 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne25)) == 0);
    assert(testDistance.getScore() == 25);
    assert(testDistance.ajouterCarteDistance(Carte(Borne50)) == 0);
    assert(testDistance.getScore() == 75);
    assert(testDistance.ajouterCarteDistance(Carte(Borne75)) == 0);
    assert(testDistance.getScore() == 150);
    assert(testDistance.ajouterCarteDistance(Carte(Borne100)) == 0);
    assert(testDistance.getScore() == 250);
    assert(testDistance.ajouterCarteDistance(Carte(Borne200)) == 0);
    assert(testDistance.getScore() == 450);
    assert(testDistance.ajouterCarteLimitationVitesse(AttaqueLimiteDeVitesse) == 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne75)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne100)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne200)) != 0);
    assert(testDistance.getScore() == 450);
    assert(testDistance.ajouterCarteDistance(Carte(Borne25)) == 0);
    assert(testDistance.getScore() == 475);
    assert(testDistance.ajouterCarteDistance(Carte(Borne50)) == 0);
    assert(testDistance.getScore() == 525);
    assert(testDistance.ajouterCarteFinLimitationVitesse(Carte(ParadeFinDeLimite)));
    assert(testDistance.ajouterCarteDistance(Carte(Borne25)) == 0);
    assert(testDistance.getScore() == 550);
    assert(testDistance.ajouterCarteDistance(Carte(Borne50)) == 0);
    assert(testDistance.getScore() == 600);
    assert(testDistance.ajouterCarteDistance(Carte(Borne75)) == 0);
    assert(testDistance.getScore() == 675);
    assert(testDistance.ajouterCarteDistance(Carte(Borne100)) == 0);
    assert(testDistance.getScore() == 775);
    assert(testDistance.ajouterCarteDistance(Carte(Borne200)) == 0);
    assert(testDistance.getScore() == 975);
    assert(testDistance.ajouterCarteDistance(Carte(Borne50)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne75)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne100)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne200)) != 0);
    assert(testDistance.getScore() == 975);
    assert(testDistance.ajouterCarteDistance(Carte(Borne25)) == 0);
    assert(testDistance.getScore() == 1000);
    assert(testDistance.ajouterCarteDistance(Carte(Borne25)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne50)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne75)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne100)) != 0);
    assert(testDistance.ajouterCarteDistance(Carte(Borne200)) != 0);
    assert(testDistance.getScore() == 1000);
    //TODO tester getCartePoseeDistance
    std::array<Carte, 7> carteMain = testCarteMain.getCarteEnMain();
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BottePrioritaire)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BotteCiterne)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BotteIncrevable)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BotteAsDuVolant)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaqueFeuRouge)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaqueLimiteDeVitesse)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaquePanneEssence)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaqueCrevaison)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaqueAccident)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeFeuVert)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeFinDeLimite)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeEssence)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeRoueDeSecours)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeReparations)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne25)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne50)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne75)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne100)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne200)) == carteMain.end());
    assert(testCarteMain.ajouterCarteMain(Carte(BottePrioritaire)) == 0);
    assert(testCarteMain.ajouterCarteMain(Carte(BotteAsDuVolant)) == 0);
    assert(testCarteMain.ajouterCarteMain(Carte(ParadeFinDeLimite)) == 0);
    assert(testCarteMain.ajouterCarteMain(Carte(Borne200)) == 0);
    assert(testCarteMain.ajouterCarteMain(Carte(Borne75)) == 0);
    assert(testCarteMain.ajouterCarteMain(Carte(Borne25)) == 0);
    assert(testCarteMain.ajouterCarteMain(Carte(ParadeFeuVert)) == 0);

    assert(testCarteMain.ajouterCarteMain(Carte(AttaqueCrevaison)) != 0);
    carteMain = testCarteMain.getCarteEnMain();
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BottePrioritaire)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BotteAsDuVolant)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeFinDeLimite)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne200)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne75)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne25)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeFeuVert)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaqueCrevaison)) == carteMain.end());

    assert(testCarteMain.supprimerCarteMain(Carte(Borne75)) == 0);
    assert(testCarteMain.supprimerCarteMain(Carte(Borne25)) == 0);
    assert(testCarteMain.supprimerCarteMain(Carte(ParadeFeuVert)) == 0);
    assert(testCarteMain.supprimerCarteMain(Carte(AttaqueCrevaison)) != 0);
    carteMain = testCarteMain.getCarteEnMain();
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BottePrioritaire)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(BotteAsDuVolant)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeFinDeLimite)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne200)) != carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne75)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(Borne25)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(ParadeFeuVert)) == carteMain.end());
    assert(std::find(carteMain.begin(), carteMain.end(), Carte(AttaqueCrevaison)) == carteMain.end());
}
