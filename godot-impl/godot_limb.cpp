#include "godot_limb.h"

#include <CapsuleMesh.hpp>
#include <CapsuleShape.hpp>

using namespace godot;

void GodotLimb::_register_methods() {
	register_method("physicsProcess", &GodotLimb::physicsProcess);
}

void GodotLimb::_ready() {
}

GodotLimb::GodotLimb() {
}

GodotLimb::~GodotLimb() {
}

void GodotLimb::_init() {
}

void GodotLimb::setLimb(Limb *limb) {
	this->limb = limb;
	Ref<CapsuleMesh> capsule_mesh;
    capsule_mesh.instance();
    capsule_mesh->set_radius(this->limb->getWidth());
    capsule_mesh->set_mid_height(this->limb->getLength());
    mesh = capsule_mesh;
	meshInstance = MeshInstance::_new();
	meshInstance->set_mesh(mesh);
	meshInstance->set_rotation(Vector3(M_PI/2, 0, 0));
	meshInstance->set_translation(Vector3(0, -this->limb->getLength()/2, 0));
	body = ManagedRigidBody::_new();
	add_child(body);
	body->add_child(meshInstance);
	body->set_mass(5);
	body->set_gravity_scale(1);
	// body->setPhysicsProcessFunction([this](float delta){this->physicsProcess(delta);});
	auto capsuleShape = CapsuleShape::_new();
	capsuleShape->set_radius(this->limb->getWidth());
	capsuleShape->set_height(this->limb->getLength());
	collisionShape = CollisionShape::_new();
	collisionShape->set_shape(capsuleShape);
	collisionShape->set_rotation(Vector3(M_PI/2, 0, 0));
	collisionShape->set_translation(Vector3(0, -this->limb->getLength()/2, 0));
	body->add_child(collisionShape);
	auto childLimb = this->limb->getChildLimb().get();
	if(childLimb != nullptr) {
		auto position = childLimb->getPosition();
		auto rotation = childLimb->getRotation();
		child = GodotLimb::_new();
		add_child(child);
		child->set_translation(Vector3(position[0], position[1], position[2]));
		child->setLimb(childLimb);
		Basis basis;
		basis.set_axis(0, Vector3(1, 0, 0));
		joint = Generic6DOFJoint::_new();
		joint->set_node_a(body->get_path());
		joint->set_node_b(child->body->get_path());
		joint->set_transform(Transform(basis, Vector3(position[0], position[1], position[2])));


        // Configure the joint angular limits
        joint->set_param_x(Generic6DOFJoint::PARAM_ANGULAR_LOWER_LIMIT, Math::deg2rad(90.0));
        joint->set_param_x(Generic6DOFJoint::PARAM_ANGULAR_LOWER_LIMIT, -Math::deg2rad(90.0));
		joint->set_exclude_nodes_from_collision(true);
		add_child(joint);
	}
}
	
void GodotLimb::physicsProcess(float delta) {
	Godot::print("limb physicsProcess");
}
