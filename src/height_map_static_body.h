#ifndef HEIGHT_MAP_STATIC_BODY_H
#define HEIGHT_MAP_STATIC_BODY_H

#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <CollisionShape.hpp>
#include <ConcavePolygonShape.hpp>
#include <ResourceLoader.hpp>
#include "height_map_mesh.h"

namespace godot {

class HeightMapStaticBody : public StaticBody {
    GODOT_CLASS(HeightMapStaticBody, StaticBody)

private:
    String heightmap_path;
    float max_height;

    MeshInstance* mesh_instance;
    CollisionShape* collision_shape;
    bool ready = false;

public:
    static void _register_methods();

    HeightMapStaticBody();
    ~HeightMapStaticBody();

    void _init();
    void _ready();

    void set_heightmap_path(String path);
    String get_heightmap_path() const;

    void set_max_height(float height);
    float get_max_height() const;

    void generate_heightmap_mesh();
};

}

#endif // HEIGHT_MAP_STATIC_BODY_H
