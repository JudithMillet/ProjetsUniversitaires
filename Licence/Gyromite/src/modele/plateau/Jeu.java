/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package modele.plateau;

import modele.deplacements.*;

import java.awt.*;
import java.util.HashMap;

/** Cette classe gère les positions
 * (ajouter conditions de victoire, chargement du plateau, etc.)
 */
public class Jeu {

    public static final int SIZE_X = 20;
    public static final int SIZE_Y = 10;

    // compteur de déplacements horizontal et vertical (1 max par défaut, à chaque pas de temps)
    private HashMap<Entite, Integer> cmptDeplH = new HashMap<Entite, Integer>();
    private HashMap<Entite, Integer> cmptDeplV = new HashMap<Entite, Integer>();

    private Heros hector; // Heros du jeu
    private boolean deposerRadis; // Booleen pour la possiblité de déposer ou non un radis
    private int nbRadis; // Nombre de radis ramassés
    private int score; // Score du Heros
    // 0 en cours, 1 perdu, 2 gagnée
    private int status = 0;

    private int nbBombe = 0;

    private HashMap<Entite, Point> map = new  HashMap<Entite, Point>(); // permet de récupérer la position d'une entité à partir de sa référence
    private Entite[][][] grilleEntites = new Entite[SIZE_X][SIZE_Y][2]; // permet de récupérer une entité à partir de ses coordonnées + 1 3ème dimension pour parcourir les "anciennes" cases

    private Ordonnanceur ordonnanceur = new Ordonnanceur(this);

    public Jeu() throws Exception {
        initialisationDesEntites();
        deposerRadis = false;
        nbRadis = 0;
        score = 0;
    }

    public void resetCmptDepl() {
        cmptDeplH.clear();
        cmptDeplV.clear();
    }

    public void start(long _pause) {
        ordonnanceur.start(_pause);
    }

    public Entite[][][] getGrille() {
        return grilleEntites;
    }

    public int getNbRadis() {
        return nbRadis;
    }

    public boolean getDeposerRadis() {
        return deposerRadis;
    }

    public void setDeposerRadis(boolean depot) {
        deposerRadis = depot;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public int getStatus() {
        return status;
    }

    public int getNbBombe() {
        return nbBombe;
    }

    public void setNbBombe(int nbBombe) {
        this.nbBombe = nbBombe;
    }

    /**
    * Permet d'incrémenter ou de réduire nbRadis selon le booleen
     */
    public void addRadis(boolean radis) {
        if (radis)
            nbRadis++;
        else
            nbRadis--;
    }
    /**
     * Permet d'ajouter un radis à la position courante
     */
    public void ajouterUnRadis(Entite eRadis) {
        Point pRadis = map.get(eRadis);
        addEntite(new Radis(this), pRadis.x, pRadis.y, 0);
    }


    /**
     * Permet de modifier le booleen deposerRadis lorsque l'uti clique sur R
     */
    public void setToDropOff(boolean deposer) {
        if (nbRadis > 0) deposerRadis = deposer;
    }


    public int getScore() {
        return score;
    }

    public void addScore(int points) {
        score += points;
    }

    /**
     * Initialise les entités du jeu
     */
    private void initialisationDesEntites() throws Exception {
        hector = new Heros(this);
        addEntite(hector, 2, 1, 1);

        Gravite g = new Gravite();
        g.addEntiteDynamique(hector);
        ordonnanceur.add(g);

        Controle4Directions.getInstance().addEntiteDynamique(hector);
        ordonnanceur.add(Controle4Directions.getInstance());

        // murs extérieurs horizontaux
        for (int x = 0; x < 20; x++) {
            addEntite(new Sol(this), x, 0, 0);
            addEntite(new Sol(this), x, 9, 0);
        }

        // murs extérieurs verticaux
        for (int y = 1; y < 9; y++) {
            addEntite(new Mur(this), 0, y, 0);
            addEntite(new Mur(this), 19, y, 0);
        }

        // murs intérieurs horizontaux
        for (int i = 1; i < 10; i++) {
            if (i != 5) addEntite(new Sol(this), i, 3, 0);
        }
        for (int j = 11; j < 17; j++)
            addEntite(new Sol(this), j, 5, 0);
        addEntite(new Sol(this), 1, 5, 0);
        addEntite(new Sol(this), 3, 6, 0);
        addEntite(new Sol(this), 4, 6, 0);
        addEntite(new Sol(this), 5, 6, 0);
        addEntite(new Sol(this), 6, 6, 0);
        addEntite(new Sol(this), 7, 6, 0);
        addEntite(new Sol(this), 10, 8, 0);
        addEntite(new Sol(this), 11, 8, 0);
        addEntite(new Sol(this), 13, 8, 0);
        addEntite(new Sol(this), 15, 8, 0);
        addEntite(new Sol(this), 15, 2, 0);
        addEntite(new Sol(this), 16, 2, 0);
        addEntite(new Sol(this), 18, 2, 0);

        // murs intérieurs verticaux
        addEntite(new Mur(this), 7, 4, 0);
        addEntite(new Mur(this), 7, 5, 0);
        addEntite(new Mur(this), 16, 6, 0);
        addEntite(new Mur(this), 16, 7, 0);
        addEntite(new Mur(this), 16, 8, 0);
        addEntite(new Mur(this), 3, 7, 0);
        addEntite(new Mur(this), 3, 8, 0);

       
        // cordes
        for (int c = 1; c < 9; c++) addEntite(new Corde(this), 17, c, 0);
        addEntite(new Corde(this), 2, 4, 0);
        addEntite(new Corde(this), 2, 5, 0);
        addEntite(new Corde(this), 2, 6, 0);
        addEntite(new Corde(this), 2, 7, 0);
        addEntite(new Corde(this), 2, 8, 0);
        addEntite(new Corde(this), 5, 1, 0);
        addEntite(new Corde(this), 5, 2, 0);
        addEntite(new Corde(this), 5, 3, 0);
        addEntite(new Corde(this), 5, 4, 0);
        addEntite(new Corde(this), 5, 5, 0);
        addEntite(new Corde(this), 9, 4, 0);
        addEntite(new Corde(this), 9, 5, 0);
        addEntite(new Corde(this), 9, 6, 0);
        addEntite(new Corde(this), 9, 7, 0);
        addEntite(new Corde(this), 9, 8, 0);

        // Colonnes
        Colonne colonneB01 = new Colonne(this, Couleur.bleu, Direction.bas);
        Colonne colonneB02 = new Colonne(this, Couleur.bleu, Direction.bas);

        addEntite(colonneB01, 12, 6, 0);
        addEntite(colonneB02, 14, 6, 0);

        ControleColonneBleu.getInstance().addEntiteDynamique(colonneB01);
        ControleColonneBleu.getInstance().addEntiteDynamique(colonneB02);

        Colonne colonneR01 = new Colonne(this, Couleur.rouge, Direction.haut);
        Colonne colonneR02 = new Colonne(this, Couleur.rouge, Direction.haut);
        Colonne colonneR03 = new Colonne(this, Couleur.rouge, Direction.haut);

        addEntite(colonneR01, 10, 5, 0);
        addEntite(colonneR02, 10, 6, 0);
        addEntite(colonneR03, 10, 7, 0);
        ControleColonneRouge.getInstance().addEntiteDynamique(colonneR01);
        ControleColonneRouge.getInstance().addEntiteDynamique(colonneR02);
        ControleColonneRouge.getInstance().addEntiteDynamique(colonneR03);

        ordonnanceur.add(ControleColonneBleu.getInstance());
        ordonnanceur.add(ControleColonneRouge.getInstance());

        // Bombes
        addEntite(new Bombe(this),1,8,0);
        addEntite(new Bombe(this),15,7,0);
        addEntite(new Bombe(this),15,1,0);
        nbBombe+=3;

        // Radis
        addEntite(new Radis(this),1,4,0);
        addEntite(new Radis(this),6,5,0);

         // Ennemis
        Bot smick0 = new Bot(this);
        addEntite(smick0, 14, 4, 1);

        IA.getInstance().addEntiteDynamique(smick0);
        ordonnanceur.add(IA.getInstance());

    }

    /**
     * Permet d'ajouter une entité à un position et une dimension choisies
     */
    private void addEntite(Entite e, int x, int y, int dim) {
        grilleEntites[x][y][dim] = e;
        map.put(e, new Point(x, y));
    }

    /**
     * Permet de supprimer une entite en choisissant la dimension
     */
    private void supprimerEntite(Entite e, int dim) {
        Point posE = map.get(e);
        grilleEntites[posE.x][posE.y][dim] = null;
        map.remove(e);
    }

    /**
     * Permet de tuer une entité
     */
    public void tueEntite(Entite e) {
        boolean finirPartie = e instanceof Heros;
        // on prend pas en compte le cas particulier des colonnes car on compte pas supprimer des colonnes
        if (e instanceof EntiteDynamique) {
            supprimerEntite(e, 1);
            if (e instanceof Bot) {
                addScore(5000);
                IA.getInstance().removeEntiteDynamique((EntiteDynamique) e);
            }
            if (e instanceof Heros) {
                ((Heros) e).tuee();
                Controle4Directions.getInstance().removeEntiteDynamique((EntiteDynamique) e);
            }
            if (finirPartie) finirPartie(false);
        } else supprimerEntite(e, 0);
    }


    /**
     * Finit la partie
     */
    public void finirPartie(boolean victoire) {
        setStatus(victoire?2:1);
    }

    /**
     * Permet par exemple a une entité  de percevoir son environnement proche et de définir sa stratégie de déplacement
     */
    public Entite regarderDansLaDirection(Entite e, Direction d, boolean RegarderEntiteDynamique) {
        Point positionEntite = map.get(e);
        return RegarderEntiteDynamique ? objetALaPosition(calculerPointCible(positionEntite, d), 1) : objetALaPosition(calculerPointCible(positionEntite, d), 0);
    }

    /**
     * Si le déplacement de l'entité est autorisé (pas de mur ou autre entité), il est réalisé
     * Sinon, rien n'est fait.
     */
    public boolean deplacerEntite(Entite e, Direction d) {
        boolean retour = false;
        Point pCourant = map.get(e);

        Point pCible = calculerPointCible(pCourant, d);
        Entite eCible = objetALaPosition(pCible);

        if (contenuDansGrille(pCible) && ((eCible == null) || !(eCible.peutServirDeSupport()))) { // a adapter (collisions murs, etc.)
            // compter le déplacement : 1 deplacement horizontal et vertical max par pas de temps par entité
            switch (d) {
                case bas, haut:
                    if (cmptDeplV.get(e) == null) {
                        cmptDeplV.put(e, 1);
                        retour = true;
                    }
                    break;
                case gauche, droite:
                    if (cmptDeplH.get(e) == null) {
                        cmptDeplH.put(e, 1);
                        retour = true;
                    }
                    break;
            }
        }

        if (retour) {
            deplacerEntite(pCourant, pCible, e);
        }
        return retour;
    }


    private Point calculerPointCible(Point pCourant, Direction d) {
        if (d == null) return pCourant;
        return switch (d) {
            case haut -> new Point(pCourant.x, pCourant.y - 1);
            case bas -> new Point(pCourant.x, pCourant.y + 1);
            case gauche -> new Point(pCourant.x - 1, pCourant.y);
            case droite -> new Point(pCourant.x + 1, pCourant.y);
        };
    }

    /**
     * Permet de déplacer une entité d'une position courant à une position dite "cible",
     * Si c'est une colonne, on la déplacera dans les deux dimensions
     */
    private void deplacerEntite(Point pCourant, Point pCible, Entite e) {
        /* Si on a une entité dynamique on la stocke dans la dimension 1 */
        if (e instanceof EntiteDynamique) {
            grilleEntites[pCourant.x][pCourant.y][1] = null;
            grilleEntites[pCible.x][pCible.y][1] = e;
            // Si on a une colonne on la stocke dans les 2 dimensions
            if (e instanceof Colonne) {
                grilleEntites[pCourant.x][pCourant.y][0] = null;
                grilleEntites[pCible.x][pCible.y][0] = e;
            }
        } else {
            grilleEntites[pCourant.x][pCourant.y][0] = null;
            grilleEntites[pCible.x][pCible.y][0] = e;
        }
        map.put(e, pCible);
    }

    /**
     * Indique si p est contenu dans la grille
     */
    private boolean contenuDansGrille(Point p) {
        return p.x >= 0 && p.x < SIZE_X && p.y >= 0 && p.y < SIZE_Y;
    }

    private Entite objetALaPosition(Point p, int dim) {
        Entite retour = null;
        if (contenuDansGrille(p) && dim >= 0 && dim <= 1) {
            retour = grilleEntites[p.x][p.y][dim];
        }
        return retour;
    }

    private Entite objetALaPosition(Point p) {
        return objetALaPosition(p, 0);
    }

    public Ordonnanceur getOrdonnanceur() {
        return ordonnanceur;
    }
}
