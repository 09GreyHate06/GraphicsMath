#include "BasicMesh.h"

namespace GM::Utils
{
	std::vector<float> BasicMesh::CreateCubeVertices(bool hasTexCoords, bool hasNormals, float minPos, float maxPos, float texCoordU, float texCoordV)
	{
		std::vector<float> vertices;
		if (hasTexCoords && hasNormals)
		{
			vertices =
			{
				//  back		             						  
				maxPos, maxPos, maxPos,    0.0f,	     0.0f,           0.0f, 0.0f, 1.0f,
				minPos, maxPos, maxPos,    texCoordU,  0.0f,           0.0f, 0.0f, 1.0f,
				minPos, minPos, maxPos,    texCoordU,  texCoordV,	     0.0f, 0.0f, 1.0f,
				maxPos, minPos, maxPos,    0.0f,	     texCoordV,	     0.0f, 0.0f, 1.0f,

				// front
				minPos, maxPos, minPos,    0.0f,	     0.0f,          0.0f, 0.0f, -1.0f,
				maxPos, maxPos, minPos,    texCoordU,  0.0f,          0.0f, 0.0f, -1.0f,
				maxPos, minPos, minPos,    texCoordU,  texCoordV,     0.0f, 0.0f, -1.0f,
				minPos, minPos, minPos,    0.0f,	     texCoordV,     0.0f, 0.0f, -1.0f,

				// bottom			     	      
				maxPos, minPos, maxPos,    0.0f,	     0.0f,          0.0f, -1.0f,  0.0,
				minPos, minPos, maxPos,    texCoordU,  0.0f,          0.0f, -1.0f,  0.0,
				minPos, minPos, minPos,    texCoordU,  texCoordV,     0.0f, -1.0f,  0.0,
				maxPos, minPos, minPos,    0.0f,	     texCoordV,     0.0f, -1.0f,  0.0,

				// top		      	     					  
				minPos, maxPos, maxPos,    0.0f,	     0.0f,           0.0f, 1.0f, 0.0f,
				maxPos, maxPos, maxPos,    texCoordU,  0.0f,           0.0f, 1.0f, 0.0f,
				maxPos, maxPos, minPos,    texCoordU,  texCoordV,      0.0f, 1.0f, 0.0f,
				minPos, maxPos, minPos,    0.0f,	     texCoordV,      0.0f, 1.0f, 0.0f,

				// left				     	     
				minPos, maxPos, maxPos,    0.0f,	     0.0f,     	    -1.0f, 0.0f,  0.0,
				minPos, maxPos, minPos,    texCoordU,  0.0f,     	    -1.0f, 0.0f,  0.0,
				minPos, minPos, minPos,    texCoordU,  texCoordV,     -1.0f, 0.0f,  0.0,
				minPos, minPos, maxPos,    0.0f,	     texCoordV,     -1.0f, 0.0f,  0.0,

				// right			             
				maxPos, maxPos, minPos,    0.0f,	     0.0f,           1.0f, 0.0f, 0.0f,
				maxPos, maxPos, maxPos,    texCoordU,  0.0f,           1.0f, 0.0f, 0.0f,
				maxPos, minPos, maxPos,    texCoordU,  texCoordV,      1.0f, 0.0f, 0.0f,
				maxPos, minPos, minPos,    0.0f,	     texCoordV,      1.0f, 0.0f, 0.0f,
			};
		}
		else if (hasTexCoords && !hasNormals)
		{
			vertices =
			{
				//  back		             						  
				maxPos, maxPos, maxPos,    0.0f,	     0.0f,
				minPos, maxPos, maxPos,    texCoordU,  0.0f,
				minPos, minPos, maxPos,    texCoordU,  texCoordV,
				maxPos, minPos, maxPos,    0.0f,	     texCoordV,

				// front
				minPos, maxPos, minPos,    0.0f,	     0.0f,
				maxPos, maxPos, minPos,    texCoordU,  0.0f,
				maxPos, minPos, minPos,    texCoordU,  texCoordV,
				minPos, minPos, minPos,    0.0f,	     texCoordV,

				// bottom			     	      					 
				maxPos, minPos, maxPos,    0.0f,	     0.0f,
				minPos, minPos, maxPos,    texCoordU,  0.0f,
				minPos, minPos, minPos,    texCoordU,  texCoordV,
				maxPos, minPos, minPos,    0.0f,	     texCoordV,

				// top		      	     					  
				minPos, maxPos, maxPos,    0.0f,	     0.0f,
				maxPos, maxPos, maxPos,    texCoordU,  0.0f,
				maxPos, maxPos, minPos,    texCoordU,  texCoordV,
				minPos, maxPos, minPos,    0.0f,	     texCoordV,

				// left				     	     
				minPos, maxPos, maxPos,    0.0f,	     0.0f,
				minPos, maxPos, minPos,    texCoordU,  0.0f,
				minPos, minPos, minPos,    texCoordU,  texCoordV,
				minPos, minPos, maxPos,    0.0f,	     texCoordV,

				// right			             
				maxPos, maxPos, minPos,    0.0f,	     0.0f,
				maxPos, maxPos, maxPos,    texCoordU,  0.0f,
				maxPos, minPos, maxPos,    texCoordU,  texCoordV,
				maxPos, minPos, minPos,    0.0f,	     texCoordV,
			};
		}
		else if (!hasTexCoords && hasNormals)
		{
			vertices =
			{
				//  back		             						  
				maxPos, maxPos, maxPos,    0.0f, 0.0f, 1.0f,
				minPos, maxPos, maxPos,    0.0f, 0.0f, 1.0f,
				minPos, minPos, maxPos,    0.0f, 0.0f, 1.0f,
				maxPos, minPos, maxPos,    0.0f, 0.0f, 1.0f,

				// front
				minPos, maxPos, minPos,    0.0f, 0.0f, -1.0f,
				maxPos, maxPos, minPos,    0.0f, 0.0f, -1.0f,
				maxPos, minPos, minPos,    0.0f, 0.0f, -1.0f,
				minPos, minPos, minPos,    0.0f, 0.0f, -1.0f,

				// bottom			     	      
				maxPos, minPos, maxPos,    0.0f, -1.0f,  0.0,
				minPos, minPos, maxPos,    0.0f, -1.0f,  0.0,
				minPos, minPos, minPos,    0.0f, -1.0f,  0.0,
				maxPos, minPos, minPos,    0.0f, -1.0f,  0.0,

				// top		      	     					  
				minPos, maxPos, maxPos,    0.0f, 1.0f, 0.0f,
				maxPos, maxPos, maxPos,    0.0f, 1.0f, 0.0f,
				maxPos, maxPos, minPos,    0.0f, 1.0f, 0.0f,
				minPos, maxPos, minPos,    0.0f, 1.0f, 0.0f,

				// left				     	     
				minPos, maxPos, maxPos,   -1.0f, 0.0f,  0.0,
				minPos, maxPos, minPos,   -1.0f, 0.0f,  0.0,
				minPos, minPos, minPos,   -1.0f, 0.0f,  0.0,
				minPos, minPos, maxPos,   -1.0f, 0.0f,  0.0,

				// right			             
				maxPos, maxPos, minPos,    1.0f, 0.0f, 0.0f,
				maxPos, maxPos, maxPos,    1.0f, 0.0f, 0.0f,
				maxPos, minPos, maxPos,    1.0f, 0.0f, 0.0f,
				maxPos, minPos, minPos,    1.0f, 0.0f, 0.0f,
			};

		}
		else
		{
			vertices =
			{
				//  back		             						  
				maxPos, maxPos, maxPos,
				minPos, maxPos, maxPos,
				minPos, minPos, maxPos,
				maxPos, minPos, maxPos,

				// front
				minPos, maxPos, minPos,
				maxPos, maxPos, minPos,
				maxPos, minPos, minPos,
				minPos, minPos, minPos,

				// bottom			     	      
				maxPos, minPos, maxPos,
				minPos, minPos, maxPos,
				minPos, minPos, minPos,
				maxPos, minPos, minPos,

				// top		      	     					  
				minPos, maxPos, maxPos,
				maxPos, maxPos, maxPos,
				maxPos, maxPos, minPos,
				minPos, maxPos, minPos,

				// left				     	     
				minPos, maxPos, maxPos,
				minPos, maxPos, minPos,
				minPos, minPos, minPos,
				minPos, minPos, maxPos,

				// right			             
				maxPos, maxPos, minPos,
				maxPos, maxPos, maxPos,
				maxPos, minPos, maxPos,
				maxPos, minPos, minPos,
			};
		}

		return vertices;
	}

	std::vector<float> BasicMesh::CreatePlaneVertices(bool hasTexCoords, bool hasNormals, float minPos, float maxPos, float texCoordU, float texCoordV)
	{
		std::vector<float> vertices;

		if (hasTexCoords && hasNormals)
		{
			vertices =
			{
				minPos, 0.0f, maxPos,   0.0f,      0.0f,       0.0f, 1.0f, 0.0f,
				maxPos, 0.0f, maxPos,   texCoordU, 0.0f,       0.0f, 1.0f, 0.0f,
				maxPos, 0.0f, minPos,   texCoordU, texCoordV,  0.0f, 1.0f, 0.0f,
				minPos, 0.0f, minPos,   0.0f,      texCoordV,  0.0f, 1.0f, 0.0f,
			};
		}
		else if (hasTexCoords && !hasNormals)
		{
			vertices =
			{
				minPos, 0.0f, maxPos,   0.0f,      0.0f,
				maxPos, 0.0f, maxPos,   texCoordU, 0.0f,
				maxPos, 0.0f, minPos,   texCoordU, texCoordV,
				minPos, 0.0f, minPos,   0.0f,      texCoordV,
			};
		}
		else if (!hasTexCoords && hasNormals)
		{
			vertices =
			{
				minPos, 0.0f, maxPos,   0.0f, 1.0f, 0.0f,
				maxPos, 0.0f, maxPos,   0.0f, 1.0f, 0.0f,
				maxPos, 0.0f, minPos,   0.0f, 1.0f, 0.0f,
				minPos, 0.0f, minPos,   0.0f, 1.0f, 0.0f,
			};
		}
		else
		{
			vertices =
			{
				minPos, 0.0f, maxPos,
				maxPos, 0.0f, maxPos,
				maxPos, 0.0f, minPos,
				minPos, 0.0f, minPos,
			};
		}

		return vertices;
	}

	std::array<uint32_t, 36> BasicMesh::CreateCubeIndices()
	{
		return std::move(std::array<uint32_t, 36>
		{
			// back
			0, 1, 2,
			2, 3, 0,

			// front
			4, 5, 6,
			6, 7, 4,

			// left
			8, 9, 10,
			10, 11, 8,

			// right
			12, 13, 14,
			14, 15, 12,

			// bottom
			16, 17, 18,
			18, 19, 16,

			// top
			20, 21, 22,
			22, 23, 20
		});
	}

	std::array<uint32_t, 6> BasicMesh::CreatePlaneIndices()
	{
		return std::move(std::array<uint32_t, 6>
		{
			0, 1, 2,
			2, 3, 0,
		});
	}

}
