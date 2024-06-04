#include "height_map_static_body.h"

using namespace godot;

const float defaultMaxHeight = 10.0;
const float defaultMapSize = 100.0;

void HeightMapStaticBody::_register_methods() {
	register_property<HeightMapStaticBody, String>("heightmapPath", &HeightMapStaticBody::setHeightmapPath, &HeightMapStaticBody::getHeightmapPath, String(""));
	register_property<HeightMapStaticBody, float>("maxHeight", &HeightMapStaticBody::setMaxHeight, &HeightMapStaticBody::getMaxHeight, defaultMaxHeight);
	register_property<HeightMapStaticBody, float>("mapSize", &HeightMapStaticBody::setMapSize, &HeightMapStaticBody::getMapSize, defaultMapSize);

	register_method("_ready", &HeightMapStaticBody::_ready);
}

HeightMapStaticBody::HeightMapStaticBody() {}
HeightMapStaticBody::~HeightMapStaticBody() {}

void HeightMapStaticBody::_init() {
	heightmapPath = "";
	maxHeight = defaultMaxHeight;
	mapSize = defaultMapSize;
}

void HeightMapStaticBody::_ready() {
	meshInstance = MeshInstance::_new();
	add_child(meshInstance);

	collisionShape = CollisionShape::_new();
	add_child(collisionShape);
	ready = true;
	generateHeightmapMesh();
}

void HeightMapStaticBody::setHeightmapPath(String path) {
	heightmapPath = path;
	generateHeightmapMesh();
}

String HeightMapStaticBody::getHeightmapPath() const {
	return heightmapPath;
}

void HeightMapStaticBody::setMaxHeight(float height) {
	maxHeight = height;
	generateHeightmapMesh();
}

float HeightMapStaticBody::getMaxHeight() const {
	return maxHeight;
}

void HeightMapStaticBody::setMapSize(float size) {
	mapSize = size;
	generateHeightmapMesh();
}

float HeightMapStaticBody::getMapSize() const {
	return mapSize;
}

void HeightMapStaticBody::generateHeightmapMesh() {
	if (!ready) return;
	if (heightmapPath.empty()) {
		return;
	}

	Ref<HeightMapMesh> heightMapMesh;
	heightMapMesh.instance();
	heightMapMesh->setHeightmapPath(heightmapPath);
	if (heightMapMesh->generateMeshFromHeightmap(maxHeight, mapSize)) {
		meshInstance->set_mesh(heightMapMesh);

		Ref<ConcavePolygonShape> shape;
		shape.instance();
		Array surfaceArrays = heightMapMesh->surface_get_arrays(0);
		PoolVector3Array vertices = surfaceArrays[Mesh::ARRAY_VERTEX];
		PoolIntArray indices = surfaceArrays[Mesh::ARRAY_INDEX];

		PoolVector3Array faces;
		for (int i = 0; i < indices.size(); i += 3) {
			faces.append(vertices[indices[i]]);
			faces.append(vertices[indices[i + 1]]);
			faces.append(vertices[indices[i + 2]]);
		}

		shape->set_faces(faces);
		collisionShape->set_shape(shape);
		Godot::print(shape->get_faces().size());
		collisionShape->set_shape(shape);
		Godot::print(collisionShape->get_shape()->get_debug_mesh()->get_aabb());
	}
}
