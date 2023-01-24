#include "Application.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

void DestructionListener::SayGoodbye(b2Joint* joint) {
	if (app->m_mouseJoint == joint) app->m_mouseJoint = NULL;
	else app->JointDestroyed(joint);
}

Application::Application() {

    // Gravity and world
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.81f);
	m_world = new b2World(gravity);

	// Init
	m_textLine = 30;
	m_mouseJoint = NULL;
	m_COM = NULL;
	m_ball = NULL;
	m_object = NULL;
	m_pointCount = 0;
	m_destructionListener.app = this;
	m_world->SetDestructionListener(&m_destructionListener);
	m_world->SetContactListener(this);
	m_world->SetDebugDraw(&g_debugDraw);
	m_world->SetAllowSleeping(true);
	m_world->SetWarmStarting(true);
	m_world->SetContinuousPhysics(true);
	m_world->SetSubStepping(false);
	m_stepCount = 0;
	memset(&m_maxProfile, 0, sizeof(b2Profile));
	memset(&m_totalProfile, 0, sizeof(b2Profile));
	m_magnitudeToRotatePlatform = 0.0f;
	m_rotateClockwise = 0;
	m_lastChangeRotation = glfwGetTime();
	m_startBalance = glfwGetTime();
	m_lastShoot = m_lastChangeRotation;
	m_balancedDuration = 0.0f;

	// Ground body
	{
        b2BodyDef bodyDef;
        m_groundBody = m_world->CreateBody(&bodyDef);
		b2EdgeShape shape;
		shape.Set(b2Vec2(-5.0f, 0.0f), b2Vec2(5.0f, 0.0f));
		m_groundBody->CreateFixture(&shape, 0.0f);
	}

	// Plateform body
	{
	    b2BodyDef bodyDef;
	    bodyDef.position.Set(0.0f,1.0f);
	    bodyDef.fixedRotation = false;
		bodyDef.allowSleep = false;
		bodyDef.awake = true;
	    m_plateform = m_world->CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(2.0f, 0.02f);
		m_plateform->CreateFixture(&shape,0.0f);
	}

	// Creature
	m_creature = new Creature(m_world);

}

Application::~Application() {
	// By deleting the world, we delete the ball, ground, creature bodies and joints, etc.
	delete m_world;	m_world = NULL;
	delete m_creature; m_creature = NULL;
}

void Application::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	const b2Manifold* manifold = contact->GetManifold();
	if (manifold->pointCount == 0) return;

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
	b2GetPointStates(state1, state2, oldManifold, manifold);

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	for (int32 i = 0; i < manifold->pointCount && m_pointCount < k_maxContactPoints; ++i) {
		ContactPoint* cp = m_points + m_pointCount;
		cp->fixtureA = fixtureA;
		cp->fixtureB = fixtureB;
		cp->position = worldManifold.points[i];
		cp->normal = worldManifold.normal;
		cp->state = state2[i];
		cp->normalImpulse = manifold->points[i].normalImpulse;
		cp->tangentImpulse = manifold->points[i].tangentImpulse;
		cp->separation = worldManifold.separations[i];
		++m_pointCount;
	}
}

void Application::DrawTitle(const char *string) {
    g_debugDraw.DrawString(5, 5, string);
    m_textLine = 3 * DRAW_STRING_NEW_LINE;
}

class QueryCallback : public b2QueryCallback {
public:
	QueryCallback(const b2Vec2& point) 	{
		m_point = point;
		m_fixture = NULL;
	}
	bool ReportFixture(b2Fixture* fixture) override {
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody) {
			bool inside = fixture->TestPoint(m_point);
			if (inside) {
				m_fixture = fixture;
				// We are done, terminate the query.
				return false;
			}
		}
		// Continue the query.
		return true;
	}
	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

void Application::MouseDown(const b2Vec2& p) {
	m_mouseWorld = p;
	if (m_mouseJoint != NULL) return;

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(p);
	m_world->QueryAABB(&callback, aabb);

	if (callback.m_fixture) {
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = body;
		md.target = p;
		md.maxForce = 1000.0f * body->GetMass();
		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
		body->SetAwake(true);
	}
}

void Application::MouseUp(const b2Vec2& p) {
	if (m_mouseJoint) {
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
}

void Application::MouseMove(const b2Vec2& p) {
	m_mouseWorld = p;
	if (m_mouseJoint) m_mouseJoint->SetTarget(p);
}

void Application::LaunchBall() {
	if (m_ball)	{
		m_world->DestroyBody(m_ball);
		m_ball = NULL;
	}

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	// double angleAutour = ((rand() % 180) / 180.0f) * b2_pi;
	// bd.position = b2Vec2(-cos(angleAutour)*6.0f,sin(angleAutour)*6.0f+1.0f);
	bd.position = b2Vec2(-5.f,3.0f);
	bd.bullet = true;
	m_ball = m_world->CreateBody(&bd);
	b2Vec2 cible = m_creature->getCOM();
	cible.y += (rand() % 10) / 3.0f;
	b2Vec2 velocity ((cible.x-bd.position.x)*2.0f,(cible.y-bd.position.y)*2.0f);
	m_ball->SetLinearVelocity(velocity);

	b2CircleShape circle;
	circle.m_radius = 0.2f;

	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 10.0f;
	fd.restitution = 0.0f;

	m_ball->CreateFixture(&fd);
}

void Application::LaunchObject() {
	if (m_object)	{
		m_world->DestroyBody(m_object);
		m_object = NULL;
	}

	b2BodyDef bd;
    bd.fixedRotation = false;
    bd.allowSleep = false;
    bd.awake = true;
    bd.type = b2_dynamicBody;
    bd.linearDamping = 0.01f;
    bd.angularDamping = 0.01f;
    b2CircleShape circle;
	circle.m_radius = 0.2f;

	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = 10.0f;
	fd.restitution = 0.0f;

    // Create object
    bd.position.Set(-2.0f, 2.22f);
    m_object = m_world->CreateBody(&bd);
    m_object->CreateFixture(&fd);
}

b2Body* Application::getBall() {
	if (m_ball)	{
		return m_ball;
	} else {
        return NULL;
	}
}

b2Body* Application::getObject() {
	if (m_object)	{
		return m_object;
	} else {
        return NULL;
	}
}

void Application::Step(Settings* settings) {
	float32 timeStep = 1.0f / 60.0f;
	/*if (settings->pause) {
		timeStep = 0.0f;
		g_debugDraw.DrawString(522, m_textLine, "****PAUSE****");
		m_textLine += DRAW_STRING_NEW_LINE;
	}*/

	g_debugDraw.SetFlags(b2Draw::e_shapeBit + settings->drawJoints * b2Draw::e_jointBit);
	m_pointCount = 0;

	// Tourner la plateforme
	if (settings->rotationPlateforme && !m_creature->hasFallen()) {
	    float32 currentPlatformAngle = m_plateform->GetAngle();
	    double time = glfwGetTime();
	    // Changement de rotation toutes les 3 secondes
	    if (time - m_lastChangeRotation > 3.0f) {
	        m_lastChangeRotation = time;
	        m_rotateClockwise = rand() % 2;
	        m_magnitudeToRotatePlatform += 0.0004;
	    }
	    // Application de la rotation
	    float32 newAngle = currentPlatformAngle + (m_rotateClockwise ? 1 : -1) * m_magnitudeToRotatePlatform;
	    if (abs(newAngle) < 0.25) m_plateform->SetTransform(b2Vec2(0.0f,1.0f),newAngle);
	}

	// Lancer la balle
	if (settings->launchBalls && !m_creature->hasFallen()) {
        double time = glfwGetTime();
	    // Lancement d'une balle toutes les 3 secondes
	    if (time - m_lastShoot > 3.0f) {
	        m_lastShoot = time;
	        LaunchBall();
	    }
	}

	// Placer objet
	if (settings->launchObject && !m_creature->hasFallen()) {
        double time = glfwGetTime();
	    // Ajout de l'objet
	    LaunchObject();
	}


	//if (!settings->pause) {
	    // Controle de la creature
	    m_creature->update(settings->balanceControl,settings->motionTracking, m_ball, m_object);

	    // Simule un pas de temps
    	m_world->Step(timeStep, 8, 3);
	//}

	// Visualiser le CdM
	if (settings->showCOM) {
        if (m_COM == NULL) {
            // on cree le CdM
            b2BodyDef bd;
            bd.position = m_creature->getCOM();
            bd.allowSleep = false;
            bd.awake = true;
            bd.active = false;
            m_COM = m_world->CreateBody(&bd);
            b2CircleShape circle;
            circle.m_radius = 0.1f;
            m_COM->CreateFixture(&circle, 0.0f);
        }
        else {
            m_COM->SetTransform(m_creature->getCOM(),0.0f);
        }
	}
	else {
	    if (m_COM) {
            // on detruit le CdM
            m_world->DestroyBody(m_COM);
			m_COM = NULL;
	    }
	}

	m_world->DrawDebugData();
    g_debugDraw.Flush();

   	// Affichage du temps d'equilibre
   	if (settings->balanceControl) {
        if (!m_creature->hasFallen()) {
                //if (settings->pause) m_startBalance = glfwGetTime();
                m_balancedDuration = ((int)((glfwGetTime() - m_startBalance) * 10.0f))/10.0f;
        }
   	    std::ostringstream osstmp;
   	    osstmp << "Duree en equilibre : " << m_balancedDuration << " secondes";
   	    g_debugDraw.DrawString(5,30,osstmp.str().c_str());
   	}

	if (timeStep > 0.0f) ++m_stepCount;

	// Track maximum profile times
	{
		const b2Profile& p = m_world->GetProfile();
		m_maxProfile.step = b2Max(m_maxProfile.step, p.step);
		m_maxProfile.collide = b2Max(m_maxProfile.collide, p.collide);
		m_maxProfile.solve = b2Max(m_maxProfile.solve, p.solve);
		m_maxProfile.solveInit = b2Max(m_maxProfile.solveInit, p.solveInit);
		m_maxProfile.solveVelocity = b2Max(m_maxProfile.solveVelocity, p.solveVelocity);
		m_maxProfile.solvePosition = b2Max(m_maxProfile.solvePosition, p.solvePosition);
		m_maxProfile.solveTOI = b2Max(m_maxProfile.solveTOI, p.solveTOI);
		m_maxProfile.broadphase = b2Max(m_maxProfile.broadphase, p.broadphase);

		m_totalProfile.step += p.step;
		m_totalProfile.collide += p.collide;
		m_totalProfile.solve += p.solve;
		m_totalProfile.solveInit += p.solveInit;
		m_totalProfile.solveVelocity += p.solveVelocity;
		m_totalProfile.solvePosition += p.solvePosition;
		m_totalProfile.solveTOI += p.solveTOI;
		m_totalProfile.broadphase += p.broadphase;
	}

	if (m_mouseJoint) {
		b2Vec2 p1 = m_mouseJoint->GetAnchorB();
		b2Vec2 p2 = m_mouseJoint->GetTarget();

		b2Color c;
		c.Set(0.0f, 1.0f, 0.0f);
		g_debugDraw.DrawPoint(p1, 4.0f, c);
		g_debugDraw.DrawPoint(p2, 4.0f, c);

		c.Set(0.8f, 0.8f, 0.8f);
		g_debugDraw.DrawSegment(p1, p2, c);
	}

	if (settings->drawContactPoints) {
		const float32 k_axisScale = 0.3f;
		for (int32 i = 0; i < m_pointCount; ++i) {
			ContactPoint* point = m_points + i;
			if (point->state == b2_addState) {
				// Add
				g_debugDraw.DrawPoint(point->position, 10.0f, b2Color(0.3f, 0.95f, 0.3f));
			}
			else if (point->state == b2_persistState) {
				// Persist
				g_debugDraw.DrawPoint(point->position, 5.0f, b2Color(0.3f, 0.3f, 0.95f));
			}

			if (settings->drawContactNormals == 1) {
				b2Vec2 p1 = point->position;
				b2Vec2 p2 = p1 + k_axisScale * point->normal;
				g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
			}
		}
	}
}

void Application::ShiftOrigin(const b2Vec2& newOrigin) {
	m_world->ShiftOrigin(newOrigin);
}
