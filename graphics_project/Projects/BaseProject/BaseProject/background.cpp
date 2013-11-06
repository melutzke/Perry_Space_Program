/*	CS 559 Project 2
	Mitchell Lutzke & Steve Krejci

	The Background class is used to generate the star field.
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

// 0 to x in floats
inline float RandRange(float x){
	return (float)rand()/((float)RAND_MAX/x);
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
		const float pRadius = RandRange(50.0f) + 50.0f;

		theta = float(2*M_PI*(RandRange(1.0f)));
		phi = acos(2*(RandRange(1.0f))-1.0f);

		pX = pRadius * cos(theta) * sin(phi);
		pY = pRadius * sin(theta) * sin(phi);
		pZ = pRadius * cos(phi);

		// What colors are stars?
		// http://www.vendian.org/mncharity/dir3/starcolor/

		vec3 colors [7] = {
			vec3(155.0f, 176.0f, 255.0f),
			vec3(170.0f, 191.0f, 255.0f), 
			vec3(202.0f, 215.0f, 255.0f), 
			vec3(248.0f, 247.0f, 255.0f), 
			vec3(255.0f, 244.0f, 234.0f), 
			vec3(255.0f, 210.0f, 161.0f), 
			vec3(255.0f, 204.0f, 111.0f)
		};

		int color_choice = rand()%30;
		vec3 color = (color_choice < 7) ? colors[color_choice] : vec3(255.0f);
		color /= vec3(255.0f);
		color *= vec3(RandRange(1.0f));

		VertexAttributesPCNT newPoint = VertexAttributesPCNT(
			vec3( pX, pY, pZ ), 
			color, 
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

void Background::Draw(const mat4 & projection, mat4 modelview, const vec3 & eye, const ivec2 & size, const float time, const int CameraMode)
{
	glEnable(GL_DEPTH_TEST);
	if (this->GLReturnedError("Background::Draw - on entry"))
		return;

	mat4 mvp = projection * modelview;
	

	shader.Use();
	shader.CommonSetup(0.0f, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(mat4(1.0f)), CameraMode, value_ptr(eye));
	glViewport(0, 0, size.x, size.y);
	glBindVertexArray(this->vertex_array_handle);
	glPointSize(0.1f);
	glDrawElements(GL_POINTS , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glUseProgram(0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Background::Draw - on exit"))
		return;
}
