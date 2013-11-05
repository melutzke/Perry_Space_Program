/*	CS 559 Project 2
	Mitchell Lutzke & Steve Krejci

	The Meshpack class is a fancy container class for a mesh object's
	vertices, indexes, and normals, and is then used to generate
	a given scene.
*/

#include <iostream>
#include "meshpack.h"

using namespace std;
using namespace glm;

MeshPack::MeshPack(vector<VertexAttributesPCNT> vertices, vector<GLuint> vertex_indices)
{
	this->vertices = vertices;
	this->vertex_indices = vertex_indices;
	this->m = mat4(1.0f);
}

void MeshPack::addToScene(vector<VertexAttributesPCNT>& arg_vertices, vector<GLuint>& arg_vertex_indices){

	vector<VertexAttributesPCNT> temp_vertices;

	for(unsigned i = 0; i < this->vertices.size(); i++){
		temp_vertices.push_back(this->vertices[i]);
		temp_vertices[temp_vertices.size() - 1].position = vec3(this->m * vec4( this->vertices[i].position, 1));
		temp_vertices[temp_vertices.size() - 1].normal = normalize(vec3(this->m * vec4(this->vertices[i].normal, 1)));
	}

	for(unsigned i = 0; i < this->vertex_indices.size(); i++){
		this->vertex_indices[i] = this->vertex_indices[i] + arg_vertices.size();
	}

	arg_vertices.insert(arg_vertices.end(), temp_vertices.begin(), temp_vertices.end());
	arg_vertex_indices.insert(arg_vertex_indices.end(), this->vertex_indices.begin(), this->vertex_indices.end());

	temp_vertices.clear();
}