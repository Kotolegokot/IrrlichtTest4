#ifndef TETRAHEDRON_PRODUCER
#define TETRAHEDRON_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPointCloudShape.h>
#include <irrlicht.h>
#include "IBodyProducer.h"
#include "ObjMesh.h"
#include "options.h"
#include "util/Vector3.h"
#include "util.h"

using namespace irr;

#define TETRAHEDRON_MODEL "media/models/tetrahedron.obj"

class TetrahedronProducer : public IBodyProducer {
public:
    TetrahedronProducer(btScalar edge) :
        m_edge(edge) {}

    btScalar getMass() const override
    {
        static const btScalar K = 1.0f / 6.0f / std::sqrt(2);

        return m_edge * m_edge * m_edge * K * MASS_COEFFICIENT;
    }

    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                                  const btTransform &absoluteTransform) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(TETRAHEDRON_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                           addMeshSceneNode(mesh.release()));
        node->setPosition(bullet2irrlicht(absoluteTransform.getOrigin()));
        node->setRotation(quatToEuler(absoluteTransform.getRotation()));
        node->setScale({ m_edge, m_edge, m_edge });
        node->setMaterialTexture(0, irrlichtDevice.getVideoDriver()->getTexture("media/textures/tetrahedron.png"));
        node->setVisible(TEXTURES_ENABLED);
#if FOG_ENABLED
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
#endif // FOG_ENABLED
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);

        return node;
    }

    std::unique_ptr<btCollisionShape> createShape() const override
    {
        return std::make_unique<btConvexPointCloudShape>(objMesh.getPoints(), objMesh.getPointsCount(),
                                                         btVector3(1, 1, 1) * m_edge);
    }

private:
    const btScalar m_edge;

    static ObjMesh &objMesh;
};

#endif // TETRAHEDRON_PRODUCER
