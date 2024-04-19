#ifndef BALL_H
#define BALL_H

#include <Godot.hpp>
#include <RigidBody.hpp>
#include <CollisionShape.hpp>
#include <Input.hpp>


namespace godot {
    class Ball : public RigidBody {
        GODOT_CLASS(Ball, RigidBody)

        godot::Input* _input;
        godot::CollisionShape* _collision_shape; 

    public:
        real_t ball_speed = 50;
        real_t fall_acceleration = 75;


        Ball();
        ~Ball();

        void _init();

        void _ready();

        void _physics_process(float delta);

        static void _register_methods();
    };
}

#endif