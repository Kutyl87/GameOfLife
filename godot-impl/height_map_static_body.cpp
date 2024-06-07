#include "height_map_static_body.h"
#include <godot_organism.h>
#include <organism.h>

using namespace godot;

const float defaultMaxHeight = 10.0;
const float defaultMapSize = 100.0;

void HeightMapStaticBody::_register_methods() {
	register_property<HeightMapStaticBody, String>("heightmapPath", &HeightMapStaticBody::setHeightmapPath,
												   &HeightMapStaticBody::getHeightmapPath, String(""));
	register_property<HeightMapStaticBody, float>("maxHeight", &HeightMapStaticBody::setMaxHeight,
												  &HeightMapStaticBody::getMaxHeight, defaultMaxHeight);
	register_property<HeightMapStaticBody, float>("mapSize", &HeightMapStaticBody::setMapSize,
												  &HeightMapStaticBody::getMapSize, defaultMapSize);

	register_method("_ready", &HeightMapStaticBody::_ready);
}

HeightMapStaticBody::HeightMapStaticBody() {}

HeightMapStaticBody::~HeightMapStaticBody() {}

void HeightMapStaticBody::_init() {
	heightmapPath = "";
	maxHeight = defaultMaxHeight;
	mapSize = defaultMapSize;

	physicsMaterial.instance();
	physicsMaterial->set_friction(10.0);
	set_physics_material_override(physicsMaterial);
	// manager = std::make_unique<SimulationManager>();
}

void HeightMapStaticBody::_ready() {
	meshInstance = MeshInstance::_new();
	add_child(meshInstance);

	collisionShape = CollisionShape::_new();
	add_child(collisionShape);
	ready = true;
	generateHeightmapMesh();

	std::vector<std::unique_ptr<Limb>> limbs;
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{-1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{-1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));

	Organism* test_organism =
		new Organism(std::array<float, 3>{500, 20, 500}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
					 std::move(limbs), std::vector<std::unique_ptr<Organ>>{});
	GodotOrganism* godot_organism = GodotOrganism::_new();
	add_child(godot_organism);
	godot_organism->setOrganism(test_organism);
	godot_organism->set_translation(Vector3(500, 5, 500));
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
	if (!ready)
		return;
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
