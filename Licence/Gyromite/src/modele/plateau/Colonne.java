package modele.plateau;


import modele.deplacements.Direction;

/**
 * Colonne
 * Entite dynamique pouvant être déplacée verticalement
 * Peut servir pour monter et peut écraser une entité dynamique
 */
public class Colonne extends EntiteDynamique {
    //blue by default; blue -> false, red -> true
    private final Couleur color;
    private final Direction directionMouvement;

    private boolean onABougeJusteAvant = false;

    public Direction getDirectionMouvement() {
        return directionMouvement;
    }

    public Couleur getColor() {
        return color;
    }

    public Colonne(Jeu _jeu, Couleur _color, Direction direction) throws Exception {
        super(_jeu);
        this.color = _color;
        if (direction == Direction.haut || direction == Direction.bas) {
            directionMouvement = direction;
        } else throw new Exception("Les colonnes ne bougent pas latéralement");
    }

    public boolean peutEtreEcrase() {
        return false;
    }

    public boolean peutServirDeSupport() {
        return true;
    }

    public boolean peutPermettreDeMonterDescendre() {
        return false;
    }

    public boolean OnABougeJusteAvant() {
        return onABougeJusteAvant;
    }
    public void OnViensDeBouge(boolean rep) {
        onABougeJusteAvant = rep;
    }

    @Override
    public boolean avancerDirectionChoisie(Direction d){
        if(d == null) return false;
        switch (d) {
            case haut -> {
                    boolean EntiteDessusVide = true;
                    Entite EntiteDynDessus = regarderDansLaDirection(Direction.haut,true);
                    Entite EntiteDessusDessus = EntiteDynDessus!=null?EntiteDynDessus.regarderDansLaDirection(Direction.haut,false):null;
                    Entite EntiteStatiqueDessus = regarderDansLaDirection(Direction.haut,false);
                    if (EntiteStatiqueDessus==null) {
                        if (EntiteDynDessus instanceof Heros || EntiteDynDessus instanceof Bot) {
                            if (EntiteDessusDessus == null) {
                                jeu.deplacerEntite(EntiteDynDessus, Direction.haut);
                            } else EntiteDessusVide = false;
                        }
                        if (EntiteDessusVide) {
                            if (EntiteDynDessus instanceof Colonne) {
                                Colonne cd = (Colonne) EntiteDynDessus;
                                cd.avancerDirectionChoisie(Direction.haut);
                            }
                            return super.avancerDirectionChoisie(d);
                        }
                    }
            }
            case bas -> {
                Entite EntiteDessous = regarderDansLaDirection(Direction.bas,true);
                Entite EntiteDessousDessous = EntiteDessous!=null?EntiteDessous.regarderDansLaDirection(Direction.bas,false):null;
                if (EntiteDessous!=null && EntiteDessous.peutEtreEcrase()){
                    if (EntiteDessousDessous!=null){
                        jeu.tueEntite(EntiteDessous);
                    }
                }
                if (EntiteDessous instanceof Colonne){
                    Colonne cd = (Colonne) EntiteDessous;
                    cd.avancerDirectionChoisie(Direction.bas);
                }
                    return super.avancerDirectionChoisie(d);
            }

        }
        return false;
    }
}
