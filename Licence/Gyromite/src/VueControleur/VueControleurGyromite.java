package VueControleur;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Observable;
import java.util.Observer;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.*;

import modele.deplacements.Controle4Directions;
import modele.deplacements.ControleColonneBleu;
import modele.deplacements.ControleColonneRouge;
import modele.deplacements.Direction;
import modele.plateau.*;


/** Cette classe a deux fonctions :
 *  (1) Vue : proposer une représentation graphique de l'application (cases graphiques, etc.)
 *  (2) Controleur : écouter les évènements clavier et déclencher le traitement adapté sur le modèle (flèches direction Pacman, etc.))
 *
 */
public class VueControleurGyromite extends JFrame implements Observer {
    private Jeu jeu; // référence sur une classe de modèle : permet d'accéder aux données du modèle pour le rafraichissement, permet de communiquer les actions clavier (ou souris)

    private int sizeX; // taille de la grille affichée
    private int sizeY;
    private int resizeX; // taille de la grille affichée redimensionnée
    private int resizeY;
    private boolean finished;

    // icones affichées dans la grille
    private ImageIcon icoHero;
    private ImageIcon icoHeroCorde;
    private ImageIcon icoBot;
    private ImageIcon icoVide;
    private ImageIcon icoMur;
    private ImageIcon icoSol;
    private ImageIcon icoCorde;
    private ImageIcon icoBombe;
    private ImageIcon icoRadis;

    //bit le plus à droite -> couleur, 0 bleu, 1 rouge
    //milieu -> si la colonne a un bas, gauche -> si la colonne a un haut
    private ImageIcon[] icoColonne;

    private JLabel[][] tabJLabel; // cases graphique (au moment du rafraichissement, chaque case va être associée à une icône, suivant ce qui est présent dans le modèle)
    private JLabel JLabelRadis;
    private JLabel JLabelScore;
    private JLabel JLabelFin;
    private JButton BoutonExit;

    public VueControleurGyromite(Jeu _jeu) {
        jeu = _jeu;
        sizeX = jeu.SIZE_X;
        sizeY = jeu.SIZE_Y;
        resizeX = 670;
        resizeY = 420;
        addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                resizeX = getWidth();
                resizeY = getHeight();
                chargerLesIcones();
            }
        });
        chargerLesIcones();
        placerLesComposantsGraphiques();
        ajouterEcouteurClavier();
        setLocationRelativeTo(null);
    }

    private void ajouterEcouteurClavier() {
        addKeyListener(new KeyAdapter() { // new KeyAdapter() { ... } est une instance de classe anonyme, il s'agit d'un objet qui correspond au controleur dans MVC
            @Override
            public void keyPressed(KeyEvent e) {
                // on regarde quelle touche a été pressée
                switch (e.getKeyCode()) {
                    case KeyEvent.VK_LEFT -> Controle4Directions.getInstance().setDirectionCourante(Direction.gauche);
                    case KeyEvent.VK_RIGHT -> Controle4Directions.getInstance().setDirectionCourante(Direction.droite);
                    case KeyEvent.VK_DOWN -> Controle4Directions.getInstance().setDirectionCourante(Direction.bas);
                    case KeyEvent.VK_UP -> Controle4Directions.getInstance().setDirectionCourante(Direction.haut);
                    case KeyEvent.VK_A -> ControleColonneBleu.getInstance().setToMove(true);
                    case KeyEvent.VK_Z -> ControleColonneRouge.getInstance().setToMove(true);
                    case KeyEvent.VK_R -> jeu.setToDropOff(true);
                }
            }
        });
    }


    private void chargerLesIcones() {
        icoHero = chargerIcone("/images/ProfBaissé.png");
        icoHeroCorde = chargerIcone("/images/ProfCorde.png");
        icoBot = chargerIcone("/images/Bot.png");
        icoVide = chargerIcone("/images/Vide.png");
        icoMur = chargerIcone("/images/Mur.png");
        icoSol = chargerIcone("/images/Sol.png");
        icoCorde = chargerIcone("/images/Corde.png");
        icoBombe = chargerIcone("/images/Bombe.png");
        icoRadis = chargerIcone("/images/Radis.png");

        icoColonne = new ImageIcon[8];
        icoColonne[0b000] = chargerIcone("/images/ColonneBleuMilieu.png");
        icoColonne[0b010] = chargerIcone("/images/ColonneBleuBas.png");
        icoColonne[0b100] = chargerIcone("/images/ColonneBleuHaut.png");
        icoColonne[0b110] = chargerIcone("/images/ColonneBleuUnique.png");
        icoColonne[0b101] = chargerIcone("/images/ColonneRougeHaut.png");
        icoColonne[0b001] = chargerIcone("/images/ColonneRougeMilieu.png");
        icoColonne[0b011] = chargerIcone("/images/ColonneRougeBas.png");
        icoColonne[0b111] = chargerIcone("/images/ColonneRougeUnique.png");
    }

    private ImageIcon chargerIcone(String urlIcone) {
        BufferedImage image = null;

        try {
            image = ImageIO.read(VueControleurGyromite.class.getResourceAsStream(urlIcone));
        } catch (IOException ex) {
            Logger.getLogger(VueControleurGyromite.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        Image redimension = image.getScaledInstance(resizeX/sizeX, resizeY/sizeY, Image.SCALE_SMOOTH);
        return new ImageIcon(redimension);
    }

    private void placerLesComposantsGraphiques() {
        setTitle("Gyromite");
        setSize(670, 420); // 20 width 10 height
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // permet de terminer l'application à la fermeture de la fenêtre

        JComponent grilleJLabels = new JPanel(new GridLayout(sizeY, sizeX)); // grilleJLabels va contenir les cases graphiques et les positionner sous la forme d'une grille

        tabJLabel = new JLabel[sizeX][sizeY];

        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                JLabel jlab = new JLabel();
                tabJLabel[x][y] = jlab; // on conserve les cases graphiques dans tabJLabel pour avoir un accès pratique à celles-ci (voir mettreAJourAffichage() )
                grilleJLabels.add(jlab);
            }
        }

        // Label affichant le nombre de radis en bas de la fenêtre
        JLabelRadis = new JLabel("Nombre de radis : "+ jeu.getNbRadis());
        JLabelRadis.setHorizontalAlignment(SwingConstants.LEFT);
        JLabelRadis.setVerticalAlignment(SwingConstants.TOP);
        add(JLabelRadis,BorderLayout.SOUTH);

        // Label affichant le score en haut de la fenêtre
        JLabelScore = new JLabel("Score : "+ jeu.getScore());
        JLabelScore.setHorizontalAlignment(SwingConstants.RIGHT);
        JLabelScore.setVerticalAlignment(SwingConstants.TOP);
        add(JLabelScore,BorderLayout.NORTH);

        add(grilleJLabels, BorderLayout.CENTER);
    }

    private void AfficherLaFin() {
        getContentPane().removeAll();
        revalidate();
        repaint();
        setTitle("Gyromite");
        setSize(670, 420); // 20 width 10 height
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // permet de terminer l'application à la fermeture de la fenêtre

        JComponent grilleJLabels = new JPanel(new GridLayout(sizeY, sizeX)); // grilleJLabels va contenir les cases graphiques et les positionner sous la forme d'une grille

        String Fin ="";
        if (jeu.getStatus()==1) Fin ="Perdu, Pas de chance réessayez une prochaine fois";
        else if (jeu.getStatus()==2) Fin = "Bravo, Vous avez gagné";
        JLabelFin = new JLabel(Fin);
        JLabelFin.setHorizontalAlignment(SwingConstants.CENTER);
        JLabelFin.setVerticalAlignment(SwingConstants.TOP);

        add(JLabelFin,BorderLayout.NORTH);

        JLabelScore = new JLabel("Avec un score de "+ jeu.getScore());
        JLabelScore.setHorizontalAlignment(SwingConstants.CENTER);
        JLabelScore.setVerticalAlignment(SwingConstants.CENTER);

        JButton BoutonExit = new JButton("Quittez");

        BoutonExit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.exit(0);
            }
        });

        BoutonExit.setHorizontalAlignment(SwingConstants.CENTER);
        BoutonExit.setVerticalAlignment(SwingConstants.CENTER);
        add(BoutonExit);

        add(JLabelScore,BorderLayout.SOUTH);
        setVisible(true);
        
    }
    
    /**
     * Il y a une grille du côté du modèle ( jeu.getGrille() ) et une grille du côté de la vue (tabJLabel)
     */
    private void mettreAJourAffichage() {

        for (int x = 0; x < sizeX; x++) {
            for (int y = 0; y < sizeY; y++) {
                if (jeu.getGrille()[x][y][1] instanceof Heros) { // si la grille du modèle contient un Heros, on associe l'icône Heros du côté de la vue
                    // System.out.println("Héros !");
                    if(jeu.getGrille()[x][y][0] instanceof Corde) tabJLabel[x][y].setIcon(icoHeroCorde);
                    else tabJLabel[x][y].setIcon(icoHero);

                } else if (jeu.getGrille()[x][y][1] instanceof Bot) {
                    tabJLabel[x][y].setIcon(icoBot);
                }else if (jeu.getGrille()[x][y][0] instanceof Mur) {
                    tabJLabel[x][y].setIcon(icoMur);
                }else if (jeu.getGrille()[x][y][0] instanceof Sol) {
                    tabJLabel[x][y].setIcon(icoSol);
                } else if (jeu.getGrille()[x][y][0] instanceof Colonne) {
                    boolean colAuDessus = jeu.getGrille()[x][y-1][0] instanceof Colonne;
                    boolean colEnDessous = jeu.getGrille()[x][y+1][0] instanceof Colonne;
                    Couleur col = ((Colonne)jeu.getGrille()[x][y][0]).getColor();
                    int val = 0;
                    if (col==Couleur.rouge) val |= 0x1;
                    if (!colEnDessous) val |= 0x2;
                    if (!colAuDessus) val |= 0x4;
                    tabJLabel[x][y].setIcon(icoColonne[val]);
                } else if (jeu.getGrille()[x][y][0] instanceof Corde) {
                    tabJLabel[x][y].setIcon(icoCorde);
                } else if (jeu.getGrille()[x][y][0] instanceof Bombe) {
                    tabJLabel[x][y].setIcon(icoBombe);
                } else if (jeu.getGrille()[x][y][0] instanceof Radis) {
                tabJLabel[x][y].setIcon(icoRadis);
                } else {
                    tabJLabel[x][y].setIcon(icoVide);
                }
            }
        }
    }

    /**
     * Met à jour le nombre de radis
     */
    private void mettreAJourRadis() {
        JLabelRadis.setText("Nombre de radis : "+ jeu.getNbRadis());
    }

    /**
     * Met à jour le score
     */
    private void mettreAJourScore() {
        JLabelScore.setText("Score : "+ jeu.getScore());
    }

    private boolean CheckState() {
        if(jeu.getStatus()!=0){
            if(!finished){
                AfficherLaFin();
                finished =true;
            }
            return false;
        }
        return true;
    }

    @Override
    public void update(Observable o, Object arg) {
        if(CheckState()) {
            mettreAJourAffichage();
            mettreAJourRadis();
            mettreAJourScore();
        }
    }
}
