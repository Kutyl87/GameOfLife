#ifndef GODOT_LIMB_H
#define GODOT_LIMB_H

#include "managed_rigid_body.h"
#include <limb.h>

#include <Godot.hpp>
#include <RigidBody.hpp>
#include <CollisionShape.hpp>
#include <MeshInstance.hpp>
#include <Generic6DOFJoint.hpp>
#include <RigidBody.hpp>
#include <vector>


namespace godot {
	class GodotLimb : public Spatial {
		GODOT_CLASS(GodotLimb, Spatial)

		MeshInstance *meshInstance;
		Ref<Mesh> mesh;
		CollisionShape *collisionShape;
		Generic6DOFJoint *joint;
		Limb *limb;

	public:
		GodotLimb *child = nullptr;
		ManagedRigidBody *body;
		GodotLimb();
		~GodotLimb();

		void _init();

		void setLimb(Limb* limb);

		void _ready();

		void physicsProcess(float delta);

		static void _register_methods();
	};
}

#endif // GODOT_LIMB_H