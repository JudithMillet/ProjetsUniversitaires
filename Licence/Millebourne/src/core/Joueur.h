/**
* @file Joueur.h
* @brief Classe Joueur
* @author FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine 
* @date 2020/03/21
*/
#ifndef LIFAP4_JOUEUR_H
#define LIFAP4_JOUEUR_H

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#include "Carte.h"

/**
* @class Joueur
* @brief Classe Joueur
*/
class Joueur
{
public:
    /**
    * @brief Constructeur par défaut
    * @return
    */
    Joueur();
    /**
    * @brief Constructeur de joueur (selon le nom donné, et s'il est un bot).
    * @param leNom : une chaine de caractère, BOOL : un booléen
    * @return
    */
    Joueur(std::string leNom, bool BOOl, bool vert);
    /**
    * @brief Destructeur de la classe Joueur
    * @return
    */
    ~Joueur();
    /**
    * @brief Retourne un booléen désignant si le joueur est un bot
    * @return un booléen : si le joueur est un bot
    */
    bool estBot();
    /**
    * @brief Retourne le nombre de cartes en mains
    * @return un entier : le nombre de carte que le joueur a en main
    */
    int getNbrCarteEnMain() const;
    /**
    * @brief Retourne les cartes en mains
    * @return un tableau de carte : Celle que le joueur a en main
    */
    std::array<Carte, 7> getCarteEnMain() const;
    /**
    * @brief récupère la dernière carte Botte posée par le joueur
    * @return Carte : la dernière carte Botte du joueur
    */
    std::vector<Carte> getCartePoseeBotte(); // renvoie les cartes bottes posé
    /**
    * @brief récupère la dernière carte Vitesse posée par le joueur
    * @return Carte : la dernière carte Vitesse du joueur
    */
    Carte getCartePoseeLimitationVitesse() const;
    /**
    * @brief récupère le nom
    * @return String : le nom du joueur
    */
    Carte getCartePoseeBataille() const;
    /**
    * @brief récupère la dernière carte Distance posée par le joueur
    * @return Carte : la dernière carte Distance du joueur
    */
    std::vector<Carte> getCartePoseeDistance();
    /**
    * @brief Ajoute une carte dans la main du joueur
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteMain(const Carte c);
    /**
    * @brief Supprime une carte de la main du joueur
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int supprimerCarteMain(const Carte c);
    /**
    * @brief Ajoute une carte Attaque reçu par un autre Joueur dans la pile Bataille
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteAttaque(const Carte c);
    /**
    * @brief Ajoute une carte Parade dans la pile Bataille
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteParade(const Carte c);
    /**
    * @brief Ajoute une carte "Limitation de Vitesse" reçu par un autre Joueur dans la pile Vitesse
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteLimitationVitesse(const Carte c);
    /**
    * @brief Ajoute une carte "Fin de limitation de Vitesse" dans la pile Vitesse
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteFinLimitationVitesse(const Carte c);
    /**
    * @brief Ajoute une carte Botte dans la pile Botte
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteBotte(Carte c);
    /**
    * @brief Ajoute une carte Distance dans la pile Distance
    * @param c : une Carte passé en constante
    * @return renvoie un entier indiquant si la fonction a réussie ou échoué
    * */
    int ajouterCarteDistance(Carte c);
    /**
    * @brief Donne le nom du Joueur
    * @return renvoie une chaine de caractères
    */
    std::string getNom() const;
    /**
    * @brief Donne  le score : la somme de toutes les cartes distances
    * @return renvoie le score
    */
    int getScore() const;
    /**
    * @brief Crée une chaine de caractère détaillant les cartes posées devant le joueur
    * @return Chaine de caratère
    */
    std::string joueurAffichageConsole();
    /**
    * @brief Crée une chaine de caractère détaillant les cartes en main dujoueur
    * @return Chaine de caratère
    */
    std::string joueurAffichageMain();
    /**
    * @brief Effectue les tests pour vérifier si tout fonctionne
    */
    void testRegression(); 

private:
    /**
	* @brief nom : une chaine de caractère désignant le nom
	*/
    std::string nom;
    /**
	* @brief carteEnMain : tableaux fixe de 7 cartes que le joueur a en main
	*/
    std::array<Carte, 7> carteEnMain;
    /**
    * @brief carteEnMain : Tableaux dynamique de carte Botte posées
    */
    std::vector<Carte> cartePoseeBotte;
    /**
	* @brief carteEnMain : Tableaux dynamique de carte Vitesse posées
	*/
    std::vector<Carte> cartePoseeLimitationVitesse;
    /**
	* @brief carteEnMain : Tableaux dynamique de carte Bataille posées
	*/
    std::vector<Carte> cartePoseeBataille;
    /**
	* @brief carteEnMain : Tableaux dynamique de carte Distance posées
	*/
    std::vector<Carte> cartePoseeDistance;
    /**
	* @brief estBot : Un booléen désignant si le joueur est un bot
	*/
    bool bot;
    /**
	* @brief regleVert : Un booléen désignant si il y a besoin de poser un feu vert après une carte parade
	*/
    bool regleVert;
        /**
    * @brief renvoie un booléen indiquant si la carte passé en paramètre peut etre joué
    * @param c : Une carte passé en constante qu'on testera
    * @return bool : un booléen indiquant si la carte peut être joué
    * */
    bool peutJouerUneCarte(const Carte c);
};

#endif
