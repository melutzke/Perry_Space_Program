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
#include <stdlib.h>
#include "mesh.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <iostream>
#include <fstream>



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

int Mesh::up (int index, int stacks, int slices) {
	if (index >= (stacks-1)*slices)
		// Top row – there IS no 'up'
		return 0;
	else
		return index + slices;
}
int Mesh::down (int index, int stacks, int slices) {
	if (index < slices)
		// Bottom row – there IS no 'down'
		return 0;
	else
		return index - slices;
}
int Mesh::left (int index, int stacks, int slices) {
	if (index % slices == 0)
		// Left column – we'll want to wrap to the RIGHT
		return index + slices - 2;
	else
		return index -1;
}
int Mesh::right (int index, int stacks, int slices) {
	if (index < slices)
		// Right column – we'll want to wrap to the LEFT
		return index - slices - 2;
	else
		return index + 1;
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

MeshPack * Mesh::Sphere(float radius, unsigned int stacks, unsigned int slices, vec3 coords, vec3 scaleVec, vec3 color)
{

	if (slices < 0) slices = 1;


	mat4 m = mat4(1.0f);
	
	m = translate(m, coords);
	m = scale(m, scaleVec);

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec4 z_axis(0.0f, 0.0f, 1.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);

	vector<VertexAttributesPCN> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	float const R = 1./(float)(stacks-1);
    float const S = 1./(float)(slices-1);
    int r, s;
	

	for(r = 0; r < stacks; r++){
		for(s = 0; s < slices; s++) {

            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
			
            vertices.push_back(VertexAttributesPCN(vec3(m * vec4(vec3(x * radius, y * radius, z * radius), 1)), color, normalize(vec3(-x, -y, -z))) );
		}
    }

	for(int i = 0; i < stacks-1; i++){
		for(int k = 0; k < slices-1; k++){
			vertex_indices.push_back(k+(i*slices));
			vertex_indices.push_back(k+1+(i*slices));
			vertex_indices.push_back(k+((i+1)*slices));

			vertex_indices.push_back(k+((i+1)*slices));
			vertex_indices.push_back(k+((i)*slices)+1);
			vertex_indices.push_back(k+((i+1)*slices)+1);
		}
	}

	for(int i = slices; i < vertices.size() - slices; i++){
		// get face vectors of three of triangles associated with the point (each triangle cannot share a side!)
		// average them
		// ???
		// profit!

		vertices[i].normal = -getNormal(vertices, i, stacks, slices);
	}



	MeshPack * newPack = new MeshPack(vertices, vertex_indices, vertex_indices);
	cout << vertices.size();

	return newPack;

	
	/*
	vector<VertexAttributesPCN> sphere_vertices;
	vector<GLuint> sphere_indices;
	vector<VertexAttributesP> sphere_normals;
	vector<VertexAttributesP> sphere_texcoords;
	vector<VertexAttributesP> normal_vertices;

    float R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    int r, s;

    sphere_vertices.resize(rings * sectors * 3);
    sphere_normals.resize(rings * sectors * 3);
    //sphere_texcoords.resize(rings * sectors * 2);
    std::vector<VertexAttributesPCN>::iterator v = sphere_vertices.begin();
    std::vector<VertexAttributesP>::iterator n = sphere_normals.begin();
    //std::vector<VertexAttributesP>::iterator t = sphere_texcoords.begin();
    for(r = 0; r < rings; r++){
		for(s = 0; s < sectors; s++) {
			R = 1./(float)(rings-1) * rand();
			cout << rand() << endl;
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            //*t++ = s*S;
            //*t++ = r*R;

            *v++ = VertexAttributesPCN(vec3(x * radius, y * radius, z * radius), vec3(1, 0.5, 0), normalize(vec3(-x, -y, -z)));
		}
    }

    sphere_indices.resize(rings * sectors * 4);
    std:vector<GLuint>::iterator i = sphere_indices.begin();
    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
            *i++ = r * sectors + s;
            *i++ = r * sectors + (s+1);
            *i++ = (r+1) * sectors + (s+1);
            *i++ = (r+1) * sectors + s;
    }

	MeshPack * newPack = new MeshPack(sphere_vertices, sphere_indices, sphere_indices);

	return newPack;
	*/
}



glm::vec3 Mesh::getNormal(vector<VertexAttributesPCN> vertices, int i, int stacks, int slices) {
	vec3 NewNormal = vec3(0.0f);
	vec3 myself = vertices[i].position;
	vec3 v0 = vertices[ right(i, stacks, slices)						].position;
	vec3 v1 = vertices[ down(right(i, stacks, slices), stacks, slices)	].position;
	vec3 v2 = vertices[ down(i, stacks, slices)							].position;
	vec3 v3 = vertices[ left(i, stacks, slices)							].position;
	vec3 v4 = vertices[ left(up(i, stacks, slices), stacks, slices)		].position;
	vec3 v5 = vertices[ up(i, stacks, slices)							].position;

	vec3 n0 = cross( (v0 - myself), (v1 - myself) );
	vec3 n1 = cross( (v2 - myself), (v3 - myself) );
	vec3 n2 = cross( (v4 - myself), (v5 - myself) );

	vec3 n3 = cross( (v5 - myself), (v0 - myself) );
	vec3 n4 = cross( (v1 - myself), (v2 - myself) );
	vec3 n5 = cross( (v3 - myself), (v4 - myself) );

	//blended shading
	NewNormal = normalize(n0 + n1 + n2 + n3 + n4 + n5);
	return NewNormal;
}

MeshPack * Mesh::Experimental(float radius, unsigned int stacks, unsigned int slices, vec3 coords)
{

	stacks = 180;
	slices = 360;

	vector<float> vec;
    ifstream    file("mars.txt");
	string      line;
	int counter = 0;
	if (file)
	{
		string token;
		stringstream iss;
		while ( getline(file, line) )
		{
			iss << line;
			while ( getline(iss, token, '\t') )
			{
				if(counter > 1){
					//cout << token << endl;
					vec.push_back(atof(token.c_str()));
				}
				counter++;
			}
			iss.clear();
		}
	}


	if (slices < 0) slices = 1;


	mat4 m = mat4(1.0f);

	m = translate(m, coords);

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec4 z_axis(0.0f, 0.0f, 1.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);

	vector<VertexAttributesPCN> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	float const R = 1./(float)(stacks-1);
    float const S = 1./(float)(slices-1);
    int r, s;
	

	for(r = 0; r < stacks; r++){
		for(s = 0; s < slices; s++) {

            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
			vec3 altitude_addition = vec3(vec[r*stacks+s]* 1/25) * normalize(vec3(x, y, z));
            vertices.push_back(VertexAttributesPCN(vec3(m * vec4(vec3(x * radius, y * radius, z * radius) + altitude_addition, 1)), vec3(1, 0.5, 0), normalize(vec3(-x, -y, -z))) );
		}
    }

	for(int i = 0; i < stacks-1; i++){
		for(int k = 0; k < slices-1; k++){
			vertex_indices.push_back(k+(i*slices));
			vertex_indices.push_back(k+1+(i*slices));
			vertex_indices.push_back(k+((i+1)*slices));

			vertex_indices.push_back(k+((i+1)*slices));
			vertex_indices.push_back(k+((i)*slices)+1);
			vertex_indices.push_back(k+((i+1)*slices)+1);
		}
	}

	for(int i = slices; i < vertices.size() - slices; i++){
		// get face vectors of three of triangles associated with the point (each triangle cannot share a side!)
		// average them
		// ???
		// profit!

		vertices[i].normal = -getNormal(vertices, i, stacks, slices);
	}



	MeshPack * newPack = new MeshPack(vertices, vertex_indices, vertex_indices);
	cout << vertices.size();

	return newPack;
}