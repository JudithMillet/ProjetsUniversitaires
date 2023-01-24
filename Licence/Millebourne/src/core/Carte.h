/**
* @file Carte.h
* @brief Classe Carte
* @author FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine 
* @date 2020/03/11
*/
#ifndef LIFAP4_CARTE_H
#define LIFAP4_CARTE_H

#include <string>
#include <cassert>
#include <iostream>

enum Card
{
	Vide,
	BottePrioritaire,		//1 carte
	BotteCiterne,			//1 carte
	BotteIncrevable,		//1 carte
	BotteAsDuVolant,		//1 carte
	AttaqueFeuRouge,		//5 cartes
	AttaqueLimiteDeVitesse, //4 cartes
	AttaquePanneEssence,	//3 cartes
	AttaqueCrevaison,		//3 cartes
	AttaqueAccident,		//3 cartes
	ParadeFeuVert,			//14 cartes
	ParadeFinDeLimite,		//6 cartes
	ParadeEssence,			//6 cartes
	ParadeRoueDeSecours,	//6 cartes
	ParadeReparations,		//6 cartes
	Borne25,				//10 cartes
	Borne50,				//10 cartes
	Borne75,				//10 cartes
	Borne100,				//12 cartes
	Borne200				// 4 cartes
};

/**
* @class Carte
* @brief Classe Carte
* */
class Carte
{
public:
	/**
	    * @brief constructeur par défault de Carte.
	    * @param
	    * @return
	    * */
	Carte();
	/**
	    * @brief constructeur Carte (initialise les cartes selon l'id).
	    * @param id : un entier
	    * @return
	    * */
	Carte(int id);
	/**
	    * @brief destructeur Carte par defaut (affiche un message lordque la carte est détruite).
	     * @return
	     * */
	~Carte();
	/**
	    * @brief Accesseur : récupère la valeur de la carte.
	     * @return un entier
	     * */
	int getValeur() const;
	/**
	    * @brief Accesseur : récupère le nom de la carte.
	     * @return une chaine de caractère
	     * */
	std::string getNom() const;
	/**
	    * @brief Accesseur : récupère le nom de display de la carte.
	     * @return une chaine de caractère
	     * */
	std::string getNomDisplay() const;

	bool operator==(const Carte &a);

	bool operator!=(const Carte &a);

	/**
    * @brief Effectue les tests pour vérifier si tout fonctionne
    */
    void testRegression();

private:
	/**
	    * @brief valeur : un entier compris entre [1,19].
        * Chaque valeur correspond à un type de carte différent.
	     * */
	enum Card valeur;
	/**
	    * @brief nom : une chaine de caractère affichable de la carte.
	     * */
	std::string nom;
	/**
        * @brief nomDisplay : une chaine de caractère pour l'affichage affichable de la carte.
         * */
	std::string nomDisplay;
};

#endif