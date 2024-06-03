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

		void _init(); // Our initializer called by Godot

		void set_heightmap_path(String path);
		bool generate_mesh_from_heightmap(float max_height);

	private:
		void generate_vertices(Ref<Image> heightmap, PoolVector3Array &vertices, PoolVector3Array &normals, float max_height);
		Vector3 calculate_normal(Ref<Image> heightmap, int x, int y, float max_height, float pixelSize);
		void generate_indices(int width, int height, PoolIntArray &indices);

		String heightmap_path;
	};

}

#endif // HEIGHT_MAP_MESH_H