/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package modele.plateau;

import modele.deplacements.Direction;

/**
 * Ennemis (Smicks)
 * Entite dynamique
 */
public class Bot extends EntiteDynamique {
    private static final long TPSOCCUPE = 3000;
    private Direction directionBot;
    private long tpsDebutOccupe;

    public Bot(Jeu _jeu) {
        super(_jeu);
        tpsDebutOccupe = 0;
    }

    public boolean peutEtreEcrase() {
        return true;
    }

    public boolean peutServirDeSupport() {
        return true;
    }

    public boolean peutPermettreDeMonterDescendre() {
        return false;
    }

    public boolean estOccupe() {

        return (System.currentTimeMillis() - tpsDebutOccupe) <= TPSOCCUPE;
    }

    public void setOccupe() {
        tpsDebutOccupe = System.currentTimeMillis();
    }
    /**
     * Permet de gérer si le Bot peut avancer dans la direction choisie en vérifiant qu'il n'est pas distrait par un radis
     */
    @Override
    public boolean avancerDirectionChoisie(Direction d) {
        boolean OnBouge = false;
        if (d == null) return false;
        if (regarderDansLaDirection(null, false) instanceof Radis) { // S'il se retrouve sur un radis, il est distrait pendant 3 secondes
            setOccupe();
            jeu.tueEntite(regarderDansLaDirection(null, false));
        } else if (!estOccupe()) { // Le Bot ne doit pas être distrait par un radis pour avancer
            switch (d) {
                case haut -> {
                    Entite objetPosActuelle = regarderDansLaDirection(null, false);
                    Entite objetPosDessus = regarderDansLaDirection(Direction.haut, false);
                    if ((objetPosActuelle != null && objetPosActuelle.peutPermettreDeMonterDescendre() && (objetPosDessus == null || !objetPosDessus.peutServirDeSupport())))
                        OnBouge = true;
                }
                case bas -> {
                    Entite objetPosDessous = regarderDansLaDirection(Direction.bas, false);
                    if ((objetPosDessous == null || !objetPosDessous.peutServirDeSupport()))
                        OnBouge = true;
                }
                case gauche, droite -> {
                    Entite objetPosDir = regarderDansLaDirection(d, false);
                    Entite objetPosDessous = regarderDansLaDirection(Direction.bas, false);
                    Entite objetPosDessousDir = (objetPosDessous != null ? objetPosDessous.regarderDansLaDirection(d, false) : null);
                    if (objetPosDessous != null && objetPosDessousDir != null && (objetPosDessousDir.peutPermettreDeMonterDescendre() || objetPosDessousDir.peutServirDeSupport()) && (objetPosDessous.peutServirDeSupport() || objetPosDessous.peutPermettreDeMonterDescendre()) && (objetPosDir == null || !objetPosDir.peutServirDeSupport()))
                        OnBouge = true;
                }
            }
        }
        if (OnBouge) {
            Entite entiteDynPosFutur = regarderDansLaDirection(d, true);
            if (entiteDynPosFutur instanceof Heros) { // Un bot tue le Heros
                // probleme on tue avant la verification de mouvement unique
                jeu.tueEntite(entiteDynPosFutur);
            }
            if (super.avancerDirectionChoisie(d)) {
                changerDirection(d);
                return true;
            }
        }
        return false;
    }

    public Direction getDir() {
        return this.directionBot;
    }

    public void changerDirection(Direction dc) {
        this.directionBot = dc;
    }

    public void resetDirection() {
        directionBot = null;
    }
}
