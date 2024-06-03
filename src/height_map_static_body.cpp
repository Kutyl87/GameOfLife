#include "height_map_static_body.h"

using namespace godot;

void HeightMapStaticBody::_register_methods() {
	register_property<HeightMapStaticBody, String>("heightmap_path", &HeightMapStaticBody::set_heightmap_path, &HeightMapStaticBody::get_heightmap_path, String(""));
	register_property<HeightMapStaticBody, float>("max_height", &HeightMapStaticBody::set_max_height, &HeightMapStaticBody::get_max_height, 10.0);
	register_property<HeightMapStaticBody, float>("mapSize", &HeightMapStaticBody::setMapSize, &HeightMapStaticBody::getMapSize, 10.0);

	register_method("_ready", &HeightMapStaticBody::_ready);
}

HeightMapStaticBody::HeightMapStaticBody() {}
HeightMapStaticBody::~HeightMapStaticBody() {}

void HeightMapStaticBody::_init() {
	heightmap_path = "";
	max_height = 10.0;
	mapSize = 100.0;
	Godot::print("init.");
}

void HeightMapStaticBody::_ready() {
	Godot::print("ready.");
	mesh_instance = MeshInstance::_new();
	add_child(mesh_instance);

	collision_shape = CollisionShape::_new();
	add_child(collision_shape);
	ready = true;
	generate_heightmap_mesh();
}

void HeightMapStaticBody::set_heightmap_path(String path) {
	heightmap_path = path;
	generate_heightmap_mesh();
}

String HeightMapStaticBody::get_heightmap_path() const {
	return heightmap_path;
}

void HeightMapStaticBody::set_max_height(float height) {
	max_height = height;
	generate_heightmap_mesh();
}

float HeightMapStaticBody::get_max_height() const {
	return max_height;
}

void HeightMapStaticBody::setMapSize(float size) {
	mapSize = size;
	generate_heightmap_mesh();
}

float HeightMapStaticBody::getMapSize() const {
	return mapSize;
}

void HeightMapStaticBody::generate_heightmap_mesh() {
	if(!ready) return;
	if (heightmap_path.empty()) {
		Godot::print("Heightmap path is empty.");
		return;
	}

	Ref<HeightMapMesh> height_map_mesh;
	height_map_mesh.instance();
	height_map_mesh->set_heightmap_path(heightmap_path);
	if(height_map_mesh->generate_mesh_from_heightmap(max_height, mapSize)) {
		mesh_instance->set_mesh(height_map_mesh);

		Ref<ConcavePolygonShape> shape;
		shape.instance();Array surface_arrays = height_map_mesh->surface_get_arrays(0);
		PoolVector3Array vertices = surface_arrays[Mesh::ARRAY_VERTEX];
		PoolIntArray indices = surface_arrays[Mesh::ARRAY_INDEX];

		Godot::print("Vertices count: " + String::num_int64(vertices.size()));
		Godot::print("Indices count: " + String::num_int64(indices.size()));

		// Convert indices to faces
		PoolVector3Array faces;
		for (int i = 0; i < indices.size(); i += 3) {
			faces.append(vertices[indices[i]]);
			faces.append(vertices[indices[i + 1]]);
			faces.append(vertices[indices[i + 2]]);
		}

		shape->set_faces(faces);
		collision_shape->set_shape(shape);
		Godot::print(shape->get_faces().size());
		collision_shape->set_shape(shape);
		Godot::print(collision_shape->get_shape()->get_debug_mesh()->get_aabb());
	}
}
