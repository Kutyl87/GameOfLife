#ifndef GODOT_ORGANISM_H
#define GODOT_ORGANISM_H

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
		GodotLimb *child = nullptr;
		Generic6DOFJoint *joint;
		Limb *limb;

	public:
		RigidBody *body;
		GodotLimb();
		~GodotLimb();

		void _init();

		void setLimb(Limb* limb);

		void _ready();

		void _physics_process(float delta);

		static void _register_methods();
	};
}

#endif // GODOT_ORGANISM_H