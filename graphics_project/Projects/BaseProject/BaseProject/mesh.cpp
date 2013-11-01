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
	if (index < slices)
		// Right column – we'll want to wrap to the LEFT
		return index - slices - 2;
	else
		return index + 1;
}

MeshPack* Mesh::Cylinder(float top_radius, float bot_radius, unsigned int stacks, unsigned int slices, glm::vec3 coords, glm::vec3 scaleVec, glm::vec3 color, float rotation, bool isWing)
{
	if (slices < 0) slices = 1;
	
	slices *= 4;
	
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

	vector<VertexAttributesPCN> vertices;
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
			
            vertices.push_back(VertexAttributesPCN(vec3(m * vec4(vec3(x * curr_radius, y, z * curr_radius), 1)), color, normalize(vec3(x, y, z))) );
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



glm::vec3 Mesh::getNormal(vector<VertexAttributesPCN>& vertices, int i, int stacks, int slices) {
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

	//blended shading
	if(NewNormal != vec3(0.0f))
		NewNormal = normalize(NewNormal);
	
	return NewNormal;
}



GLuint Mesh::loadBMP_custom(const char * imagepath){

	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file){
		printf("Image could not be opened\n"); 
		return 0;
	}
	if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    		printf("Not a correct BMP file\n");
    		return false;
	}
	if ( header[0]!='B' || header[1]!='M' ){
    		printf("Not a correct BMP file\n");
    		return 0;
	}
	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way
	// Create a buffer
	data = new unsigned char [imageSize];
 
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
 
	//Everything is in memory now, the file can be closed
	fclose(file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

MeshPack * Mesh::Experimental(float radius, unsigned int stacks, unsigned int slices, vec3 coords)
{

	GLuint Texture = loadBMP_custom("./mars_texture.bmp");

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

	vector<VertexAttributesPCN> vertices;
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
            vertices.push_back(VertexAttributesPCN(vec3(m * vec4(vec3(x * radius, y * radius, z * radius) + altitude_addition, 1)), vec3(1, 0.5, 0), normalize(vec3(x, y, z))) );
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

	for(int i = slices; i < vertices.size() - slices; i++){
		vertices[i].normal = getNormal(vertices, i, stacks, slices);
	}

	cout << "About to make mesh pack" << endl;

	MeshPack * newPack = new MeshPack(vertices, vertex_indices, vertex_indices);
	cout << vertices.size() << " vertices created";

	cout << "Read in " << vec.size() << " altitude points" << endl;

	return newPack;
}