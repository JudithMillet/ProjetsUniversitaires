
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "CharacterController.h"
// #include "TransformQ.h"

using namespace std;


CharacterController::CharacterController() {
	m_ch2w = Transform();
	m_v = 0;
	m_vMax = 2;
}

void CharacterController::update(const float dt) {

	if (key_state('z') && m_v < m_vMax) accelerate(0.002);
	if (key_state('s') && m_v >= -m_vMax) accelerate(-0.002);
	if (key_state('d')) turnXZ(3);
	if (key_state('q')) turnXZ(-3);

	// update position
	// pos(t+dt) = pos(t) + dt * vit(t+dt);
	Vector pos = position();
	pos = pos + dt * m_v * m_ch2w(Vector(1,0,0));

	setPosition(pos);
}

void CharacterController::update(int &bvh, int &m_frameNumber, const float dt) {
	
	if (key_state('z') && m_v < m_vMax) {
		accelerate(0.1); 
		bvh = 0;
	} 
	if (key_state('s') && m_v > 0.1) {
		accelerate(-0.1); 
		bvh = 0;
	}
	if (key_state('d')) turnXZ(-3);
	if (key_state('q')) turnXZ(3);

	// Kick
	if (key_state('x') || bvh == 3) {
		want_kick(); 
		bvh = 3;

		// Stop kick
		if (m_frameNumber%288 == 0) {
			want_iddle(bvh, m_frameNumber);
		}

	} else {
		// Position neutre
		if (m_v <= 0.1) {
			bvh = 0;
			setPosition(Vector(0,0,0));
		} else {
			// Marche
			if(m_v <= 1 && m_v != 0) bvh = 1;
			//Course
			else if (m_v <= 2.1 && m_v != 0) bvh = 2;

			Vector pos = position();
			pos = pos + dt * 0.1 * m_v * m_ch2w(Vector(1,0,0));

			setPosition(pos);
		}
	}
	
	m_frameNumber++;
}

void CharacterController::turnXZ(const float& rot_angle_v) {
	m_ch2w = m_ch2w * RotationY(rot_angle_v);
}

void CharacterController::accelerate(const float& speed_inc) {
	m_v += speed_inc;
}

void CharacterController::setVelocityMax(const float vmax) {
	m_vMax = vmax;
}

void CharacterController::want_kick() {
	setPosition(Vector(0,0,0)); // stop character
}

void CharacterController::want_iddle(int &bvh, int &nb_frame) {
	bvh = 0;
	nb_frame = 0;
}

Transform CharacterController::getM_ch2w() {
	return m_ch2w;
}

void CharacterController::setPosition(const Vector& p) {
	m_ch2w = Translation(p) * m_ch2w;
}

void CharacterController::setVelocityNorm(const float v) {
	m_v = v;
}

const Vector CharacterController::position() const {
	return m_ch2w(Vector(0,0,0));
}

// const Vector CharacterController::direction() const {
	
// }

float CharacterController::velocity() const {
	return m_v;
}

float CharacterController::testDistances(int &m_frameNumber) {
	// tester chaque frame en calculant la distance --> partir de la frame qui a la distance la plus proche
	return 0.f;
}