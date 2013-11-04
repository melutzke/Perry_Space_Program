/*	A more sophisticated Hello World using modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	Created:	02/28/13
	Updates:
*/
#include <iostream>
#include "meshpack.h"

using namespace std;
using namespace glm;

MeshPack::MeshPack(vector<VertexAttributesPCNT> vertices, vector<GLuint> vertex_indices, vector<GLuint> normal_indices)
{
	this->vertices = vertices;
	this->vertex_indices = vertex_indices;
	this->normal_indices = normal_indices;
	this->m = mat4(1.0f);
}

void MeshPack::addToScene(vector<VertexAttributesPCNT>& arg_vertices, vector<GLuint>& arg_vertex_indices, vector<GLuint>& arg_normal_indices){

	vector<VertexAttributesPCNT> temp_vertices;

	for(int i = 0; i < this->vertices.size(); i++){
		temp_vertices.push_back(this->vertices[i]);
		temp_vertices[temp_vertices.size() - 1].position = vec3(this->m * vec4( this->vertices[i].position, 1));
		temp_vertices[temp_vertices.size() - 1].normal = normalize(vec3(this->m * vec4(this->vertices[i].normal, 1)));
	}

	for(int i = 0; i < this->vertex_indices.size(); i++){
		this->vertex_indices[i] = this->vertex_indices[i] + arg_vertices.size();
	}

	arg_vertices.insert(arg_vertices.end(), temp_vertices.begin(), temp_vertices.end());
	arg_vertex_indices.insert(arg_vertex_indices.end(), this->vertex_indices.begin(), this->vertex_indices.end());
	arg_normal_indices.insert(arg_normal_indices.end(), this->normal_indices.begin(), this->normal_indices.end());

	temp_vertices.clear();
}
void MeshPack::rotate(float degrees, vec3 direction){
	this->m = glm::rotate(this->m, degrees, direction);
}
void MeshPack::translate(vec3 position){
	this->m = glm::translate(this->m, position);
}
void MeshPack::scale(vec3 scale){
	this->m = glm::scale(this->m, scale);
}