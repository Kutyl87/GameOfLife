#ifndef HEIGHT_MAP_STATIC_BODY_H
#define HEIGHT_MAP_STATIC_BODY_H

#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <CollisionShape.hpp>
#include <ConcavePolygonShape.hpp>
#include <ResourceLoader.hpp>
#include <PhysicsMaterial.hpp>
#include "height_map_mesh.h"
#include <simulationManager.h>
#include <godot_organism.h>
#include "godot_food.h"
#include <food.h>

namespace godot {

	class HeightMapStaticBody : public StaticBody {
		GODOT_CLASS(HeightMapStaticBody, StaticBody)

	private:
		String heightmapPath;
		float maxHeight;
		float mapSize;

		MeshInstance* meshInstance;
		CollisionShape* collisionShape;
		bool ready = false;
		Ref<PhysicsMaterial> physicsMaterial;
		std::unique_ptr<SimulationManager> manager;
		std::vector<GodotOrganism*> organisms;
		std::vector<GodotFood*> foods;
		void spawn(std::shared_ptr<::Object> object);
		void despawn(std::shared_ptr<::Object> object);
		PoolVector3Array vertices;
		Ref<HeightMapMesh> heightMapMesh;
	public:
		static void _register_methods();

		HeightMapStaticBody();
		~HeightMapStaticBody();

		void _init();
		void _ready();

		void setHeightmapPath(String path);
		String getHeightmapPath() const;

		void setMaxHeight(float height);
		float getMaxHeight() const;

		void setMapSize(float size);
		float getMapSize() const;

		void generateHeightmapMesh();
	};

}

#endif // HEIGHT_MAP_STATIC_BODY_H
