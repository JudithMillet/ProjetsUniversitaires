
#ifndef _CHARANIMVIEWER_H
#define _CHARANIMVIEWER_H


#include "CharacterController.h"
#include "quaternion.h"
#include "Viewer.h"
#include "BVH.h"
#include "Skeleton.h"
#include "TransformQ.h"

#include <PhysicalWorld.h>

class CharAnimViewer : public Viewer
{
public:
    CharAnimViewer();

    int init();
    int render();
    int update( const float time, const float delta );

	static CharAnimViewer& singleton() { return *psingleton;  }

protected:
	void bvhDrawRec(const chara::BVHJoint& bvh, const Transform& f2w, int f);
    
    chara::BVH m_bvh;
    chara::BVH m_bvh_walk;
    chara::BVH m_bvh_run;
    chara::BVH m_bvh_kick;
    int m_frameNumber;

    int bvh;

    Skeleton m_ske;

    PhysicalWorld m_world;

	void draw_skeleton(const Skeleton& );

private:
	static CharAnimViewer* psingleton;

    CharacterController controller;
};



#endif
