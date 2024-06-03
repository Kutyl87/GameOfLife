#ifndef HEIGHT_MAP_MESH_H
#define HEIGHT_MAP_MESH_H

#include <Godot.hpp>
#include <ArrayMesh.hpp>
#include <Image.hpp>
#include <String.hpp>

namespace godot {

	class HeightMapMesh : public ArrayMesh {
		GODOT_CLASS(HeightMapMesh, ArrayMesh)

	public:
		static void _register_methods();

		HeightMapMesh();
		~HeightMapMesh();

		void _init();

		void setHeightmapPath(String path);
		bool generateMeshFromHeightmap(float maxHeight, float mapSize);

	private:
		void generateVertices(Ref<Image> heightmap, PoolVector3Array &vertices, PoolVector3Array &normals, float maxHeight, float mapSize);
		Vector3 calculateNormal(Ref<Image> heightmap, int x, int y, float maxHeight, float pixelSize);
		void generateIndices(int width, int height, PoolIntArray &indices);

		String heightmapPath;
	};

}

#endif // HEIGHT_MAP_MESH_H
