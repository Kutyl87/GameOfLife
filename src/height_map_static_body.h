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
		String heightmapPath;
		float maxHeight;
		float mapSize;

		MeshInstance* meshInstance;
		CollisionShape* collisionShape;
		bool ready = false;

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
