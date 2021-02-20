package modele.deplacements;

import modele.plateau.Colonne;
import modele.plateau.EntiteDynamique;

/**
 * A la reception d'une commande, toutes les cases (EntitesDynamiques) des colonnes se déplacent dans la direction définie
 * (vérifier "collisions" avec le héros)
 */
public abstract class ControleColonne extends RealisateurDeDeplacement {
    protected boolean bouger;

    public void setToMove(boolean doitBouger) {
        bouger = doitBouger;
    }

    protected abstract boolean isCorrectColor(Colonne colonne);

    protected boolean realiserDeplacement() {
        boolean ret = false;
        for (EntiteDynamique e : lstEntitesDynamiques) {
            if (e instanceof Colonne) {
                Colonne ec = (Colonne) e;
                if (isCorrectColor(ec)) {
                    if (bouger) {
                        if (e.avancerDirectionChoisie(ec.getDirectionMouvement())) {
                            ret = true;
                            ec.OnViensDeBouge(true);
                        }
                    } else {
                        if (!ec.OnABougeJusteAvant()) {
                            if (e.avancerDirectionChoisie(ec.getDirectionMouvement().opposite()))
                                ret = true;
                        }
                        else ec.OnViensDeBouge(false);
                    }
                }
            }

        }
        return ret;
    }

    public void resetOrdreBouger() {
        bouger = false;
    }
}
