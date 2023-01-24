# TP d'Animation de personnage (partie 2) - MIF37

## Auteur

- Judith Millet 11811933

## How to

Impossible de lancer le projet, images du résultat dans le dossier **results/**.

## Réalisations

### Partie 1 : affichage

Création d'un module Skeleton.cpp/.h afin de modéliser un squelette composé de sphères et de cylindres. Animation en fonction des frames.

## Partie 2 : controleur d'animation

Création d'un controleur CharacterController qui permet d'animer une sphère dans un premier temps avec les touches :
- 'z' pour avancer,
- 's' pour reculer,
- 'q' pour aller à gauche,
- 'd' pour aller à droite.

Pour voir ce comportement, il faut décommenter ***Viewer v;*** dans main.cpp et commenter ***CharAnimViewer v;***.

Ensuite, ce controleur permet de controler un personnage en changeant son comportement :
- 'z' une fois pour marcher,
- 'z' une deuxième fois pour courir,
- 's' une fois pour marcher ou pour s'arrêter,
- 's' une deuxième fois pour s'arrêter,
- 'q' pour aller à gauche,
- 'd' pour aller à droite,
- 'x' pour frapper.

Ce personnage peut se déplacer dans la scène de la même manière que la sphère mais en changeant son comportement.

## Partie 3 : Transition et plus

Au niveau de l'interpolation entre les animations, l'objectif ici est de calculer la distance entre la dernière frame de l'animation précédente et chaque frame de la nouvelle animation afin de décider de la plus "proche". Ainsi, l'animation suivante part de la frame choisie. 
