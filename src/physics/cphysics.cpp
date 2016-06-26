/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <btBulletDynamicsCommon.h>
#include "cphysics.h"
#include "../entities/cmesh.h"
#include "../entities/cworld.h"

using namespace Citrine;

Citrine::PhysicsWorld::PhysicsWorld(World* parent)
{
	this->parent = parent;
	//Initialize Bullet components
	broadphase = new btDbvtBroadphase;
	collision_configuration = new btDefaultCollisionConfiguration;
	collision_dispatcher = new btCollisionDispatcher(collision_configuration);
	solver = new btSequentialImpulseConstraintSolver;
	//Initialize world
	world = new btDiscreteDynamicsWorld(collision_dispatcher, broadphase, solver, collision_configuration);

	//Set default gravity
	world->setGravity(btVector3(0.0f, -9.806f, 0.0f));
}

Citrine::PhysicsWorld::~PhysicsWorld()
{
	delete collision_dispatcher;
	delete broadphase;
	delete world;
}

void Citrine::PhysicsWorld::Step(unsigned int elapsed_microseconds)
{
	float step_size = 0.001;
	float seconds_elapsed = elapsed_microseconds / 1000.0f;
	unsigned int amount_of_steps = static_cast<int> (seconds_elapsed / step_size);

	//Step the physics
	for (unsigned int i = 0; i < amount_of_steps; i++)
		world->stepSimulation(step_size, 10);

	//Update mesh transformation matrices
	for (std::list<SmartPointer<Mesh> >::iterator i = parent->GetMeshes()->begin(); i != parent->GetMeshes()->end(); i++)
	{
		PhysicsObject* phys_object = (*i)->GetPhysics();
		if (phys_object)
			phys_object->Update();
	}
}

btDiscreteDynamicsWorld* Citrine::PhysicsWorld::GetWorld()
{
	return world;
}

World* Citrine::PhysicsWorld::GetParent()
{
	return parent;
}

void Citrine::PhysicsWorld::SetGravity(float x, float y, float z)
{
	world->setGravity(btVector3(x, y, z));
}

Citrine::PhysicsObject::PhysicsObject(Mesh* parent, PhysicsWorld* world)
{
	this->parent = parent;
	this->world = world;

	//Get mesh position and size
	Vector3 position = parent->GetTransformation()->GetPosition();
	Vector3 size = GetMeshSizeBox();

	//Create box shape
	shape = new btBoxShape(btVector3(size.x, size.y, size.z));
	//Create motion state
	motion_state = new btDefaultMotionState;

	//Calculate mass and inertia
	btScalar mass = 1.0f;
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	shape->calculateLocalInertia(mass, inertia);
	//Create rigid body construction info
	btRigidBody::btRigidBodyConstructionInfo construction_info(mass, motion_state, shape, inertia);
	//Create rigid body
	body = new btRigidBody(construction_info);

	//Add rigid body to world
	world->GetWorld()->addRigidBody(body);
}

Citrine::PhysicsObject::~PhysicsObject()
{
	delete shape;
	delete motion_state;
	delete body;
}

void Citrine::PhysicsObject::Update()
{
	//Get OpenGL matrix
	btTransform transform;
	motion_state->getWorldTransform(transform);
	btScalar matrix_array[16];
	transform.getOpenGLMatrix(matrix_array);

	//Update mesh rotation
	Matrix3x3 rotation_matrix;
	rotation_matrix.x = Vector3(matrix_array[0], matrix_array[1], matrix_array[2]);
	rotation_matrix.y = Vector3(matrix_array[4], matrix_array[5], matrix_array[6]);
	rotation_matrix.z = Vector3(matrix_array[8], matrix_array[9], matrix_array[10]);
	parent->GetTransformation()->SetRotationMatrix(&rotation_matrix);
	//Update mesh translation
	parent->GetTransformation()->SetPosition(matrix_array[12], matrix_array[13], matrix_array[14]);

}

void Citrine::PhysicsObject::SetGravity(float x, float y, float z)
{
	body->setGravity(btVector3(x, y, z));
}

void Citrine::PhysicsObject::AddForce(float x, float y, float z)
{
	body->applyForce(btVector3(x, y, z), btVector3(0.0f, 0.0f, 0.0f));
}

void Citrine::PhysicsObject::AddTorque(float x, float y, float z)
{
	body->applyTorque(btVector3(x, y, z));
}

Vector3 Citrine::PhysicsObject::GetMeshSizeBox()
{
	Vector3 out(0.0f, 0.0f, 0.0f);

	std::pair<Vertex*, unsigned short> vertices = parent->GetVertices();
	for (unsigned int i = 0; i < vertices.second; i++)
	{
		if (vertices.first[i].position.x > out.x)
			out.x = vertices.first[i].position.x;
		if (vertices.first[i].position.y > out.y)
			out.y = vertices.first[i].position.y;
		if (vertices.first[i].position.z > out.z)
			out.z = vertices.first[i].position.z;
	}

	return out;
}
