/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package modele.deplacements;

/** Type énuméré des directions : les directions correspondent à un ensemble borné de valeurs, connu à l'avance
 *
 *
 */
public enum Direction {
    haut, bas, gauche, droite;
    public Direction opposite() {
        return switch (this) {
            case haut -> Direction.bas;
            case bas -> Direction.haut;
            case gauche -> Direction.droite;
            case droite -> Direction.gauche;
            default -> throw new IllegalStateException("This should never happen: " + this + " has no opposite.");
        };
    }
}
