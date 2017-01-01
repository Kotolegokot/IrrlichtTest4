/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

struct IBodyFactory {
    IBodyFactory() = default;

    virtual std::unique_ptr<btRigidBody> produce(const btVector3 &pos)
    {
        btTransform abs_trans = rel_trans;
        abs_trans.getOrigin() += pos;

        auto motion_state = std::make_unique<btDefaultMotionState>(btTransform::getIdentity());
        auto shape = create_shape();
        btScalar _mass = mass();

        btVector3 inertia(0, 0, 0);
        if (_mass)
            shape->calculateLocalInertia(_mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(_mass, motion_state.release(),
                                                               shape.release(), inertia);

        auto rigid_body = std::make_unique<btRigidBody>(rigid_body_ci);
        rigid_body->setCenterOfMassTransform(abs_trans);
        rigid_body->setUserIndex(0);
        finishing_touch(*rigid_body);

        return rigid_body;
    }

    btTransform rel_trans = btTransform::getIdentity();

protected:
    virtual std::unique_ptr<btCollisionShape> create_shape() = 0;
    virtual btScalar mass() = 0;
    virtual void finishing_touch(btRigidBody &) = 0;
};