#include "MeshComponent.h"

#include "SharedTransformations.h"
#include "SharedMaterials.h"

#define VERBOSE false

/** @brief	Definition of a static data member; meshComps */
std::vector<class MeshComponent*> MeshComponent::meshComps;

std::unordered_map<std::string, BaseMeshLoad> MeshComponent::loadedModels;

MeshComponent::~MeshComponent()
{
	if (VERBOSE) cout << "MeshComponent destructor called " << endl;

	if (loadedModels.size() > 0) {

		// Search for the model among those that were previously loaded
		auto iter = loadedModels.find(scaleMeshName);

		if (iter != loadedModels.end()) {

			iter->second.copyCount -= 1;

			if (VERBOSE) cout << "objects left of this type " << iter->second.copyCount << endl;

			if (iter->second.copyCount <= 0)
			{
				if (VERBOSE) cout << "freeing all resouces for " << scaleMeshName << " model" << endl;

				for (auto& subMesh : iter->second.modelSubMeshes) {

					glDeleteVertexArrays(1, &subMesh.vao);

					glDeleteBuffers(1, &subMesh.vertexBuffer);

					if (subMesh.renderMode == INDEXED) {
						glDeleteBuffers(1, &subMesh.indexBuffer);
					}
				}

				loadedModels.erase(iter);

				delete collisionShape;
			}
			else {

				if (VERBOSE) cout << scaleMeshName << " not being deleted" << endl;
			}
			//if (VERBOSE) listModels();
		}
	}
	
} // end destructor


// Preform drawing operations. 
void MeshComponent::draw() const
{
	if (this->owningGameObject->getState() == ACTIVE) {

		// Use the shader program for this MeshComponent
		glUseProgram(shaderProgram);

		SharedTransformations::setModelingMatrix(this->owningGameObject->getModelingTransformation());

		// Render all subMeshes
		for (auto & subMesh : subMeshes) {

			// Bind vertex array object for the subMesh
			glBindVertexArray(subMesh.vao);

			//glUniform1i(102, static_cast<int>(subMesh.material.textureMode));

			//if (subMesh.material.diffuseTextureEnabled == true) {
			//	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseTextureObject);
			//}
			//glUniform4fv(101, 1, glm::value_ptr(subMesh.material.diffuseMat));
			
			SharedMaterials::setShaderMaterialProperties(subMesh.material);

			// Determine if sequential(ordered) or indexed rendering will be 
			// used to render the sub Mesh
			if (subMesh.renderMode == ORDERED) {

				// Trigger vertex fetch for ordered rendering 
				glDrawArrays(subMesh.primitiveMode, 0, subMesh.count);

			}
			else if (subMesh.renderMode == INDEXED) {

				// Trigger vertex fetch for indexed rendering 
				glDrawElements(subMesh.primitiveMode, subMesh.count, GL_UNSIGNED_INT, 0);
			}

			SharedMaterials::cleanUpMaterial(subMesh.material);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

} // end draw


SubMesh  MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData)
{
	// Create the SubMesh to be configured for the vertex data
	SubMesh subMesh;

	// Generate, bind, and load the vertex array object.
	// Store the identifier for the vertex array object in the subMesh
	glGenVertexArrays(1, &subMesh.vao);
	glBindVertexArray(subMesh.vao);

	// Create buffer and load the vertex positions and colors into it.
	// Store the identifier for the buffer in the subMesh.
	glGenBuffers(1, &subMesh.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, subMesh.vertexBuffer);

	// Specify the location and data format of the vertex positions and colors.
	// Enable flow of the vertex data to a specified location in the vertex shader.
	// Load the vertex colors into a buffer
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(pntVertexData), &vertexData[0], GL_STATIC_DRAW);

	// Specify the location and data format of an array of vertex positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	// Specify the location and data format of an array of vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const void*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	// Specify the location and data format of an array of vertex texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 
		(const void*)(sizeof(glm::vec4) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	// Store the number of vertices to be rendered in the subMesh
	subMesh.count = static_cast<GLuint>(vertexData.size());

	// Store the renderMode in the subMesh for ORDERED rendering
	subMesh.renderMode = ORDERED;

	return subMesh;

} // end buildSubMesh


SubMesh MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices)
{
	// Create the SubMesh to be configured for the vertex data
	SubMesh subMesh = buildSubMesh(vertexData);
	
	// Create buffer and load the indices into it.
	// Store the identifier for the index buffer in the subMesh.
	glGenBuffers(1, &subMesh.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),&indices[0], GL_STATIC_DRAW);

	// Store the number of indices to be process when rendering the subMesh
	subMesh.count = static_cast<GLuint>(indices.size());

	// Store the renderMode in the subMesh for INDEXED rendering
	subMesh.renderMode = INDEXED;

	return subMesh;

} // end buildSubMesh

void MeshComponent::addMeshComp()
{
	auto iter = std::find(meshComps.begin(), meshComps.end(), this);

	if (iter == meshComps.end()) {

		this->buildMesh();

		meshComps.emplace_back(this);
		std::sort(meshComps.begin(), meshComps.end(), Component::CompareUpdateOrder);
	}

} // end addMeshComp

void MeshComponent::removeMeshComp()
{
	auto iter = std::find(meshComps.begin(), meshComps.end(), this);

	if (iter != meshComps.end()) {

		if (VERBOSE) cout << "removeMeshComp" << endl;

		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, meshComps.end() - 1);
		meshComps.pop_back();
	}

} // end removeMeshComp

const std::vector<class MeshComponent*> MeshComponent::GetMeshComponents()
{
	return meshComps;

}

bool MeshComponent::previsouslyLoaded()
{
	// Search for the model among those that were previously loaded
	auto iter = loadedModels.find(scaleMeshName);

	// Check if the model was previously loaded
	if (iter != loadedModels.end()) {

		if (VERBOSE) std::cout << endl << "Retrieving mesh: " << scaleMeshName << endl;

		subMeshes = iter->second.modelSubMeshes;

		this->collisionShape = iter->second.collisionShape;

		iter->second.copyCount += 1;

		if (VERBOSE) std::cout << " copyCount = " << iter->second.copyCount << std::endl;

		if (VERBOSE) listLoadedMeshes();
		
		return true;
	}
	else {

		return false;

	}

} // end previsouslyLoaded



void MeshComponent::saveInitialLoad()
{
	if (VERBOSE) std::cout << endl << "Loading Mesh: " << scaleMeshName << std::endl;

	BaseMeshLoad modelRecord;

	modelRecord.modelSubMeshes = subMeshes;
	modelRecord.collisionShape = this->collisionShape;
	modelRecord.copyCount = 1;

	// Add the loaded model to the map containing all loaded
	// models to avoid loading it a second time.
	loadedModels.emplace(scaleMeshName, modelRecord);

	if (VERBOSE) listLoadedMeshes();
}


void MeshComponent::listLoadedMeshes()
{
	cout << endl << "MODEL LIST:" << endl;

	for (auto& model : loadedModels) {

		cout << model.first << ", ";

	}
	cout << endl;
}