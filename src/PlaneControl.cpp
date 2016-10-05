#include "PlaneControl.h"

PlaneControl::PlaneControl(Plane *plane, const Controls &controls) :
    plane(plane), controls(controls) {}

// move the plane according to keys pressed
void PlaneControl::handle(EventReceiver *eventReceiver)
{
    btTransform transform;
    plane->getRigidBody()->getMotionState()->getWorldTransform(transform);
    btQuaternion rotation = transform.getRotation();
    btVector3 axis = rotation.getAxis();
    btScalar angle = rotation.getAngle();

    btVector3 turnImpulse(0, 0, 0);
    btVector3 rotateImpulse(0, 0, 0);

    // up and down
    if (!(eventReceiver->IsKeyDown(controls.up) &&
        eventReceiver->IsKeyDown(controls.down))) { // if not both are pressed
        // up
        if (eventReceiver->IsKeyDown(controls.up)) {
            turnImpulse = btVector3(0, 1, 0);

        }
        // down
        else if (eventReceiver->IsKeyDown(controls.down)) {
            turnImpulse = btVector3(0, -1, 0);
        }
    }

    // left and right
    if (!(eventReceiver->IsKeyDown(controls.left) &&
        eventReceiver->IsKeyDown(controls.right))) { // if not both are pressed
        // left
        if (eventReceiver->IsKeyDown(controls.left)) {
            turnImpulse = btVector3(-1, 0, 0);
        }
        // right
        else if (eventReceiver->IsKeyDown(controls.right)) {
            turnImpulse = btVector3(1, 0, 0);
        }
    }

    // rotation
    if (!(eventReceiver->IsKeyDown(KEY_KEY_Q) &&
        eventReceiver->IsKeyDown(KEY_KEY_E))) { // if not both are pressed
        if (eventReceiver->IsKeyDown(KEY_KEY_Q)) {
            rotateImpulse = btVector3(0, 0, 1);
        }
        else if (eventReceiver->IsKeyDown(KEY_KEY_E)) {
            rotateImpulse = btVector3(0, 0, -1);
        }
    }

    // air resistance simulation
    btVector3 planeVelosity = -plane->getRigidBody()->getLinearVelocity();
    btScalar planeVelocityLength = planeVelosity.length();
    planeVelosity.safeNormalize();
    planeVelosity *= 0.001f*planeVelocityLength*planeVelocityLength;
    plane->getRigidBody()->applyForce(planeVelosity, btVector3(0, 0, 0));

   //turnImpulse = turnImpulse.rotate(axis, angle);
    turnImpulse *= 25;
    rotateImpulse *= 100;
    plane->getRigidBody()->applyCentralImpulse(turnImpulse);
    plane->getRigidBody()->applyTorqueImpulse(rotateImpulse);
    plane->getRigidBody()->applyForce(btVector3(0, 0, 600), btVector3(0, 0, 0));
}

void PlaneControl::setPlane(Plane *plane)
{
    this->plane = plane;
}

Plane *PlaneControl::getPlane()
{
    return plane;
}
