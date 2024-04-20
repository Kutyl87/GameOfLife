#include "main.h"
#include <SceneTree.hpp>
#include <Godot.hpp>

using namespace godot;

void Main::_register_methods() {
	Godot::print("Main test point 1");
	register_method("_ready", &Main::_ready);
	register_method("_process", &Main::_process);
}

void Main::_ready() {
	_input = godot::Input::get_singleton();
}

void Main::_process() {
}