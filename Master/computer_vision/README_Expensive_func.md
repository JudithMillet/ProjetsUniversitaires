# Utilisation de cython pour accélérer le calcul.

Pour le calcul des clusters, on utilise un fichier (expensive_func.pyx) qui utilise le langage cython.
Les fonctions qui était utilisé pour le calcul de clusters sont maintenant dans ce fichier.
Le fichier "expensive_func.pyx" est accompagné de "setup.py". Ce dernier permet la compilation du code.

Pour faire fonctionner les modifications apporté dans "expensive_func.pyx" il faut lancer dans un terminal à l'emplacement du projet la commande :

py setup.py build_ext --inplace

Les autres fichiers qui ont été ajouté sont issus de la compilation d'ailleurs ils ne sont pas pareil si on travail sur linux ou sur windows.

Pour travailler avec les fonctions ajouté dans "expensive_func.pyx" il faut faire un import python classique. Ex :

import expensive_func as exp
