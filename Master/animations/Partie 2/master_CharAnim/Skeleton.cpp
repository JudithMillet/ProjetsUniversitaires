
#include "Skeleton.h"
#include "BVHAxis.h"

using namespace chara;

void Skeleton::init(const BVH& bvh)
{
    // TODO resize ?

    for(int i=0 ; i< bvh.getNumberOfJoint() ; i++) {
        SkeletonJoint sj;
        sj.m_parentId = bvh.getJoint(i).getParentId(); // TODO check if parent ? so -1
        sj.m_l2w = Transform();

        m_joints.push_back(sj);
    }
}


Point Skeleton::getJointPosition(int i) const
{
    // recupère colonne droite de la matrice (seulement la translation) --> possible en faisant M * Point(0,0,0) 
    return m_joints[i].m_l2w(Point(0,0,0));
}

Transform Skeleton::getJointTransform(int i) const
{
    return m_joints[i].m_l2w;
}


int Skeleton::getParentId(const int i) const
{
    return m_joints[i].m_parentId;
}


void Skeleton::setPose(const BVH& bvh, int frameNumber, const Transform &t)
{
    // TODO
    // Parcourir toutes les articulations (SkeletonJoint ou BVHJoint) 
    //     Declarer la matrice l2f (pere<-local)
    //     Init avec la translation Offset
    //     Parcourir tous les channels
    //          Accumuler dans la matrice l2f les translations et rotation en fonction du type de Channel
    // Multiplier la matrice l2f avec la matrice l2w (world<-local) du p�re qui est d�j� stock� dans le tableau 
    // Attention il peut ne pas y avoir de p�re (pour la racine)
    
    for(int i=0 ; i<bvh.getNumberOfJoint() ; i++) {
        float x, y, z;
        bvh.getJoint(i).getOffset(x, y, z);

        // Matrice de i
        Transform m_l2f = Translation(x, y, z);

        

        for (int j = 0; j < bvh.getJoint(i).getNumberOfChannel(); j++) {
            float data = bvh.getJoint(i).getChannel(j).getData(frameNumber%(bvh.getNumberOfFrame()-1)); // TODO modify modulo frameNumber%6

            // TODO add AXIS_W ? 
            if (bvh.getJoint(i).getChannel(j).isTranslation()) {

                switch (bvh.getJoint(i).getChannel(j).getAxis()) {
                    case AXIS_X :
                        m_l2f = m_l2f * Translation(data, 0, 0);
                        break;
                    case AXIS_Y :
                        m_l2f = m_l2f * Translation(0, data, 0);
                        break;
                    case AXIS_Z :
                        m_l2f = m_l2f * Translation(0, 0, data);
                        break;
                }
            } else if (bvh.getJoint(i).getChannel(j).isRotation()) {

                switch (bvh.getJoint(i).getChannel(j).getAxis()) {
                    case AXIS_X :
                        m_l2f = m_l2f * RotationX(data);
                        break;
                    case AXIS_Y :
                        m_l2f = m_l2f * RotationY(data);
                        break;
                    case AXIS_Z :
                        m_l2f = m_l2f * RotationZ(data);
                        break;
                }
            }
        }


        if(getParentId(i) >= 0) { 
            m_joints[i].m_l2w = m_joints[getParentId(i)].m_l2w * m_l2f;

        } else {
            m_joints[i].m_l2w = t * m_l2f;
        }
    }
}
