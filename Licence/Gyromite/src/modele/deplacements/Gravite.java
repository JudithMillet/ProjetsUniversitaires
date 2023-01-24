package modele.deplacements;

import modele.plateau.Entite;
import modele.plateau.EntiteDynamique;
import modele.plateau.Heros;

public class Gravite extends RealisateurDeDeplacement {
    @Override
    public boolean realiserDeplacement() {
        boolean ret = false;
        if (!lstEntitesDynamiques.isEmpty()) {
            for (EntiteDynamique e : lstEntitesDynamiques) {
                // on peut pas acceder à la classe gravité autrement
                if (e instanceof Heros && !((Heros) e).estMort()){
                    Entite eBas = e.regarderDansLaDirection(Direction.bas, false);
                    Entite ePos = e.regarderDansLaDirection(null, false);
                    if (eBas == null || (!eBas.peutServirDeSupport() && !eBas.peutPermettreDeMonterDescendre()) || (ePos != null && !ePos.peutPermettreDeMonterDescendre())) {
                        if (e.avancerDirectionChoisie(Direction.bas))
                            ret = true;
                    }
                }
            }
        }

        return ret;
    }
}
