package VueControleur;

import modele.plateau.Jeu;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * VueMenu est la fenêtre de menu au lancement du jeu
 * Donnant le but du jeu et les commandes utiles
 */
public class VueMenu extends JFrame {
    Jeu jeu = new Jeu();
    VueControleurGyromite vc = new VueControleurGyromite(jeu);

    public VueMenu() throws Exception {
        setTitle("Menu du Gyromite");
        setSize(670,420);
        setResizable(true); // Permet de redimensionner le menu
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setLocationRelativeTo(null);
        //Création des panels
        JPanel content = new JPanel();
        JPanel panel = new JPanel();
        panel.setLayout(new FlowLayout());
        panel.setBackground(Color.LIGHT_GRAY);
        JButton bouton1 = new JButton("Nouvelle partie");
        panel.add(bouton1);
        bouton1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Clique");
                jeu.getOrdonnanceur().addObserver(vc);
                vc.setVisible(true);
                jeu.start(300);
            }
        });
        JButton bouton2 = new JButton("Quittez");
        panel.add(bouton2);
        bouton2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.exit(0);
            }
        });
        content.add(panel);
        getContentPane().add(content, BorderLayout.CENTER);

        JPanel content2 = new JPanel();
        JPanel panel2 = new JPanel();
        panel2.setLayout(new FlowLayout());
        panel2.setBackground(Color.LIGHT_GRAY);
        JTextArea textArea = new JTextArea("\nLe but du jeu est de ramasser toutes les bombes en évitant les Smicks.\n"+
                "Vous pouvez les distraire en ramassant des radis et en les déposant à des endroits stratégiques.\n" +
                "Attention aux colonnes ! Elles peuvent vous écraser mais elles peuvent aussi écraser les Smicks ...\n" +
                "A bon entendeur ......\n\n" +
                "\nCommandes utiles : \n\n" +
                "Les flèches pour se déplacer\n" +
                "R pour ramasser un radis\n" +
                "A pour déplacer horizontalement les colonnes bleues\n" +
                "Z pour déplacer horizontalement les colonnes rouges\n"+
                "\nAuteurs :\n" +
                        "Judith Millet et Antoine Rybacki", 5, 40);
        textArea.setEditable(false);
        panel2.add(textArea);
        content2.add(panel2);
        getContentPane().add(content2, BorderLayout.SOUTH);
    }

}
