/*
 * CalculsMSS.cpp :
 * Copyright (C) 2016 Florence Zara, LIRIS
 *               florence.zara@liris.univ-lyon1.fr
 *               http://liris.cnrs.fr/florence.zara/
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file CalculsMSS.cpp
Programme calculant pour chaque particule i d un MSS son etat au pas de temps suivant 
 (methode d 'Euler semi-implicite) : principales fonctions de calculs.
\brief Fonctions de calculs de la methode semi-implicite sur un systeme masses-ressorts.
*/ 

#include <cstddef>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "vec.h"
#include "ObjetSimule.h"
#include "ObjetSimuleMSS.h"
#include "Viewer.h"

using namespace std;





/**
* Calcul des forces appliquees sur les particules du systeme masses-ressorts.
 */
void ObjetSimuleMSS::CalculForceSpring()
{
	//Vector f = Vector(0, 0, 0);
	
	for (int i = 0 ; i < _SystemeMasseRessort->GetNbRessort() ; i++) {
		Ressort* r = _SystemeMasseRessort->GetRessortList()[i];

		Particule* pA = r->GetParticuleA();
		Particule* pB = r->GetParticuleB();

		Vector vitesse = V[pB->GetId()] - V[pA->GetId()];

		// vecteur u normalisé
		Vector uij = normalize(P[pB->GetId()] - P[pA->GetId()]);

		float tmp = - r->GetRaideur() * 
				(length(P[pB->GetId()] - P[pA->GetId()]) - r->GetLrepos())
				+ (- r->GetAmortissement() * 
					(uij.x * vitesse.x +
					uij.y * vitesse.y +
					uij.z * vitesse.z )
				);

		Vector f_tmp = tmp * uij ;

		Force[pA->GetId()] = Force[pA->GetId()] - f_tmp;
		Force[pB->GetId()] = Force[pB->GetId()] + f_tmp;

		// Déchirure
		if(length(P[pB->GetId()] - P[pA->GetId()]) > 0.5) {
			_SystemeMasseRessort->GetRessortList().erase(_SystemeMasseRessort->GetRessortList().begin()+i);
		}

	}

	/// f = somme_i (ki * (l(i,j)-l_0(i,j)) * uij ) + (n * uij * (vi - vj)) * uij + (m*g) + force_ext

	/// Rq : Les forces dues a la gravite et au vent sont ajoutees lors du calcul de l acceleration

}//void


/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleMSS::Collision(const float &radius_sphere)
{
	// --> ramener drap dans monde du plan
	for (unsigned int i = 0; i < P.size()-1; i++) {
		Vector p_part = P[i]; // position de la particule courante

		// Translate de la particule dans la scene
		p_part = Vector(p_part.x, p_part.y + 10, p_part.z); 

		// Colision avec plan
		if (p_part.y <= 0) {
			V[i] = 0;
		}


		// Cas d'une sphere
		Transform t = Translation(1.7, 0, 0) * Scale(10,10,10) * Identity();
		Point p_sphere = t(Point(0,0,0)); // Position de la sphere

		float distance = sqrt((p_part.x - p_sphere.x) * (p_part.x - p_sphere.x) +
							(p_part.y - p_sphere.y) * (p_part.y - p_sphere.y) +
							(p_part.z - p_sphere.z) * (p_part.z - p_sphere.z));

		// collision avec sphere
		if (distance < radius_sphere) {
			V[i].y = 0;


			/// Essai de gestion de collision plus réelle
			// Vector normal = Vector(p_part.x - p_sphere.x, p_part.y - p_sphere.y, p_part.z - p_sphere.z);
			// // double c = (V[i]).dot(normal);

			// double scalar = V[i].x * normal.x + V[i].y * normal.y + V[i].z * normal.z;

			// double cos_angle = (normal.x / distance);
			// std::cout << "collision" << std::endl;
			
			// V[i].x += scalar*cos_angle;
			// V[i].y += scalar*sin(acos(cos_angle)); 
		}
	}
    
}// void