#include "godot_food.h"

#include <CapsuleMesh.hpp>
#include <CapsuleShape.hpp>
#include <ResourceLoader.hpp>

using namespace godot;

void GodotFood::_register_methods() {
    register_method("_ready", &GodotFood::_ready);
}

void GodotFood::_ready() {
	auto sceneInstance = scene->instance();
	add_child(sceneInstance);
}

GodotFood::GodotFood() {
}

GodotFood::~GodotFood() {
}

void GodotFood::_init() {
	Godot::print("Food init");
	scene = ResourceLoader::get_singleton()->load("res://stylized bush.fbx");
    if (scene.is_null()) {
        Godot::print("Failed to load scene.");
    }
}

void GodotFood::setFood(std::shared_ptr<Food> food) {
	this->food = food;
}