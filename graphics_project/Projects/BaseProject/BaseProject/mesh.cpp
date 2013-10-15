/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern OpenGL techniques. 

	Object() is a sample base class upon which drawable objects might
	be derived. It assumes that all drawable objects have some geometry
	to buffer.

	Created:	2/25/13
*/

#include <iostream>
#include <assert.h>
#include "mesh.h"

using namespace std;
using namespace glm;

/*	Notice the destructor in this case asserts that all resources
	that don't go away by themselves have ALREADY been released. This
	is because the destructor might be called after a GL context has
	been destroyed, so I force the user of this class to call the
	TakeDown() purposefully.
*/

Mesh::Mesh()
{
	// don't do anything, really (yet anyway)
}

Mesh::~Mesh()
{
	// nothing to destruct yet
}

void BuildLastNormals(vector<VertexAttributesP> normal_vertices, vector<VertexAttributesP> normal_indices){
	
}

MeshPack* Mesh::Cylinder(int slices, vec3 color)
{
	if (slices < 0) slices = 1;

	slices *= 4;

	mat4 m;

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const float increment =  360.0f / float(slices);

	vector<VertexAttributesPCN> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	for (int i = 0; i < slices; ++i)
	{
		VertexAttributesPCN v0, v1 , v2, v3;
		float y_offset = 1;
		v0.position = vec3(m * x_axis) + vec3(0.0f, y_offset, 0.0f);
		v0.color = color;
		v0.normal = vec3(m * vec4(n, 1.0f));
		
		y_offset = (y_offset == 1) ? 0 : 1;
		v1.position = vec3(m * x_axis) + vec3(0.0f, y_offset, 0.0f);
		v1.color = color;
		v1.normal = vec3(m * vec4(n, 1.0f));

		m = rotate(m, increment, y_axis);
		
		y_offset = (y_offset == 1) ? 0 : 1;
		v2.position = vec3(m * x_axis) + vec3(0.0f, y_offset, 0.0f);
		v2.color = color;
		v2.normal = vec3(m * vec4(n, 1.0f));
		
		y_offset = (y_offset == 1) ? 0 : 1;
		v3.position = vec3(m * x_axis) + vec3(0.0f, y_offset, 0.0f);
		v3.color = color;
		v3.normal = vec3(m * vec4(n, 1.0f));
		
		// Cylinder Geometry

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		
		vertex_indices.push_back(vertices.size() - 3);
		vertex_indices.push_back(vertices.size() - 2);
		vertex_indices.push_back(vertices.size() - 1);

		//this->BuildNormalVisualizationGeometry();
		const float normal_scalar = 0.125f;
		for (int j = 1; j <= 3; ++j)
		{
			normal_vertices.push_back(VertexAttributesP(vertices[vertices.size() - j].position));
			normal_vertices.push_back(VertexAttributesP(vertices[vertices.size() - j].position + vertices[vertices.size() - j].normal * normal_scalar));
			normal_indices.push_back(normal_vertices.size() - 2);
			normal_indices.push_back(normal_vertices.size() - 1);
		}
		
		// Bottom geometry
		vertices.push_back(v3);
		
		vertex_indices.push_back(vertices.size() - 3);		// Note the winding. Question for reader:
		vertex_indices.push_back(vertices.size() - 1);		// Why does this differ from the similar
		vertex_indices.push_back(vertices.size() - 2);		// code a few lines above?

		//this->BuildNormalVisualizationGeometry();
		for (int j = 1; j <= 3; ++j)
		{
			normal_vertices.push_back(VertexAttributesP(vertices[vertices.size() - j].position));
			normal_vertices.push_back(VertexAttributesP(vertices[vertices.size() - j].position + vertices[vertices.size() - j].normal * normal_scalar));
			normal_indices.push_back(normal_vertices.size() - 2);
			normal_indices.push_back(normal_vertices.size() - 1);
		}
	}

	MeshPack * newPack = new MeshPack(vertices, vertex_indices, normal_indices);

	vertices.clear();
	vertex_indices.clear();

	vertices.shrink_to_fit();
	vertex_indices.shrink_to_fit();

	

	return newPack;
}

/*
MeshPack Mesh::Sphere(int stacks, int slices, vec3 color)
{
	//this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	//this->normal_array_handle = this->normal_coordinate_handle = GLuint(-1);
	return MeshPack();
}
*/
