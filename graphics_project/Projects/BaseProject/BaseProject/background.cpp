/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#include <iostream>
#include "background.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

Background::Background() : Object()
{
}


bool Background::Initialize()
{
	if (this->GLReturnedError("Background::Initialize - on entry"))
		return false;

	while(this->vertices.size() < 10000){
		// http://hbfs.wordpress.com/2010/10/12/random-points-on-a-sphere-generating-random-sequences-iii/
		// generating random points on a sphere

		float pX, pY, pZ;
		float theta, phi;
		const float pRadius = rand() % 5000 / 100.0f + 50;

		theta = float(2*M_PI*(rand()%1000/1000.0f));
		phi = acos(2*(rand()%1000/1000.0f)-1.0f);

		pX = pRadius * cos(theta) * sin(phi);
		pY = pRadius * sin(theta) * sin(phi);
		pZ = pRadius * cos(phi);


		VertexAttributesPCNT newPoint = VertexAttributesPCNT(
			vec3( pX, pY, pZ ), 
			vec3( float(rand()%1000)/1000.0f ), 
			vec3( 0.0f, 0.0f, -1.0f ),
			vec2( 0.0f )
		);



		this->vertices.push_back(newPoint);
		this->vertex_indices.push_back(vertex_indices.size());
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 1));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (!this->shader.Initialize("solid_shader.vert", "solid_shader.frag"))
		return false;

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Background::TakeDown()
{
	super::TakeDown();
}

void Background::Draw(const ivec2 & size)
{
	assert(false);
}

void Background::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time, const int CameraMode)
{
	glEnable(GL_DEPTH_TEST);
	if (this->GLReturnedError("Background::Draw - on entry"))
		return;

	mat4 mvp = projection * modelview;
	

	shader.Use();
	shader.CommonSetup(0.0f, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(mat4(1.0f)), CameraMode);
	shader.CustomSetup(this->colors);
	glViewport(0, 0, size.x, size.y);
	glBindVertexArray(this->vertex_array_handle);
	glPointSize(0.01f);
	glDrawElements(GL_POINTS , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glUseProgram(0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Background::Draw - on exit"))
		return;
}
