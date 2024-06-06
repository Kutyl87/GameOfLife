#include "godot_organism.h"

#include <CapsuleMesh.hpp>
#include <CapsuleShape.hpp>

using namespace godot;

const float bodyLength = 2.0;
const float bodyRadius = 1.0;

void GodotOrganism::_register_methods() {
}

void GodotOrganism::_ready() {
}

GodotOrganism::GodotOrganism() {
}

GodotOrganism::~GodotOrganism() {
}

void GodotOrganism::_init() {
	Ref<CapsuleMesh> capsuleMesh;
    capsuleMesh.instance();
    capsuleMesh->set_radius(bodyRadius);
    capsuleMesh->set_mid_height(bodyLength);
    mesh = capsuleMesh;
	meshInstance = MeshInstance::_new();
	meshInstance->set_mesh(mesh);
	body = RigidBody::_new();
	add_child(body);

	body->add_child(meshInstance);
	body->set_mass(50);
	body->set_gravity_scale(1);
	auto capsuleShape = CapsuleShape::_new();
	capsuleShape->set_radius(bodyRadius);
	capsuleShape->set_height(bodyLength);
	collisionShape = CollisionShape::_new();
	collisionShape->set_shape(capsuleShape);
	body->add_child(collisionShape);
}

void GodotOrganism::setOrganism(Organism *organism) {
	this->organism = organism;
	auto& coreLimbs = this->organism->getChildren();
	for(auto& limb : limbs) {
		remove_child(limb.first);
		remove_child(limb.second);
		limb.first->queue_free();
		limb.second->queue_free();
	}
	limbs.resize(0);
	for(auto& coreLimb : coreLimbs) {
		auto limb = std::make_pair(GodotLimb::_new(), Generic6DOFJoint::_new());
		add_child(limb.first);
		limb.first->setLimb(coreLimb.get());
		auto position = coreLimb->getPosition();
		auto rotation = coreLimb->getRotation();
		limb.first->set_translation(Vector3(position[0], position[1], position[2]));
		limb.first->set_rotation(Vector3(rotation[0], 0, 0));
		Basis basis;
		basis.set_axis(0, Vector3(1, 0, 0));
		limb.second->set_node_a(body->get_path());
		limb.second->set_node_b(limb.first->body->get_path());
		limb.second->set_transform(Transform(basis, Vector3(position[0], position[1], position[2])));


        // Configure the limb.second angular limits
        limb.second->set_param_x(Generic6DOFJoint::PARAM_ANGULAR_LOWER_LIMIT, Math::deg2rad(90.0));
        limb.second->set_param_x(Generic6DOFJoint::PARAM_ANGULAR_LOWER_LIMIT, -Math::deg2rad(90.0));
		limb.second->set_exclude_nodes_from_collision(true);
		limbs.push_back(limb);
		add_child(limb.second);
	}
}
	
void GodotOrganism::_physics_process(float delta) {
}
