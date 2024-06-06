#ifndef GODOT_LIMB_H
#define GODOT_LIMB_H

#include <organism.h>
#include "godot_limb.h"

#include <Godot.hpp>
#include <Spatial.hpp>
#include <CollisionShape.hpp>
#include <MeshInstance.hpp>
#include <Generic6DOFJoint.hpp>
#include <RigidBody.hpp>
#include <vector>


namespace godot {
	class GodotOrganism : public Spatial {
		GODOT_CLASS(GodotOrganism, Spatial)

		MeshInstance *meshInstance;
		Ref<Mesh> mesh;
		CollisionShape *collisionShape;
		std::vector<std::pair<GodotLimb*, Generic6DOFJoint*>> limbs;
		Organism* organism;
		RigidBody* body;

	public:
		GodotOrganism();
		~GodotOrganism();

		void _init();

		void setOrganism(Organism *organism);

		void _ready();

		void _physics_process(float delta);

		static void _register_methods();
	};
}

#endif // GODOT_LIMB_H