# M2 - Monde Virtuel - TP

## Auteurs
* Mehdi Guittard
* Judith Millet

### Prérequis pour linux
Il vous faut une version de **premake**, **gmake**.

Si vous êtes sous Windows, suivez les étapes sur **https://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/M1IMAGE/html/group__installation.html**

### Compiler le projet sous linux

Il suffit de lancer les commandes suivantes
```
$ premake5 gmake
$ make M2_mmv
$ ./bin/M2_mmv [OPTIONS]
```
<br/>
Les options sont :

- ```-data [PATH]``` pour aller chercher un terrain en particulier,
- ```-smooth S``` pour lisser le terrain S fois,
- ```-erosion E``` pour eroder le terrain pendant E itérations,
- ```-road R``` pour générer une route sur le terrain selon la pente (R=1), selon l'environnement (R=2) ou les deux combinés (R<=3),
- ```[--help|-h|-?]``` pour obtenir le récapitulatif de ces options.