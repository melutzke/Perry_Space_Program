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
}

void MeshPack::addToScene(vector<VertexAttributesPCN>& arg_vertices, vector<GLuint>& arg_vertex_indices, vector<GLuint>& arg_normal_indices){
	for(int i = 0; i < this->vertex_indices.size(); i++){
		this->vertex_indices[i] = this->vertex_indices[i] + arg_vertices.size();
	}

	arg_vertices.insert(arg_vertices.end(), this->vertices.begin(), this->vertices.end());
	arg_vertex_indices.insert(arg_vertex_indices.end(), this->vertex_indices.begin(), this->vertex_indices.end());
	arg_normal_indices.insert(arg_normal_indices.end(), this->normal_indices.begin(), this->normal_indices.end());
}

