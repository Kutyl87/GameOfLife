#ifndef MAIN_H
#define MAIN_H

#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <Input.hpp>

#include "ball.h"

namespace godot {
	class Main : public godot::Node {
		GODOT_CLASS(Main, godot::Node)

		godot::Input* _input;

	public:
		godot::Ref<godot::PackedScene> projectile_scene;

		void _init() {}
		void _ready();
		void _process();
		static void _register_methods();
	};
}

#endif