/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package modele.plateau;

import modele.deplacements.Direction;

/**
 * Héros du jeu
 * Entité dynamique
 */
public class Heros extends EntiteDynamique {

    private boolean mort = false;

    public boolean estMort() {
        return mort;
    }

    public void tuee() {
        this.mort = true;
    }

    public Heros(Jeu _jeu) {
        super(_jeu);
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

    /**
     * Permet de gérer si le Heros peut avancer dans la direction choisie, ainsi que de ramasser des radis ou des bombes et de déposer des radis
     */
    @Override
    public boolean avancerDirectionChoisie(Direction d) {
        boolean OnBouge = false;
        Entite objetPosFutur = regarderDansLaDirection(d, false);
        Entite eSousHeros = jeu.regarderDansLaDirection(this, null, false);

        // On regarde si le Heros veut déposer un radis (le joueur a cliqué sur R)
        if (jeu.getDeposerRadis()) {
            if (!(eSousHeros instanceof Corde)) { // Si le Heros n'est pas sur une corde, il peut déposer un radis
                jeu.ajouterUnRadis(this);
                jeu.addRadis(false);
            }
            jeu.setDeposerRadis(false);
        }

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

                Entite objetPosDessous = regarderDansLaDirection(Direction.bas, false);
                if (objetPosDessous != null && (objetPosDessous.peutServirDeSupport() || objetPosDessous.peutPermettreDeMonterDescendre()) && (objetPosFutur == null || !objetPosFutur.peutServirDeSupport()))
                    OnBouge = true;
            }
        }

        if (OnBouge) {
            // On regarde si le Heros peut ramasser un radis
            if (objetPosFutur instanceof Radis) {
                ((Radis) objetPosFutur).ramasserRadis();
                jeu.tueEntite(objetPosFutur);
            }

            if (objetPosFutur instanceof Bombe) {
                ((Bombe) objetPosFutur).ramasserBombe();
                jeu.tueEntite(objetPosFutur);
            }
            OnBouge = super.avancerDirectionChoisie(d);
        }
        return OnBouge;
    }
}
