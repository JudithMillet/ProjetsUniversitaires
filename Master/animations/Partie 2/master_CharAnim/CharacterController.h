
#ifndef _CHARACTERCONTROLLER_H
#define _CHARACTERCONTROLLER_H

#include "vec.h"
#include "mat.h"
#include "window.h"

class CharacterController
	{
	public:
		CharacterController();

		void update(const float dt); // Dans le cas de la sphère seule
		void update(int &bvh, int &m_frameNumber, const float dt); // Dans le cas du personnage selon le motion graph

		//void turn(const Transform& transf_v);  // pas forcement nécessaire
		void turnXZ(const float& rot_angle_v);
		void accelerate(const float& speed_inc);
		void setVelocityMax(const float vmax);
		void want_kick();
		void want_iddle(int &bvh, int &nb_frame);

		float testDistances(int &m_frameNumber);

		Transform getM_ch2w();
		void setPosition(const Vector& p);
		void setVelocityNorm(const float v);

		const Vector position() const;
		const Vector direction() const;
		float velocity() const;

	protected:
        Transform m_ch2w;   // matrice du character vers le monde
                            // le personnage se déplace vers X
                            // il tourne autour de Y
                            // Z est sa direction droite
                                    
		float m_v;          // le vecteur vitesse est m_v * m_ch2w * Vector(1,0,0)
		float m_vMax;       // ne peut pas accélérer plus que m_vMax
};


#endif
