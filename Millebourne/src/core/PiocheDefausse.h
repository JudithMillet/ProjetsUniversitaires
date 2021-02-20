/**
* @file PiocheDefausse.h
* @brief Classe PiocheDefausse
* @author FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine 
* @date 2020/03/17
*/
#ifndef LIFAP4_PIOCHEDEFAUSSE_H
#define LIFAP4_PIOCHEDEFAUSSE_H

#include <vector>
#include <stack>
#include "Carte.h"

///@enum NBTYPE
#define NBTYPE 19

///@class PiocheDefausse
class PiocheDefausse
{
public:
    ///@brief Constructeur par défaut de la classe PiocheDefausse
    PiocheDefausse();

    /**
    * @brief Accesseur Defausse
    * @return un stack de Carte
    * */
    std::stack<Carte> getDefausse();

      /**
    * @brief Accesseur Pioche
    * @return un stack de Carte
    * */
    std::stack<Carte> getPioche();

    /**
    * @brief Donne la taille de la défausse
    * @return La taille de la défausse en entier
    * */
    int tailleDefausse() const;

    /**
    * @brief Donne la taille de la pioche
    * @return La taille de la pioche en entier
    */
    int taillePioche() const;

    /**
    * @brief Donne le nombre de cartes bornes restantes
    * @return Le nombre de cartes bornes encore dans le jeu
    */
    int cartesDistanceRestantes() const;

    ///@brief Rempli + Mélange la pioche avec les cartes du tableau tab contenant les 19 types de cartes.
    void remplirPioche(int nbCartesParType[]);

    /** 
    *@brief Transforme la défausse en pioche sans la mélanger
    *@return Retourne un entier : 1 si la défausse est vide, 2 si la pioche n'est pas vide, 0 si la défausse est bien vidée
    */
    int transfereDefausse();

    /**
    *@brief Mets une carte dans la défausse
    *@param c La carte à défausser
    *@return Retourne un entier : 0 si la défausse de la carte s'est bien passée, 1 sinon
    */
    int defausserUneCarte(Carte c);

    /**
    * @brief Prends une carte dans la pioche ou la défausse selon le choix du joueur
    *@param c La carte qui va étre tirée par le joueur @param b bouléen choisissant entre la pioche (false) ou la défausse (true)
    *@return Un entier : 0 si ça s'est bien passé, 1 si la défausse est vide, 2 si la pioche est vide
    */
    int piocher(Carte &c, bool b);

    void testRegression();

    ///@brief destructeur de la classe PiocheDefausse
    ~PiocheDefausse();

private:
    ///@brief Tableau dynamique de cartes qui constitue la pioche
    std::stack<Carte> pioche;
    ///@brief Tableau dynamique de cartes qui constitue la défausse
    std::stack<Carte> defausse;
    ///@brief Tableau statique indiquant le nombre de carte de chaque type disponible dans la pioche
    int nbCarteParTypePioche[NBTYPE];
    ///@brief Tableau statique indiquant le nombre de carte de chaque type disponible dans la défausse
    int nbCarteParTypeDefausse[NBTYPE];

    /**
    * @brief Mélange l'ordre des cartes du paquet en paramètre
    *@param c Paquet (vector) de cartes à mélanger
    */
    void melangerVector(std::vector<Carte> &c);
};

#endif