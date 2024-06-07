#ifndef MANAGED_RIGID_BODY_H
#define MANAGED_RIGID_BODY_H

#include <Godot.hpp>
#include <RigidBody.hpp>
#include <vector>
#include <functional>
#include <PhysicsMaterial.hpp>


namespace godot {
	class ManagedRigidBody : public RigidBody {
		GODOT_CLASS(ManagedRigidBody, RigidBody)

	public:
		ManagedRigidBody();
		~ManagedRigidBody();

		void _init();

		void _ready();

		void setPhysicsProcessFunction(std::function<void(float)> func);

		void _physics_process(float delta);

		static void _register_methods();
	private:
		std::function<void(float)> physicsProcess;
		Ref<PhysicsMaterial> physicsMaterial;
	};
}

#endif // MANAGED_RIGID_BODY_H