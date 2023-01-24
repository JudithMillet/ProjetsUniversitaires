/**
* @file sdlJeu.h
* @brief Programme sdlJeu
* @author FORISSIER Benjamin, LAFOURCADE Emie, MILLET Judith, RYBACKI Antoine 
* @date 2020/03/24
*/
#ifndef LIFAP4_SDLJEU_H
#define LIFAP4_SDLJEU_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "./core/Jeu.h"

/**
* @brief Définie une position selon les paramètres
* @param pos : une référence à une position sous SDL2
* @param X,Y,W,H : des entiers pour définir une position
* @return
* */
void position(SDL_Rect &pos, int X, int Y, int W, int H);

/**
* @brief Renvoie le nom que l'utilisateur a choisie
* @param renderer : une référence au renderer
* @param fond : une référence à la texture du fond
* @param police : une référence à la police d'écriture utilisé
* @param couleurNoire : une référence à la couleur utilisé
* @return un string qui correspond au nom que l'utilisateur à choisis
* */
std::string presentation(SDL_Renderer *renderer, SDL_Texture *fond, TTF_Font *police, SDL_Color couleurNoire);

/**
* @brief Renvoie un entier que l'utilisateur a choisis pour avoir le nombre de joueur
* @param renderer : une référence au renderer
* @param fond : une référence à la texture du fond
* @param police : une référence à la police d'écriture utilisé
* @param couleurNoire : une référence à la couleur utilisé
* @return un entier qui correspond au nombre que l'utilisateur à choisis
* */
int combienJoueur(SDL_Renderer *renderer, SDL_Texture *fond, TTF_Font *police, SDL_Color couleurNoire);

/**
* @brief Renvoie un entier que l'utilisateur a choisis pour avoir le nombre de joueur
* @param renderer : une référence au renderer
* @param fond : une référence à la texture du fond
* @param tab[] : un référence au tableau de règles que l'on va modifier
* @param police : une référence à la police d'écriture utilisé
* @param couleurNoire : une référence à la couleur utilisé
* @return 
* */
void lesRegles(SDL_Renderer *renderer, SDL_Texture *fond, int tab[], TTF_Font *police, SDL_Color couleurNoire);

/**
* @brief Modifie le paquet de carte pour permettre à l'utilisateur de le personnaliser
* @param renderer : une référence sur le renderer
* @param fond : une référence à la texture du fond
* @param lesCartes[] : une référence au tableau de carte que l'on va utiliser
* @param police : une référence à la police d'écriture utilisé
* @param couleur : Une référence à la couleur utilisé
* @return 
* */
void sdlInit(SDL_Renderer *renderer, SDL_Texture *fond, int lesCartes[], TTF_Font *police, SDL_Color couleur);

/**
* @brief La boucle de Jeu qui permet le déroulement d'une partie
* @param renderer : une référence sur le renderer
* @param fond : une référence à la texture du fond
* @param jeu : une référence à la partie de jeu que l'on va dérouler
* @param police : une référence à la police d'écriture utilisé
* @param couleur : Une référence à la couleur utilisé
* @return 
* */
void sdlBoucle(SDL_Renderer *renderer, SDL_Texture *fond,Jeu *jeu, TTF_Font *police, SDL_Color couleurNoire);

/**
* @brief Une fonction qui permet l'affichage des cartes
* @param renderer : une référence sur le renderer
* @param fond : une référence à la texture du fond
* @param jeu : une référence à la partie de jeu que l'on va dérouler
* @param police : une référence à la police d'écriture utilisé
* @param couleur : Une référence à la couleur utilisé
* @return 
* */
void sdlAffichage(SDL_Renderer *renderer, SDL_Texture *fond, Jeu *jeu, int emplacement, TTF_Font *police, SDL_Color couleurNoire);

/**
* @brief Une fonction qui permet l'affichage du classement de fin
* @param renderer : une référence sur le renderer
* @param fond : une référence à la texture du fond
* @param jeu : une référence à la partie de jeu que l'on va dérouler
* @param police : une référence à la police d'écriture utilisé
* @param couleur : Une référence à la couleur utilisé
* @param b : un booleen indiquant si le joueur à quitté la partie avant la fin
* @return 
* */
void sdlAffichageFin(SDL_Renderer *renderer,SDL_Texture *fond, Jeu *jeu, TTF_Font *police, SDL_Color couleurNoire, bool b);

#endif
