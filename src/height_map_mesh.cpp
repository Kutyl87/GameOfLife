#include "height_map_mesh.h"

#include <ResourceLoader.hpp>

using namespace godot;

void HeightMapMesh::_register_methods() {
	register_method("generate_mesh_from_heightmap", &HeightMapMesh::generate_mesh_from_heightmap);
	register_method("set_heightmap_path", &HeightMapMesh::set_heightmap_path);

	register_property<HeightMapMesh, String>("heightmap_path", &HeightMapMesh::heightmap_path, String(""));
}

HeightMapMesh::HeightMapMesh() {}
HeightMapMesh::~HeightMapMesh() {}

void HeightMapMesh::_init() {}

void HeightMapMesh::set_heightmap_path(String path) {
	heightmap_path = path;
}

bool HeightMapMesh::generate_mesh_from_heightmap(float max_height) {
	Ref<Image> heightmap = ResourceLoader::get_singleton()->load(heightmap_path);

	if (!heightmap.is_valid()) {
		Godot::print("Invalid heightmap image.");
		return false;
	}

	PoolVector3Array vertices;
	PoolVector3Array normals;
	PoolVector3Array smoothNormals;
	PoolIntArray indices;

	heightmap->lock();

	int width = heightmap->get_width();
	int height = heightmap->get_height();

	generate_vertices(heightmap, vertices, normals, max_height);
	generate_indices(width, height, indices);
	heightmap->unlock();

	for(int i = 0; i < 10; ++i) {
		smoothNormals = normals;
		for (int y = 1; y < height-1; ++y) {
			for (int x = 1; x < width-1; ++x) {
				smoothNormals.set(y * width + x, (normals[(y+1) * width + x] + normals[(y-1) * width + x] + normals[y * width + x+1] + normals[y * width + x-1]).normalized());
			}
		}
		normals = smoothNormals;
	}

	Array arrays;
	arrays.resize(ArrayMesh::ARRAY_MAX);
	arrays[ArrayMesh::ARRAY_VERTEX] = vertices;
	arrays[ArrayMesh::ARRAY_INDEX] = indices;
	arrays[ArrayMesh::ARRAY_NORMAL] = normals;

	clear_surfaces();
	add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
	return true;
}

void HeightMapMesh::generate_vertices(Ref<Image> heightmap, PoolVector3Array &vertices, PoolVector3Array &normals, float max_height) {
	int width = heightmap->get_width();
	int height = heightmap->get_height();

	vertices.resize(width * height);
	normals.resize(width * height);

	float pixelSize = 100.0 / width;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float height_value = heightmap->get_pixel(x, y).r * max_height;
			Vector3 vertex(x * pixelSize, height_value, y * pixelSize);
			vertices.set(y * width + x, vertex);
		}
	}

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Vector3 normal = calculate_normal(heightmap, x, y, max_height, pixelSize);
			normals.set(y * width + x, normal);
		}
	}
}

Vector3 HeightMapMesh::calculate_normal(Ref<Image> heightmap, int x, int y, float max_height, float pixelSize) {
	int width = heightmap->get_width();
	int height = heightmap->get_height();

	float heightL = (x > 0) ? heightmap->get_pixel(x - 1, y).r * max_height : heightmap->get_pixel(x, y).r * max_height;
	float heightR = (x < width - 1) ? heightmap->get_pixel(x + 1, y).r * max_height : heightmap->get_pixel(x, y).r * max_height;
	float heightD = (y > 0) ? heightmap->get_pixel(x, y - 1).r * max_height : heightmap->get_pixel(x, y).r * max_height;
	float heightU = (y < height - 1) ? heightmap->get_pixel(x, y + 1).r * max_height : heightmap->get_pixel(x, y).r * max_height;

	Vector3 normal(heightL - heightR, 2.0 * pixelSize, heightD - heightU);
	return normal.normalized();
}

void HeightMapMesh::generate_indices(int width, int height, PoolIntArray &indices) {
	for (int y = 0; y < height - 1; ++y) {
		for (int x = 0; x < width - 1; ++x) {
			int top_left = y * width + x;
			int bottom_left = (y + 1) * width + x;
			int top_right = y * width + (x + 1);
			int bottom_right = (y + 1) * width + (x + 1);

			indices.append(top_left);
			indices.append(top_right);
			indices.append(bottom_left);

			indices.append(top_right);
			indices.append(bottom_right);
			indices.append(bottom_left);
		}
	}
}