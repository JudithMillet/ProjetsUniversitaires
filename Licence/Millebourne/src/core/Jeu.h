/**
* @file Jeu.h
* @brief Classe Jeu
* @author FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine 
* @date 2020/03/11
*/
#ifndef LIFAP4_JEU_H
#define LIFAP4_JEU_H

#include <vector>
#include <algorithm>

#include "PiocheDefausse.h"
#include "Joueur.h"
#include "Carte.h"

#define NBREGLES 3

struct dataJoueur
{
	int id;
	int score;

	bool operator<(const dataJoueur &a) const
	{
		return (score < a.score);
	}
};

/**
* @class Jeu
* @brief Classe Jeu
* @param aucun
* */
class Jeu
{

public:
	/**
	* @brief constructeur Jeu (initialise le Jeu).
	* @param nbrJoueur : un entier correspondant au nombre de joueurs dans la partie.
	* @param nom : nom du joueur.
	* @param tab : tableau de règles.
	* @return
	* */
	Jeu(int nbrJoueurs, std::string nom, int tab[]);
	/**
	* @brief destructeur Jeu par defaut.
	* @param aucun
	* @return
	* */
	~Jeu();
	/**
	* @brief fonction poserUneCarte.
    * Permet de poser une Carte de n'importe quel type, de supprimer cette Carte du jeu du Joueur et de passer au prochain Joueur.
	* @param p : un entier correspondant à la position de la carte jouée
	* @param x : un entier correspondant au joueur qui joue, 
	* @param y : un entier correspondant au joueur à qui l'attaque est lancée, b : un booléen.
	* @return un entier
	* */
	int poserUneCarte(int p, int x, int y);
	/**
	* @brief fonction piocherUneCarte.
    * Permet de piocher une Carte et de l'ajouter dans le jeu du Joueur.
	* @param x : un entier correspondant au joueur qui joue, 
	* @param b : un booléen.
	* @return un entier
	* */
	int piocherUneCarte(int x, bool b);
	/**
	* @brief fonction sauvegarder.
	* @param nom : une chaine de caractères.
	* @return un entier
	* */
	int sauvegarder(std::string nom);
	/**
	* @brief fonction charger.
	* @param nom : une chaine de caractères.
	* @return un entier
	* */
	int charger(std::string nom);
	/**
	* @brief fonction ListerLesSauvegardes.
	* @param aucun
	* @return un tableau dynamique de chaines de caractères.
	* */
	std::vector<std::string> ListerLesSauvegardes();
	/**
	* @brief fonction initialiserLaPartie.
	* @param tab : un tableau d'entiers.
	* @return un entier
	* */
	int initialiserLaPartie(int tab[]);
	/**
	* @brief fonction gagnant.
    * Renvoie si quelqu'un a mille ou si la pioche est vide, celui (ou ceux) qui a (ont) le plus grand score.
	* @param fin : est à true quand l'utilisateur quitte la partie avant la fin
	* @return un tableau dynamique d'entiers.
	* */
	std::vector<int> gagnant(bool fin);
	/**
	* @brief Range les données des joueurs (leur id et leur score) de celui qui à le meilleur score à celui qui a le plus petit
	* @param aucun
	* @return un tableau dynamique de dataJoueur dans l'ordre décroissant des scores.
	* */
	std::vector<dataJoueur> getScoreJoueurs() const;
	/**
	* @brief accesseur tableau dynamique de Joueur
	* @param aucun
	* @return un tableau dynamique de Joueur
	* */
	std::vector<Joueur> getJoueurs();
	/**
	 * @brief acceseur aQuiLeTour
	 * Renvoie l'id du joueur qui doit jouer
	 * @return un entier
	 * */
	int getAQuiLeTour() const;
	/**
	 * @brief acceseur nbrJoueurs
	 * Renvoie le nombre de joueurs
	 * @return un entier
	 * */
	int getnbrJoueurs() const;
	/**
	 * @brief acceseur PiocheDefausse PD
	 * Renvoie la PiocheDefausse
	 * @return PiocheDefausse
	 * */
	PiocheDefausse getPD() const;

	std::array<int, 3> getTableauRegle() const;
	/**
	* @brief fonction testRegression.
	* @param aucun.
	* */
	void testRegression();
	/**
	* @brief permet l'affichage du classement à la fin d'une partie
	* @param fin : booleen à true si l'utilisateur quitte la partie avant la fin
	* @return une chaine de charactère, le classement des joueurs
	* */
	std::string jeuAffichageClassement(bool fin);
	/**
	* @brief fonction jouerBot
	* Fait jouer le bot et renvoie un entier correspondant au type de la carte qu'il a joué
	* @param numeroBot : un entier correspondant au numero du bot
	* @return un entier
	* @retval -54 : y n'est pas valide
	* @retval -53 : x n'est pas valide
	* @retval -52 : x et y égaux dans poserUneCarte donc cartes ATTAQUE ne peuvent pas être posées
	* @retval -51 : x et y différents dans poserUneCarte donc cartes BOTTE ou cartes PARADE ou carte BORNE ne peuvent pas être posées
	*
	* @retval -8 : la carte DISTANCE n'a pas pu être posée
	* @retval -7 : la carte BOTTE n'a pas pu être posée
	* @retval -6 : la carte de fin de limitation n'a pas pu être posée
	* @retval -5 : la carte de limitation de vitesse n'a pas pu être posée
	* @retval -4 : la carte PARADE n'a pas pu être posée
	* @retval -3 : la carte ATTAQUE n'a pas pu être posée
	* @retval -2 : la carte n'a pas pu être supprimée de la main
	* @retval -1 : le carte n'a pas pu être ajoutée à la main
	*
	* @retval 2 : la pioche est vide
	*
	* @retval -100 : le joueur n'est pas un bot
    * @retval 1 : carte BOTTE posée
    * @retval 5 : carte ATTAQUE posée
    * @retval 10 : carte CARTE_PARADE_FEU_VERT posée
    * @retval 11 : carte CARTE_PARADE_FIN_DE_LIMITE posée
    * @retval 12 : carte CARTE_PARADE_ESSENCE posée
    * @retval 13 : carte CARTE_PARADE_ROUE_DE_SECOURS posée
	* @retval 14 : carte CARTE_PARADE_REPARATIONS posée
    * @retval 15 : carte CARTE_BORNE_25 posée
    * @retval 16 : carte CARTE_BORNE_50 posée
    * @retval 17 : carte CARTE_BORNE_75 posée
    * @retval 18 : carte CARTE_BORNE_100 posée
    * @retval 19 : carte CARTE_BORNE_200 posée
	* */
	int jouerBot(int numeroBot);

private:
	/**
	* @brief joueurs : un tableau dynamique de Joueur.
	* */
	std::vector<Joueur> joueurs;
	/**
	* @brief aQuiLeTour : un entier.
	* */
	int aQuiLeTour;
	/**
	* @brief regles : un tableau d'entier de taille NBREGLES.
	* */
	std::array<int, 3> regles;
	/**
	* @brief nbrJoueurs : un entier.
	* */
	int nbrJoueurs;
	/**
	* @brief PD : PiocheDefausse.
	* */
	PiocheDefausse PD;
};

#endif