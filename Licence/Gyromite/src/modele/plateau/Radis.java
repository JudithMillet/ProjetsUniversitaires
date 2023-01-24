package modele.plateau;

import modele.plateau.EntiteStatique;
import modele.plateau.Jeu;

/**
 * Radis
 * Entité statique pouvant être ramassé, déposé ou pouvant servir de distraction pour les Smicks
 */
public class Radis extends EntiteStatique {
    public Radis(Jeu _jeu) {
        super(_jeu);
    }

    public void ramasserRadis() {
        System.out.println("Radis ramassé");
        jeu.addRadis(true);
    }

    public void deposerRadis() {
        System.out.println("Radis déposé");
        jeu.addRadis(false);
    }

    public boolean peutServirDeSupport() {
        return false;
    }
}