/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"
#include "meshpack.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	static MeshPack* Cylinder(float top_radius, float bot_radius, unsigned int stacks, unsigned int slices, glm::vec3 coords, glm::vec3 scaleVec, glm::vec3 color);
	static MeshPack* Sphere(float radius, unsigned int stacks, unsigned int slices, glm::vec3 coords, glm::vec3 scaleVec, glm::vec3 color);
	static MeshPack* Experimental(float radius, unsigned int rings, unsigned int sectors, glm::vec3 coords);
	static glm::vec3 getNormal(std::vector<VertexAttributesPCN> vertices, int i, int stacks, int slices);
	//MeshPack Sphere(int stacks, int slices, glm::vec3 color);

private:
	void BuildNormalVisualizationGeometry();
	static int up(int index, int stacks, int slices);
	static int down(int index, int stacks, int slices);
	static int left(int index, int stacks, int slices);
	static int right(int index, int stacks, int slices);
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	std::vector<VertexAttributesP> normal_vertices;
	std::vector<GLuint> vertex_indices;
	std::vector<GLuint> normal_indices;
};
