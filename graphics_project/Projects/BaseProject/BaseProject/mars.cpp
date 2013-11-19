/*	CS 559 Project 2
	Mitchell Lutzke & Steve Krejci

	The Mars class creates our Mars given an input file, as well as generates
	textures for the Red Planet.
*/

#include "Mars.h"
#include "mesh.h"
#include "ilcontainer.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

Mars::Mars() : Object()
{
	this->shader_index = 0;
	this->texture_index = 0;
	this->altitude_scale = 1.0f / 6.0f;
}

inline float RandRange(float x){
	return (float)rand()/((float)RAND_MAX/x);
}

void Mars::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
}

void Mars::StepTexture()
{
	this->texture_index = ++this->texture_index % this->textures.size();
}

bool Mars::Initialize(string the_file)
{
	if (this->GLReturnedError("Mars::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	ifstream file(the_file);
	if(!file){
		cout << "Couldn't find specified altitude file " << the_file << endl;
		return false;
	}

	mat4 m;

	MeshPack * mars_object = Mesh::Mars(m, 5.0, this->altitude_scale, the_file);
	mars_object->addToScene(this->vertices, this->vertex_indices);
	delete mars_object;

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// Mars texture, all 13 megabytes of it
	this->mars_texture = new ILContainer();
	// Mars height map, 15 megs
	this->mars_height = new ILContainer();
	// Perry - Priceless...
	this->perry_texture = new ILContainer();
	// for everything else there's Mastercard

	if( ! this->mars_texture->Initialize("mars_full_res.jpg") )
		return false;
	if( ! this->mars_height->Initialize("mars_height.jpg") )
		return false;
	if( ! this->perry_texture->Initialize("perry_sphere.jpg") )
		return false;

	this->textures.push_back(this->mars_texture);
	this->textures.push_back(this->mars_height);
	this->textures.push_back(this->perry_texture);

	if (!this->shader.Initialize("mars_shader.vert", "mars_shader.frag"))
		return false;

	if (!this->cell.Initialize("mars_shader.vert", "mars_cell.frag"))
		return false;

	this->shaders.push_back(&this->shader);
	this->shaders.push_back(&this->cell);
	
	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Mars::TakeDown()
{
	delete this->mars_texture;
	delete this->perry_texture;
	delete this->mars_height;

	this->vertices.clear();
	this->vertex_indices.clear();

	this->shader.TakeDown();
	this->cell.TakeDown();
	super::TakeDown();
}

void Mars::Draw(const ivec2 & size)
{
	assert(false);
}

void Mars::Draw(const mat4 & projection, mat4 modelview, const vec3 & eye, const ivec2 & size, const float time, const int CameraMode)
{
	if (this->GLReturnedError("Mars::Draw - on entry"))
		return;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->textures[this->texture_index]->Bind();
	this->shaders[this->shader_index]->Use();

	glViewport(0, 0, size.x, size.y);

	this->GLReturnedError("Mars::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), CameraMode, value_ptr(eye));
	this->GLReturnedError("Mars::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Mars::Draw - after draw");
	glUseProgram(0);

	glDisable(GL_TEXTURE_2D);

	if (this->GLReturnedError("Mars::Draw - on exit"))
		return;
}
