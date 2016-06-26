/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CPHYSICS_H
#define CPHYSICS_H

#include "../math/cvector.h"

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
class btDefaultMotionState;
class btRigidBody;

namespace Citrine
{
	class World;
	class Mesh;

	class PhysicsWorld
	{
		public:
			PhysicsWorld(World* parent);
			~PhysicsWorld();
			void Step(unsigned int elapsed_microseconds);
			btDiscreteDynamicsWorld* GetWorld();
			World* GetParent();
			void SetGravity(float x, float y, float z);
		private:
			btBroadphaseInterface* broadphase;
			btDefaultCollisionConfiguration* collision_configuration;
			btCollisionDispatcher* collision_dispatcher;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* world;
			World* parent;
	};

	class PhysicsObject
	{
		public:
			PhysicsObject(Mesh* parent, PhysicsWorld* world);
			~PhysicsObject();
			void Update();
			void SetGravity(float x, float y, float z);
			void AddForce(float x, float y, float z);
			void AddTorque(float x, float y, float z);
		private:
			btCollisionShape* shape;
			btDefaultMotionState* motion_state;
			btRigidBody* body;
			Mesh* parent;
			PhysicsWorld* world;

			Vector3 GetMeshSizeBox();
	};
}
#endif
