#pragma once
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertexattributes.h"

/*	Perry Kivolowitz - UW - Madison - CS 559 Demo
	A  person more  expert in templates whould be able
	to collect these classes into a single templatized
	system. The goal  would be to allow a more generic
	"object" class.

	P = position
	C = color
	N = normal
	T = texture coordinates
*/

class MeshPack
{
public:
	MeshPack(std::vector<VertexAttributesPCNT> vertices, std::vector<GLuint> vertex_indices, std::vector<GLuint> normal_indices);
	std::vector<VertexAttributesPCNT> vertices;
	std::vector<GLuint> vertex_indices;
	std::vector<VertexAttributesP> normal_vertices;
	std::vector<GLuint> normal_indices;
	glm::mat4 m;
	void addToScene(std::vector<VertexAttributesPCNT>& vertices, std::vector<GLuint>& vertex_indices, std::vector<GLuint>& normal_indices);
};