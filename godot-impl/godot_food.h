#ifndef GODOT_FOOD_H
#define GODOT_FOOD_H

#include <food.h>

#include <Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <CollisionShape.hpp>
#include <PackedScene.hpp>
#include <vector>


namespace godot {
	class GodotFood : public StaticBody {
		GODOT_CLASS(GodotFood, StaticBody)

		Ref<PackedScene> scene;

		CollisionShape *collisionShape;

	public:
		GodotFood();
		~GodotFood();

		void _init();

		void setFood(std::shared_ptr<Food> food);

		void _ready();

		static void _register_methods();
		std::shared_ptr<Food> food;
	};
}

#endif // GODOT_FOOD_H