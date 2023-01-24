/**
* @file txtJeu.h
* @brief Programme txtJeu
* @author FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine 
* @date 2020/03/24
*/
#ifndef _TXTJEU_H
#define _TXTJEU_H

#include <string>

#include "core/Jeu.h"

/**
* @brief Permet de renvoyer la valeur (entier) au lieu de la valeur ascii
* @param nbrChiffre : un entier
* @return un entier
* */
int askint(int nbrChiffre);

/**
* @brief Permet de renvoyer une chaine de caractères
* @param nbrCar : un entier
* @return une chaine de caractères
* */
std::string askstr(int nbrCar);

/**
* @brief Initialise le jeu dans le terminal
* @param Jeu : une référence sur Jeu
* */
void txtInit(Jeu *jeu);

/**
* @brief Permet le déroulement du jeu et l'affiche
* @param Jeu : une référence sur Jeu
* */
void txtBoucle(Jeu *jeu);

/**
* @brief Affiche les cartes en main du Joueur et les cartes posées (bottes,batailles,score) de tous les Joueurs
* @param Jeu : une référence sur Jeu
* */
void txtAffichageJeu(Jeu *p);

#endif
