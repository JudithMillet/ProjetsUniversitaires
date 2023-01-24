# Projet d'Orientation en Master - Master 1 informatique

<br/>

## Auteur

-  Judith Millet - 11811933

<br/>

## Contexte

Ce projet a pour but de développer une méthode automatique de reconstruction d'un bâtiment depuis un nuage de points. Il a été encadré par M. DAMIAND, chercheur au laboratoire LIRIS.

<br/>

## Prérequis

- Librairie CGAL (à cloner depuis ***https://github.com/CGAL/cgal.git***),
- cmake,
- make,
- Qt5.
<br/><br/>

## How to

Aller dans le répertoire du programme à compiler puis :
```
mkdir build
cd build
cmake -DCGAL_DIR=/path/to/cgal -DCMAKE_BUILD_TYPE=Release ..
./prog_name [-options] /data/file 
```
<br/>

## Organisation


### Dossier **progs**

- **reconstruction.cpp**, contient la méthode de reconstruction qui détecte les plans, les insère, puis supprime les plans selon un seuil. <br/><br/>
Possibilité d'appliquer les paramètres :
    - epsilon E : nombre de points valides pour une surface (170 par défaut).
    - optimal_epsilon : booléen pour calculer le seuil en fonction de l'aire (faux  par défaut).
    - normal N : seuil de la normale pour l'adjacence entre 2 plans (lors du prétraitement), (50  par défaut).
    - distance D : seuil pour la distance pour l'adjacence entre 2 plans (lors du prétraitement), (0.6  par défaut).
    - ransac_param : booléen pour l'application des paramètres RANSAC (faux  par défaut).
    - preprocess : booléen pour l'application des prétraitements (faux  par défaut). 
    - less : booléen pour prendre seulement 1 point sur 3 lors du comptage des points par face (faux  par défaut).
    <br/><br/>


- **point_cloud_proc.cpp**, contient la méthode de reconstruction en insérant les plans 5 par 5 dans la LCC (ne fonctionne pas).
- **shape_detection.cpp**, méthode de détection de primitives géométriques RANSAC de CGAL,
<br/><br/>

### Dossier **tools**

- **Linear_cell_complex_cut_operation.h**, contient les méthodes utiles à la LCC.

- **lcc_creations.h**, contient la méthode "make_hexa()" utilisée dans **Linear_cell_complex_cut_operation.h**.

- **lcc_triangulate_faces.h**, contient la méthode pour trianguler la LCC.

- **typedefs.h**, contient les types utilisés.

