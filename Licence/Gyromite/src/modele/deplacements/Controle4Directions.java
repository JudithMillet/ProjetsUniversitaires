package modele.deplacements;

import modele.plateau.Entite;
import modele.plateau.EntiteDynamique;

/**
 * Controle4Directions permet d'appliquer une direction (connexion avec le clavier) à un ensemble d'entités dynamiques
 */
public class Controle4Directions extends RealisateurDeDeplacement {
    private Direction directionCourante;
    // Design pattern singleton
    private static Controle4Directions c4d;

    public static Controle4Directions getInstance() {
        if (c4d == null) {
            c4d = new Controle4Directions();
        }
        return c4d;
    }

    public void setDirectionCourante(Direction _directionCourante) {
        directionCourante = _directionCourante;
    }

    public boolean realiserDeplacement() {
        boolean ret = false;
        for (EntiteDynamique e : lstEntitesDynamiques) {
            if (directionCourante != null) {
                Entite eBas = e.regarderDansLaDirection(Direction.bas,false);
                Entite eHaut = e.regarderDansLaDirection(Direction.haut,false);
                switch (directionCourante) {
                    case gauche, droite:
                        if (e.avancerDirectionChoisie(directionCourante))
                            ret = true;
                        break;

                    case haut:
                        // on ne peut pas sauter sans prendre appui
                        // (attention, test d'appui réalisé à partir de la position courante, si la gravité à été appliquée, il ne s'agit pas de la position affichée, amélioration possible)
                        if ((eBas != null && eBas.peutPermettreDeMonterDescendre()) || (eHaut !=null && eHaut.peutPermettreDeMonterDescendre())) { // Le heros peut seulement monter grâce à une corde
                            if (e.avancerDirectionChoisie(Direction.haut))
                                ret = true;
                        }
                        break;
                    case bas:
                        if (eBas != null && eBas.peutPermettreDeMonterDescendre()) {
                            if (e.avancerDirectionChoisie(Direction.bas))
                                ret = true;
                        }
                }
            }
        }

        return ret;

    }

    public void resetDirection() {
        directionCourante = null;
    }
}
