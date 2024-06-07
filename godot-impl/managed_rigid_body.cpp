#include "managed_rigid_body.h"

using namespace godot;

void ManagedRigidBody::_register_methods() {
	register_method("_physics_process", &ManagedRigidBody::_physics_process);
}

void ManagedRigidBody::_ready() {
}

ManagedRigidBody::ManagedRigidBody() {
}

ManagedRigidBody::~ManagedRigidBody() {
}

void ManagedRigidBody::_init() {
	physicsMaterial.instance();
	physicsMaterial->set_friction(1.0);
	set_physics_material_override(physicsMaterial);
}

void ManagedRigidBody::setPhysicsProcessFunction(std::function<void (float)> func) {
	physicsProcess = func;
}
	
void ManagedRigidBody::_physics_process(float delta) {
	if(physicsProcess) {
		physicsProcess(delta);
	}
}
