//Carte.cpp Classe Carte
//Auteurs : FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine
//Date : 11/03/2020

#include "Carte.h"

Carte::Carte()
{
    valeur = Vide;
    nom = "";
    nomDisplay = "X";
}
//Constructeur d'une carte selon l'id
Carte::Carte(int id)
{
    switch (id)
    {
    case Vide:
        valeur = Vide;
        nom = "";
        nomDisplay = "X";
        break;
    case BottePrioritaire:
        valeur = BottePrioritaire;
        nom = "Carte botte Prioritaire";
        nomDisplay = "Prio";
        break;
    case BotteCiterne:
        valeur = BotteCiterne;
        nom = "Carte botte Citerne";
        nomDisplay = "Citerne";
        break;
    case BotteIncrevable:
        valeur = BotteIncrevable;
        nom = "Carte botte Increvable";
        nomDisplay = "Increv";
        break;
    case BotteAsDuVolant:
        valeur = BotteAsDuVolant;
        nom = "Carte botte As du volant";
        nomDisplay = "AsVolant";
        break;
    case AttaqueFeuRouge:
        valeur = AttaqueFeuRouge;
        nom = "Carte botte Feu rouge";
        nomDisplay = "Rouge";
        break;
    case AttaqueLimiteDeVitesse:
        valeur = AttaqueLimiteDeVitesse;
        nom = "Carte attaque Limite de vitesse";
        nomDisplay = "Limit";
        break;
    case AttaquePanneEssence:
        valeur = AttaquePanneEssence;
        nom = "Carte attaque Panne d'essence";
        nomDisplay = "Panne";
        break;
    case AttaqueCrevaison:
        valeur = AttaqueCrevaison;
        nom = "Carte attaque Crevaison";
        nomDisplay = "Crev";
        break;
    case AttaqueAccident:
        valeur = AttaqueAccident;
        nom = "Carte attaque Accident";
        nomDisplay = "Accident";
        break;
    case ParadeFeuVert:
        valeur = ParadeFeuVert;
        nom = "Carte parade Feu vert";
        nomDisplay = "Vert";
        break;
    case ParadeFinDeLimite:
        valeur = ParadeFinDeLimite;
        nom = "Carte parade Fin de limiteFin";
        nomDisplay = "Fin";
        break;
    case ParadeEssence:
        valeur = ParadeEssence;
        nom = "Carte parade Essence";
        nomDisplay = "Essence";
        break;
    case ParadeRoueDeSecours:
        valeur = ParadeRoueDeSecours;
        nom = "Carte parade Roue de secours";
        nomDisplay = "Secours";
        break;
    case ParadeReparations:
        valeur = ParadeReparations;
        nom = "Carte parade Repartitions";
        nomDisplay = "Reparation";
        break;
    case Borne25:
        valeur = Borne25;
        nom = "Carte borne 25";
        nomDisplay = "25";
        break;
    case Borne50:
        valeur = Borne50;
        nom = "Carte borne 50";
        nomDisplay = "50";
        break;
    case Borne75:
        valeur = Borne75;
        nom = "Carte borne 75";
        nomDisplay = "75";
        break;
    case Borne100:
        valeur = Borne100;
        nom = "Carte borne 100";
        nomDisplay = "100";
        break;
    case Borne200:
        valeur = Borne200;
        nom = "Carte borne 200";
        nomDisplay = "200";
        break;
    }
}

//Destructeur
Carte::~Carte()
{
    // std::cout << "Carte détruite." << std::endl;
}

//Renvoie la valeur de la carte
int Carte::getValeur() const
{
    //assert((valeur >= 1 && valeur <= 19) || valeur == -1);
    return valeur;
}

//Renvoie le nom de la carte
std::string Carte::getNom() const
{
    //assert();
    return nom;
}

std::string Carte::getNomDisplay() const
{
    //assert();
    return nomDisplay;
}

bool Carte::operator==(const Carte &a)
{
    return valeur == a.valeur;
}

bool Carte::operator!=(const Carte &a)
{
    return valeur != a.getValeur();
}

void Carte::testRegression()
{
    Card values[19] = {BottePrioritaire,BotteCiterne,BotteIncrevable,BotteAsDuVolant,AttaqueFeuRouge,AttaqueLimiteDeVitesse,AttaquePanneEssence,AttaqueCrevaison,AttaqueAccident,ParadeFeuVert,ParadeFinDeLimite,ParadeEssence,ParadeRoueDeSecours,ParadeReparations,Borne25,Borne50,Borne75,Borne100,Borne200};
    Carte iterator;
    for( int i = 0; i < 3; i++ )
    {
    iterator = Carte(values[i]);
    //do something with it
    assert(iterator == iterator);
    assert(iterator != Carte());
    }
}