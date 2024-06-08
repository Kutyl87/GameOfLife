#include "height_map_static_body.h"
#include <godot_organism.h>
#include <organism.h>
#include <typeinfo>

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
}

void HeightMapStaticBody::_ready() {
	meshInstance = MeshInstance::_new();
	add_child(meshInstance);

	collisionShape = CollisionShape::_new();
	add_child(collisionShape);
	ready = true;
	generateHeightmapMesh();
	manager = std::make_unique<SimulationManager>([this](std::shared_ptr<::Object> organism){spawn(organism);}, [this](std::shared_ptr<::Object> organism){despawn(organism);});
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

	heightMapMesh.instance();
	heightMapMesh->setHeightmapPath(heightmapPath);
	if (heightMapMesh->generateMeshFromHeightmap(maxHeight, mapSize)) {
		meshInstance->set_mesh(heightMapMesh);

		Ref<ConcavePolygonShape> shape;
		shape.instance();
		Array surfaceArrays = heightMapMesh->surface_get_arrays(0);
		vertices = surfaceArrays[Mesh::ARRAY_VERTEX];
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

void HeightMapStaticBody::spawn(std::shared_ptr<::Object> object) {
	auto organism = std::dynamic_pointer_cast<Organism>(object);
	if(organism) {
		GodotOrganism* godot_organism = GodotOrganism::_new();
		organisms.push_back(godot_organism);
		add_child(godot_organism);
		godot_organism->setOrganism(organism.get());
		auto pos = organism->getPosition();
		int iX = static_cast<int>(pos[0]);
		int iZ = static_cast<int>(pos[2]);
		float verticalOffset = 0;
		if(iX >= 0 && iX < heightMapMesh->width && iZ >= 0 && iZ < heightMapMesh->height) {
			verticalOffset = vertices[iZ*heightMapMesh->width+iX].y;
		}
		godot_organism->set_translation(Vector3(pos[0], pos[1] + verticalOffset, pos[2]));
	}
}
void HeightMapStaticBody::despawn(std::shared_ptr<::Object> object) {
	auto organism = std::dynamic_pointer_cast<Organism>(object);
	if(organism) {
		for(auto& organism_node : organisms) {
			if(organism_node->organism == organism.get()) {
				remove_child(organism_node);
				organisms.erase(std::remove(organisms.begin(), organisms.end(), organism_node), organisms.end());
			}
		}
	}
}