#include "height_map_mesh.h"

#include <ResourceLoader.hpp>

using namespace godot;
const int smoothingSteps = 10;

void HeightMapMesh::_register_methods() {
	register_method("generateMeshFromHeightmap", &HeightMapMesh::generateMeshFromHeightmap);
	register_method("setHeightmapPath", &HeightMapMesh::setHeightmapPath);

	register_property<HeightMapMesh, String>("heightmapPath", &HeightMapMesh::heightmapPath, String(""));
}

HeightMapMesh::HeightMapMesh() {}
HeightMapMesh::~HeightMapMesh() {}

void HeightMapMesh::_init() {}

void HeightMapMesh::setHeightmapPath(String path) {
	heightmapPath = path;
}

bool HeightMapMesh::generateMeshFromHeightmap(float maxHeight, float mapSize) {
	Ref<Image> heightmap = ResourceLoader::get_singleton()->load(heightmapPath);

	if (!heightmap.is_valid()) {
		return false;
	}

	PoolVector3Array normals;
	PoolVector3Array smoothNormals;
	PoolIntArray indices;

	heightmap->lock();

	width = heightmap->get_width();
	height = heightmap->get_height();

	generateVertices(heightmap, vertices, normals, maxHeight, mapSize);
	generateIndices(width, height, indices);
	heightmap->unlock();

	for(int i = 0; i < smoothingSteps; ++i) {
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

void HeightMapMesh::generateVertices(Ref<Image> heightmap, PoolVector3Array &vertices, PoolVector3Array &normals, float maxHeight, float mapSize) {
	int width = heightmap->get_width();
	int height = heightmap->get_height();

	vertices.resize(width * height);
	normals.resize(width * height);

	float pixelSize = mapSize / width;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float heightValue = heightmap->get_pixel(x, y).r * maxHeight;
			Vector3 vertex(x * pixelSize, heightValue, y * pixelSize);
			vertices.set(y * width + x, vertex);
		}
	}

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Vector3 normal = calculateNormal(heightmap, x, y, maxHeight, pixelSize);
			normals.set(y * width + x, normal);
		}
	}
}

Vector3 HeightMapMesh::calculateNormal(Ref<Image> heightmap, int x, int y, float maxHeight, float pixelSize) {
	int width = heightmap->get_width();
	int height = heightmap->get_height();

	float heightL = (x > 0) ? heightmap->get_pixel(x - 1, y).r * maxHeight : heightmap->get_pixel(x, y).r * maxHeight;
	float heightR = (x < width - 1) ? heightmap->get_pixel(x + 1, y).r * maxHeight : heightmap->get_pixel(x, y).r * maxHeight;
	float heightD = (y > 0) ? heightmap->get_pixel(x, y - 1).r * maxHeight : heightmap->get_pixel(x, y).r * maxHeight;
	float heightU = (y < height - 1) ? heightmap->get_pixel(x, y + 1).r * maxHeight : heightmap->get_pixel(x, y).r * maxHeight;

	Vector3 normal(heightL - heightR, 2.0 * pixelSize, heightD - heightU);
	return normal.normalized();
}

void HeightMapMesh::generateIndices(int width, int height, PoolIntArray &indices) {
	for (int y = 0; y < height - 1; ++y) {
		for (int x = 0; x < width - 1; ++x) {
			int topLeft = y * width + x;
			int bottomLeft = (y + 1) * width + x;
			int topRight = y * width + (x + 1);
			int bottomRight = (y + 1) * width + (x + 1);

			indices.append(topLeft);
			indices.append(topRight);
			indices.append(bottomLeft);

			indices.append(topRight);
			indices.append(bottomRight);
			indices.append(bottomLeft);
		}
	}
}
