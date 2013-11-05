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
	static MeshPack* Cylinder(glm::mat4 m, float top_radius, float bot_radius, unsigned int stacks, unsigned int slices, glm::vec3 color);
	static MeshPack* Sphere(glm::mat4 m, float radius, unsigned int stacks, unsigned int slices, glm::vec3 color);
	static MeshPack* Mars(glm::mat4 m, float radius, std::string the_file);
	static glm::vec3 getNormal(std::vector<VertexAttributesPCNT>& vertices, int i, int stacks, int slices);

private:
	static int up(int index, int stacks, int slices);
	static int down(int index, int stacks, int slices);
	static int left(int index, int stacks, int slices);
	static int right(int index, int stacks, int slices);
	static void wind(std::vector<GLuint>& vertex_indices, int stacks, int slices);

	std::vector<VertexAttributesPCNT> vertices;
	std::vector<GLuint> vertex_indices;
};
