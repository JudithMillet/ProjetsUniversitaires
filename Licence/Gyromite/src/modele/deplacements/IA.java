package modele.deplacements;

import modele.plateau.Bot;
import modele.plateau.EntiteDynamique;

import java.util.Random;

/**
 * Intelligence Artificielle IA
 * Permet le déplacement des Bots
 */
public class IA extends RealisateurDeDeplacement {
    private static IA ia;

    public static IA getInstance() {
        if (ia == null) {
            ia = new IA();
        }
        return ia;
    }

    /**
     * Permet de réaliser les déplacements de Bots
     * @return ret : booleen
     */
    protected boolean realiserDeplacement() {
        boolean ret = false;
        for (EntiteDynamique e : lstEntitesDynamiques) {
            if (e instanceof Bot) {
                Bot eb = (Bot) e;
                Direction dir = eb.getDir();
                Random r = new Random();
                Direction randDirV = null;
                Direction randDirH = null;

                // 50 % de chance de changer d'axe de direction
                boolean EstCeQuOnEssayeUneAutreDirection = r.nextBoolean();

                if (r.nextBoolean()) { //création de directions random
                    randDirV = Direction.haut;
                } else {
                    randDirV = Direction.bas;
                }
                if (r.nextBoolean()) { //création de directions random
                    randDirH = Direction.droite;
                } else {
                    randDirH = Direction.gauche;
                }

                if (!EstCeQuOnEssayeUneAutreDirection && eb.getDir() != null &&
                        switch (eb.getDir()) {
                            case haut, bas -> eb.avancerDirectionChoisie(randDirH) || eb.avancerDirectionChoisie(randDirH.opposite());
                            case gauche, droite -> eb.avancerDirectionChoisie(randDirV) || eb.avancerDirectionChoisie(randDirV.opposite());
                        }) {
                    ret = true;
                } else if (eb.avancerDirectionChoisie(dir)) { // on regarde si on peut avancer dans la direction du bot
                    ret = true;
                } else if (eb.avancerDirectionChoisie(randDirV)) { // sinon on regarde si on peut avancer dans une direction random verticale
                    ret = true;
                } else if ((dir != randDirV) && eb.avancerDirectionChoisie(randDirV.opposite())) { // sinon à l'opposée de cette direction random
                    ret = true;
                } else if (eb.avancerDirectionChoisie(randDirH)) { // sinon dans une direction random horizontale
                    ret = true;
                } else if (eb.avancerDirectionChoisie(randDirH.opposite())) { // sinon à l'opposée de la direction random horizontale
                    ret = true;
                } else { //Sinon aucun mouvement possible, on reset la direction
                    eb.resetDirection();
                }

            }

        }
        return ret;
    }
}