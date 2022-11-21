#include "BoxMeshComponent.h"

BoxMeshComponent::BoxMeshComponent(Material material, GLuint shaderProgram, float width, float height, float depth, int updateOrder)
	: boxMat(material), MeshComponent(shaderProgram, updateOrder),
	  halfWidth(width / 2.0f), halfHeight(height / 2.0f), halfDepth(depth / 2.0f)
{

}

void BoxMeshComponent::buildMesh()
{
	this->scaleMeshName = "Box " + std::to_string(halfWidth) + " " +
						   std::to_string(halfHeight) + " " + std::to_string(halfDepth) + std::to_string(this->boxMat._id);

	if (previsouslyLoaded() == false) {

		std::vector<pntVertexData> vData;
		std::vector<unsigned int> indices;

		vec4 v1(-halfWidth, -halfHeight, halfDepth, 1.0f);
		vec4 v2(halfWidth, -halfHeight, halfDepth, 1.0f);
		vec4 v3(halfWidth, halfHeight, halfDepth, 1.0f);
		vec4 v4(-halfWidth, halfHeight, halfDepth, 1.0f);
		vec4 v5(-halfWidth, -halfHeight, -halfDepth, 1.0f);
		vec4 v6(halfWidth, -halfHeight, -halfDepth, 1.0f);
		vec4 v7(halfWidth, halfHeight, -halfDepth, 1.0f);
		vec4 v8(-halfWidth, halfHeight, -halfDepth, 1.0f);

		vec3 n1(0, 0, 1);
		vec3 n2(0, 0, -1);
		vec3 n3(1, 0, 0);
		vec3 n4(-1, 0, 0);
		vec3 n5(0, -1, 0);
		vec3 n6(0, 1, 0);

		vec2 t1(0, 1);
		vec2 t2(1, 1);
		vec2 t3(1, 0);
		vec2 t4(0, 0);

		// front face
		vData.push_back(pntVertexData(v1, n1, t4));
		vData.push_back(pntVertexData(v2, n1, t3));
		vData.push_back(pntVertexData(v3, n1, t2));
		vData.push_back(pntVertexData(v4, n1, t1));

		// right face
		vData.push_back(pntVertexData(v2, n3, ZERO_V2));
		vData.push_back(pntVertexData(v6, n3, ZERO_V2));
		vData.push_back(pntVertexData(v7, n3, ZERO_V2));
		vData.push_back(pntVertexData(v3, n3, ZERO_V2));

		// left face
		vData.push_back(pntVertexData(v5, n4, ZERO_V2));
		vData.push_back(pntVertexData(v1, n4, ZERO_V2));
		vData.push_back(pntVertexData(v4, n4, ZERO_V2));
		vData.push_back(pntVertexData(v8, n4, ZERO_V2));

		// back face
		vData.push_back(pntVertexData(v8, n2, ZERO_V2));
		vData.push_back(pntVertexData(v7, n2, ZERO_V2));
		vData.push_back(pntVertexData(v6, n2, ZERO_V2));
		vData.push_back(pntVertexData(v5, n2, ZERO_V2));

		// top
		vData.push_back(pntVertexData(v4, n6, ZERO_V2));
		vData.push_back(pntVertexData(v3, n6, ZERO_V2));
		vData.push_back(pntVertexData(v7, n6, ZERO_V2));
		vData.push_back(pntVertexData(v8, n6, ZERO_V2));

		// bot
		vData.push_back(pntVertexData(v6, n5, ZERO_V2));
		vData.push_back(pntVertexData(v2, n5, ZERO_V2));
		vData.push_back(pntVertexData(v1, n5, ZERO_V2));
		vData.push_back(pntVertexData(v5, n5, ZERO_V2));

		// Push back the indices for each face
		for (int i = 0; i < 6; i++) {
			indices.push_back(0 + 4 * i);
			indices.push_back(1 + 4 * i);
			indices.push_back(2 + 4 * i);
			indices.push_back(2 + 4 * i);
			indices.push_back(3 + 4 * i);
			indices.push_back(0 + 4 * i);
		}

		SubMesh boxMesh = MeshComponent::buildSubMesh(vData, indices);

		boxMesh.material = this->boxMat;

		this->subMeshes.push_back(boxMesh);

		this->collisionShape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));

		this->saveInitialLoad();
	}
	
} // end initialize

