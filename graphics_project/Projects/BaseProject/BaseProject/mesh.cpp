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

#pragma warning(disable: 4996)

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
		return index;
	else
		return index + slices;
}
int Mesh::down (int index, int stacks, int slices) {
	if (index < slices)
		// Bottom row – there IS no 'down'
		return index;
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
	if (index % slices == slices - 1)
		// Right column – we'll want to wrap to the LEFT
		return index - (slices - 2);
	else
		return index + 1;
}

MeshPack* Mesh::Cylinder(float top_radius, float bot_radius, unsigned int stacks, unsigned int slices, glm::vec3 coords, glm::vec3 scaleVec, glm::vec3 color, float rotation, bool isWing)
{
	if (slices < 0) slices = 1;

	
	mat4 m(1.0f);
	m = rotate(m, rotation, vec3(0.0f, 1.0f, 0.0f));

	if (isWing) {
		m = rotate(m, -90.0f, vec3(0.0f, 0.0f, 1.0f));
	}

	m = translate(m, coords);
	m = scale(m, scaleVec);

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const float increment =  360.0f / float(slices);

	vector<VertexAttributesPCNT> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	float const R = 1./(float)(stacks-1);
    float const S = 1./(float)(slices-1);
    int r, s;
	

	for(r = 0; r < stacks; r++){
		for(s = 0; s < slices; s++) {

			float curr_radius = (top_radius - bot_radius)*(r*R) + bot_radius;

            float const y = r * R;
            float const x = cos(2*M_PI * s * S);
            float const z = sin(2*M_PI * s * S);

            vertices.push_back(VertexAttributesPCNT(vec3(m * vec4(vec3(x * curr_radius, y, z * curr_radius), 1)), color, normalize(vec3(x, y, z)), vec2(0.0f) ));
		}
    }

	for(int i = 0; i < stacks-1; i++){
		for(int k = 0; k < slices-1; k++){
			vertex_indices.push_back(k+1+(i*slices));
			vertex_indices.push_back(k+(i*slices));
			
			vertex_indices.push_back(k+((i+1)*slices));

			vertex_indices.push_back(k+((i)*slices)+1);
			vertex_indices.push_back(k+((i+1)*slices));
			
			vertex_indices.push_back(k+((i+1)*slices)+1);
		}
	}

	for(int i = 0; i < vertices.size(); i++){
		// get face vectors of three of triangles associated with the point (each triangle cannot share a side!)
		// average them
		// ???
		// profit!

		vertices[i].normal = getNormal(vertices, i, stacks, slices);
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

	vector<VertexAttributesPCNT> vertices;
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
			
            vertices.push_back(VertexAttributesPCNT(vec3(m * vec4(vec3(x * radius, y * radius, z * radius), 1)), color, normalize(vec3(x, y, z)), vec2(0.0f) ));
		}
    }

	for(int i = 0; i < stacks-1; i++){
		for(int k = 0; k < slices-1; k++){
			vertex_indices.push_back(k+1+(i*slices));
			vertex_indices.push_back(k+(i*slices));
			
			vertex_indices.push_back(k+((i+1)*slices));

			vertex_indices.push_back(k+((i)*slices)+1);
			vertex_indices.push_back(k+((i+1)*slices));
			
			vertex_indices.push_back(k+((i+1)*slices)+1);
		}
	}

	for(int i = slices; i < vertices.size() - slices; i++){
		// get face vectors of three of triangles associated with the point (each triangle cannot share a side!)
		// average them
		// ???
		// profit!

		vertices[i].normal = getNormal(vertices, i, stacks, slices);
	}



	MeshPack * newPack = new MeshPack(vertices, vertex_indices, vertex_indices);
	std::cout << "Generated MeshPack w/ " << vertices.size() << " vertices." << endl;

	return newPack;

}



glm::vec3 Mesh::getNormal(vector<VertexAttributesPCNT>& vertices, int i, int stacks, int slices) {
	vec3 NewNormal = vec3(0.0f);
	vec3 myself = vertices[i].position;

	vec3 v0 = vertices[ right(i, stacks, slices) ].position;
	vec3 v1 = vertices[ down(right(i, stacks, slices), stacks, slices) ].position;
	vec3 v2 = vertices[ down(i, stacks, slices) ].position;
	vec3 v3 = vertices[ left(i, stacks, slices) ].position;
	vec3 v4 = vertices[ left(up(i, stacks, slices), stacks, slices) ].position;
	vec3 v5 = vertices[ up(i, stacks, slices) ].position;

	NewNormal += cross( (v0 - myself), (v1 - myself) );
	NewNormal += cross( (v2 - myself), (v3 - myself) );
	NewNormal += cross( (v4 - myself), (v5 - myself) );

	//NewNormal += cross( (v5 - myself), (v0 - myself) );
	//NewNormal += cross( (v1 - myself), (v2 - myself) );
	//NewNormal += cross( (v3 - myself), (v4 - myself) );

	//blended shading
	if(NewNormal != vec3(0.0f))
		NewNormal = normalize(NewNormal);
	
	return NewNormal;
}

MeshPack * Mesh::Experimental(float radius, unsigned int stacks, unsigned int slices, vec3 coords)
{


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
					vec.push_back(atof(token.c_str()));
				} else if(counter == 0){
					slices = atof(token.c_str());
				} else if(counter == 1){
					stacks = atof(token.c_str());
				}
				counter++;
			}
			iss.clear();
		}
	}
	std::cout << "done reading file" << endl;

	if (slices < 0) slices = 1;


	mat4 m = mat4(1.0f);

	m = translate(m, coords);

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec4 z_axis(0.0f, 0.0f, 1.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);

	vector<VertexAttributesPCNT> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	float const R = 1./(float)(stacks-1);
    float const S = 1./(float)(slices-1);
    int r, s;
	
	cout << "Generating points" << endl;
	int counterer = 0;
	for(r = 0; r < stacks; r++){
		for(s = 0; s < slices; s++) {

            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
			vec3 altitude_addition = vec3(vec[vec.size()-1-counterer] * 1/6.0f) * normalize(vec3(x, y, z));
            vertices.push_back(VertexAttributesPCNT(vec3(m * vec4(vec3(x * radius, y * radius, z * radius) + altitude_addition, 1)), vec3(1, 0.5, 0), normalize(vec3(x, y, z)), vec2(-s*S, -r*R) ));
			counterer++;
		}
    }

	cout << "Making indice combinations" << endl;

	for(int i = 0; i < stacks-1; i++){
		for(int k = 0; k < slices-1; k++){
			vertex_indices.push_back(k+1+(i*slices));
			vertex_indices.push_back(k+(i*slices));
			
			vertex_indices.push_back(k+((i+1)*slices));

			vertex_indices.push_back(k+((i)*slices)+1);
			vertex_indices.push_back(k+((i+1)*slices));
			
			vertex_indices.push_back(k+((i+1)*slices)+1);
		}
	}

	cout << "Creating normals" << endl;

	for(int i = 0; i < vertices.size(); i++){
		vertices[i].normal = getNormal(vertices, i, stacks, slices);
	}

	cout << "About to make mesh pack" << endl;

	MeshPack * newPack = new MeshPack(vertices, vertex_indices, vertex_indices);
	cout << vertices.size() << " vertices created";

	cout << "Read in " << vec.size() << " altitude points" << endl;

	return newPack;
}