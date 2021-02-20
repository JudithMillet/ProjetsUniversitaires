package modele.deplacements;

import modele.plateau.Colonne;
import modele.plateau.Couleur;

/**
 * A la reception d'une commande, toutes les cases (EntitesDynamiques) des colonnes se déplacent dans la direction définie
 * (vérifier "collisions" avec le héros)
 */
public class ControleColonneRouge extends ControleColonne {

    private static ControleColonneRouge ccb;

    public static ControleColonneRouge getInstance() {
        if (ccb == null) {
            ccb = new ControleColonneRouge();
        }
        return ccb;
    }

    protected boolean isCorrectColor(Colonne colonne){
        return colonne.getColor()==Couleur.rouge;
    }
}
