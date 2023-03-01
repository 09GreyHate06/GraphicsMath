#pragma once
#include <vector>
#include <array>

namespace GM::Utils
{
	class BasicMesh
	{
	public:
		static std::vector<float> CreateCubeVertices(bool hasTexCoords, bool hasNormals, float minPos, float maxPos, float texCoordU, float texCoordV);
		static 	std::vector<float> CreatePlaneVertices(bool hasTexCoords, bool hasNormals, float minPos, float maxPos, float texCoordU, float texCoordV);

		static std::array<uint32_t, 36> CreateCubeIndices();
		std::array<uint32_t, 6> CreatePlaneIndices();

	private:
		BasicMesh() = default;
	};
}