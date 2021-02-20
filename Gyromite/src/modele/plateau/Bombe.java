package modele.plateau;

/**
 * Bombe
 * Entité statique pouvant être ramassée et faisant gagner des points
 */
public class Bombe extends EntiteStatique {
    public Bombe(Jeu _jeu) {
        super(_jeu);
    }

    public void ramasserBombe() {
        System.out.println("Bombe ramassée");
        jeu.addScore(1000);
        jeu.setNbBombe(jeu.getNbBombe()-1);
        if (jeu.getNbBombe()==0)jeu.finirPartie(true);
    }

    public boolean peutServirDeSupport() {
        return false;
    }
}
