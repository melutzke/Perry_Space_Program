/*	A more sophisticated Hello World using modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	Created:	02/28/13
	Updates:
*/

#include "meshpack.h"

using namespace glm;
using namespace std;

MeshPack::MeshPack(vector<VertexAttributesPCN> vertices, vector<GLuint> vertex_indices, vector<GLuint> normal_indices)
{
	this->vertices = vertices;
	this->vertex_indices = vertex_indices;
	this->normal_indices = normal_indices;
	//this->texture_coordinate = vec2(0.0f);
}
