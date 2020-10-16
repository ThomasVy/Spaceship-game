#include "Geometry.h"

#include <utility>


GPU_Geometry::GPU_Geometry()
	: vao()
	, vertBuffer(0, 3, GL_FLOAT)
	, texCoordBuffer(1, 2, GL_FLOAT)
	, textureIDBuffer(2, 1, GL_FLOAT, 1)
	, transformationMatrix(3, sizeof(glm::vec4), 4,  4, GL_FLOAT)
{}


void GPU_Geometry::setVerts(const std::vector<glm::vec3>& verts) {
	vertBuffer.uploadData(sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
}


void GPU_Geometry::setTexCoords(const std::vector<glm::vec2>& texCoords) {
	texCoordBuffer.uploadData(sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
}

void GPU_Geometry::setMatrixTrans(const std::vector<glm::mat4>& matrixTrans)
{
	transformationMatrix.uploadData(sizeof(glm::mat4) * matrixTrans.size(), matrixTrans.data(), GL_STATIC_DRAW);
}

void GPU_Geometry::setTextureID(const std::vector<float>& textureIds)
{
	textureIDBuffer.uploadData(sizeof(float) * textureIds.size(), textureIds.data(), GL_STATIC_DRAW);
}
