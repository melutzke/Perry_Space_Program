/*	CS 559 Project 2
	Mitchell Lutzke & Steve Krejci

	The Mesh class is used to generate the meshes, and depending upon the
	shape, it places points and generates normals in the form of a
	MeshPack object - which is essentially a glorified attribute-holder.
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

void Mesh::wind (vector<GLuint>& vertex_indices, int stacks, int slices){
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
}


MeshPack* Mesh::Cylinder(mat4 m, float top_radius, float bot_radius, unsigned int stacks, unsigned int slices, glm::vec3 color)
{
	if (slices < 0) slices = 1;

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const float increment =  360.0f / float(slices);

	vector<VertexAttributesPCNT> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	// Here's where we start finding the positions of the points
	// in the mesh.
	//
	// R and S represent an incremental rotational angle so that
	// each point gets placed at the appropriate longitudinal
	// and latitudinal points with respect to the slices and
	// stacks given to the cylinder
	//
	// It should be noticed that this builds the cylinder from the
	// bottom, up.  We start at y=0, and build up to y=1.
	float const R = 1.0f/(float)(stacks-1);
    float const S = 1.0f/(float)(slices-1);
    unsigned int r, s;
	
	for(r = 0; r < stacks; r++){
		for(s = 0; s < slices; s++) {

			// Clever math to find the current stack's radius
			float curr_radius = (top_radius - bot_radius)*(r*R) + bot_radius;

            float const y = r * R;
            float const x = cosf(float(2*M_PI) * s * S);
            float const z = sinf(float(2*M_PI) * s * S);
            vertices.push_back(
				VertexAttributesPCNT(
					vec3(m * vec4(vec3(x * curr_radius, y, z * curr_radius), 1)), 
					color, 
					normalize(vec3(x, y, z)), 
					vec2(0.0f) 
				)
			);
		}
    }


	// pass vertex_indices array by reference, fill it with correctly wound positions
	Mesh::wind(vertex_indices, stacks, slices);

	// calculate true normals
	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i].normal = getNormal(vertices, i, stacks, slices);
	}

	MeshPack * newPack = new MeshPack(vertices, vertex_indices, normal_indices);

	// clear temporary storage
	vertices.clear();
	vertex_indices.clear();

	vertices.shrink_to_fit();
	vertex_indices.shrink_to_fit();

	return newPack;
}

MeshPack * Mesh::Sphere(mat4 m, float radius, unsigned int stacks, unsigned int slices, vec3 color)
{

	if (slices < 0) slices = 1;

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec4 z_axis(0.0f, 0.0f, 1.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);

	vector<VertexAttributesPCNT> vertices;
	vector<GLuint> vertex_indices;
	vector<GLuint> normal_indices;
	vector<VertexAttributesP> normal_vertices;

	float const R = 1.0f/(float)(stacks-1);
    float const S = 1.0f/(float)(slices-1);
    unsigned r, s;
	
	// This is similar to how we construct the cylinder, except now our
	// y-values are scaled by something more complex than the current
	// stack
	for(r = 0; r < stacks; r++){
		for(s = 0; s < slices; s++) {
            float const y = sinf( float(-M_PI_2 + M_PI * r * R) );
            float const x = cosf( float(2*M_PI * s * S) ) * sinf( float(M_PI * r * R) );
            float const z = sinf( float(2*M_PI * s * S) ) * sinf( float(M_PI * r * R) );
			
            vertices.push_back(
				VertexAttributesPCNT(
					vec3(m * vec4(vec3(x * radius, y * radius, z * radius), 1)), // position
					color,														 // color
					normalize(vec3(x, y, z)),									 // normal
					vec2(-float(s*S), -float(r*R))								 // texture coordinate
				)
			);
		}
    }

	//pass vertex_indices array by reference, fill it with correctly wound positions
	Mesh::wind(vertex_indices, stacks, slices);

	for(unsigned int i = slices; i < vertices.size() - slices; i++){
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

	NewNormal += cross( (v5 - myself), (v0 - myself) );
	NewNormal += cross( (v1 - myself), (v2 - myself) );
	NewNormal += cross( (v3 - myself), (v4 - myself) );

	if(NewNormal != vec3(0.0f))
		NewNormal = normalize(NewNormal);
	
	return NewNormal;
}

MeshPack * Mesh::Mars(mat4 m, float radius, string the_file)
{
	int stacks, slices;

	// Parsing input files in C++ code learned via
	// http://www.cplusplus.com/forum/beginner/26993/

	vector<float> vec;
    ifstream file(the_file);
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
					vec.push_back(float(atof(token.c_str())));
				} else if(counter == 0){
					slices = atoi(token.c_str());
				} else if(counter == 1){
					stacks = atoi(token.c_str());
				}
				counter++;
			}
			iss.clear();
		}
	} else {
		cout << "Tried to process an invalid file" << endl;
		return false;
	}
	
	cout << "Read altitudes from file " << the_file << endl;

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec4 z_axis(0.0f, 0.0f, 1.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);

	cout << "Generating initial sphere MeshPack" << endl;

	// This is identical to the generation of a regular sphere, except for the
	// altitude addition, which is scaled by 1/6 to allow for a reasonable-
	// looking Red Planet. The altitudes are added in the following for-loop
	MeshPack * mars = Mesh::Sphere(m, radius, stacks, slices, vec3(1.0f));

	cout << "MeshPack complete, moving points to mars altitudes" << endl;

	// Altitudes, after being read from the file, are added to each vector. 
	// We are careful to aim them in the direction of the sphere's normal
	for(unsigned int i = 0; i < mars->vertices.size(); i++){
		vec3 altitude_addition = vec3( vec[vec.size()-1-i] * 1/6.0f ) * mars->vertices[i].normal;
		mars->vertices[i].position += altitude_addition;
	}
	
	cout << "Altitude shifting completed" << endl;

	cout << "Creating normals" << endl;

	for(unsigned int i = 0; i < mars->vertices.size(); i++){
		mars->vertices[i].normal = getNormal(mars->vertices, i, stacks, slices);
	}

	cout << "Finalized Mars MeshPack" << endl;

	cout << mars->vertices.size() << " vertices created " << " for Mars" << endl;

	cout << "Read in " << vec.size() << " altitude points" << endl;

	return mars;
}