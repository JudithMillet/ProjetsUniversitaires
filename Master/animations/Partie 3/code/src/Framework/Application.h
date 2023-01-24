#ifndef APPLICATION_H
#define APPLICATION_H

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include <stdlib.h>

#include "Box2D/Box2D.h"
#include "DebugDraw.h"
#include "Creature.h"

class Application;

#define	RAND_LIMIT	32767
#define DRAW_STRING_NEW_LINE 16

/// Random number in range [-1,1]
inline float32 RandomFloat() {
	float32 r = (float32)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = 2.0f * r - 1.0f;
	return r;
}

/// Random floating point number in range [lo, hi]
inline float32 RandomFloat(float32 lo, float32 hi) {
	float32 r = (float32)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

/// Test settings. Some can be controlled in the GUI.
struct Settings {
	Settings() {
		rotationPlateforme = false;
		launchBalls = false;
		launchObject = false;
		showCOM = false;
		balanceControl = true;
		motionTracking = false;
		drawJoints = false;
		drawContactPoints = false;
		drawContactNormals = false;

		//pause = false;
	}
	bool rotationPlateforme;
	bool launchBalls;
	bool launchObject;
	bool showCOM;
	bool balanceControl;
	bool motionTracking;
	bool drawJoints;
	bool drawContactPoints;
	bool drawContactNormals;
	//bool pause;
};

// This is called when a joint in the world is implicitly destroyed
// because an attached body is destroyed. This gives us a chance to
// nullify the mouse joint.
class DestructionListener : public b2DestructionListener {
public:
	void SayGoodbye(b2Fixture* fixture) override { B2_NOT_USED(fixture); }
	void SayGoodbye(b2Joint* joint) override;
	Application* app;
};

const int32 k_maxContactPoints = 2048;

struct ContactPoint {
	b2Fixture* fixtureA;
	b2Fixture* fixtureB;
	b2Vec2 normal;
	b2Vec2 position;
	b2PointState state;
	float32 normalImpulse;
	float32 tangentImpulse;
	float32 separation;
};

class Application : public b2ContactListener {
public:
	Application();
	virtual ~Application();

	void DrawTitle(const char *string);
	virtual void Step(Settings* settings);
	virtual void Keyboard(int key) { B2_NOT_USED(key); }
	virtual void KeyboardUp(int key) { B2_NOT_USED(key); }
	virtual void MouseDown(const b2Vec2& p);
	virtual void MouseUp(const b2Vec2& p);
	void MouseMove(const b2Vec2& p);

	void LaunchBall();
	void LaunchObject();
	b2Body* getBall();
	b2Body* getObject();

	// Let derived tests know that a joint was destroyed.
	virtual void JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }

	// Callbacks for derived classes.
	virtual void BeginContact(b2Contact* contact)  override { B2_NOT_USED(contact); }
	virtual void EndContact(b2Contact* contact)  override { B2_NOT_USED(contact); }
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
	}

	void ShiftOrigin(const b2Vec2& newOrigin);

protected:
	friend class DestructionListener;
	friend class BoundaryListener;
	friend class ContactListener;

	b2AABB m_worldAABB;
	ContactPoint m_points[k_maxContactPoints];
	int32 m_pointCount;
	DestructionListener m_destructionListener;
	int32 m_textLine;
	b2World* m_world;
	b2MouseJoint* m_mouseJoint;
	b2Vec2 m_mouseWorld;
	int32 m_stepCount;
	b2Profile m_maxProfile;
	b2Profile m_totalProfile;

	b2Body* m_groundBody;                   // le sol
	b2Body* m_plateform;                    // la plateforme
	b2Body* m_ball;                         // la balle
    b2Body* m_object;                       // l'objet
	b2Body*	m_COM;		                    // le CdM
	Creature * m_creature;                  // la creature
	float32 m_magnitudeToRotatePlatform;    // l'intensite de rotation
	bool m_rotateClockwise;                 // sens de rotation
	double m_lastChangeRotation;            // temps du dernier changement de rotation
	double m_lastShoot;                     // temps du dernier lancement de balle
	double m_startBalance;                  // temps du dernier demarrage de l'equilibre
	double m_balancedDuration;              // temps ecoule en equilibre (en secondes)

};

#endif
