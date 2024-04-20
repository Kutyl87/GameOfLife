#include <sstream>
#include "ball.h"

using namespace godot;

void Ball::_register_methods() {
	register_method("_physics_process", &Ball::_physics_process);
	godot::register_property("ball_speed", &Ball::ball_speed, (real_t)50.0);
}

void Ball::_ready() {
}

Ball::Ball() {
}

Ball::~Ball() {
}

void Ball::_init() {
}

	
void Ball::_physics_process(float delta) {
	godot::Transform ball_transform = get_transform();

	godot::Vector3 ball_velocity = ball_transform.basis.z * ball_speed;

	std::stringstream stream;
	stream << ball_transform.get_origin().y;

	Godot::print(stream.str().c_str());

	if(ball_transform.get_origin().y < 1.501) {
		apply_impulse(ball_transform.get_origin(), Vector3(0, 5, 0));
	}
}