/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
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
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) * 2.0f / 3.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color;
	this->shader_index = 0;
	this->texture;
}

inline int ColorIndex(int i, int slices)
{
	return (i / (slices / 4)) % 2;
}

inline int PreviousSlice(int i, int slices)
{
	return (i == 0) ? slices - 1 : i - 1;
}

void Mars::BuildNormalVisualizationGeometry()
{
	const float normal_scalar = 0.125f;
	for (int j = 1; j <= 3; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position + this->vertices[this->vertices.size() - j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

void Mars::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
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

	this->colors[0] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	this->colors[1] = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	mat4 m;

	MeshPack * New_Cylinder = Mesh::Mars(m, 5.0, the_file);
	New_Cylinder->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
	delete New_Cylinder;
			

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;

	

	this->texture = ILContainer();
	if( ! this->texture.Initialize("mars_full_res.jpg") ) assert(false);
		
	

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
	this->vertices.clear();
	this->shader.TakeDown();
	this->cell.TakeDown();
	super::TakeDown();
}

void Mars::Draw(const ivec2 & size)
{
	assert(false);
}

/*	A note about drawing the normals.

	If you scale this object non-uniformly, drawing the normals will
	not be correct. This is because the normals are being stored for
	visualization as geometry. As such, scaling will not be corrected
	by the normal matrix.
*/

/*	A note about the index arrays.

	In this example, the index arrays are unsigned ints. If you know
	for certain that the number of vertices will be small enough, you
	can change the index array type to shorts or bytes. This will have
	the two fold benefit of using less storage and transferring fewer
	bytes.
*/

void Mars::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time, const int CameraMode)
{
	if (this->GLReturnedError("Mars::Draw - on entry"))
		return;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	this->texture.Bind();
	this->texture.il_texture_handle;

	//modelview = rotate(modelview, time * 10.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();

	glViewport(0, 0, size.x, size.y);

	this->GLReturnedError("Mars::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), CameraMode);
	this->GLReturnedError("Mars::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glPointSize(0.5f);
	
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Mars::Draw - after draw");
	glUseProgram(0);
	

	glDisable(GL_TEXTURE_2D);

	if (this->GLReturnedError("Mars::Draw - on exit"))
		return;
}
