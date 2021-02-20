package modele.plateau;

/**
 * Corde
 * Entite statique permettant au Heros et aux Bot de monter, descendre
 */
public class Corde extends EntiteStatique {
    public Corde(Jeu _jeu) {
        super(_jeu);
    }

    public boolean peutServirDeSupport() {
        return false;
    }

    public boolean peutPermettreDeMonterDescendre() {
        return true;
    }

}
