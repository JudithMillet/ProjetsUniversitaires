package modele.deplacements;

import modele.plateau.Colonne;
import modele.plateau.Couleur;


/**
 * A la reception d'une commande, toutes les cases (EntitesDynamiques) des colonnes se déplacent dans la direction définie
 * (vérifier "collisions" avec le héros)
 */
public class ControleColonneBleu extends ControleColonne {

    private static ControleColonneBleu ccb;

    public static ControleColonneBleu getInstance() {
        if (ccb == null) {
            ccb = new ControleColonneBleu();
        }
        return ccb;
    }

    protected boolean isCorrectColor(Colonne colonne){
        return colonne.getColor()==Couleur.bleu;
    }
}
