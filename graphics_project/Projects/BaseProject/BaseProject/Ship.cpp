/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#include <iostream>
#include "Ship.h"
#include "mesh.h"

using namespace std;
using namespace glm;

Ship::Ship() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) * 2.0f / 3.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color;
	this->shader_index = 0;
}

inline int ColorIndex(int i, int slices)
{
	return (i / (slices / 4)) % 2;
}

inline int PreviousSlice(int i, int slices)
{
	return (i == 0) ? slices - 1 : i - 1;
}

void Ship::BuildNormalVisualizationGeometry()
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

void Ship::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
}

void Ship::RenderSpaceship(int slices) {
        // Build main body of spaceship
        MeshPack * New_Sphere = Mesh::Sphere(1.0, slices, slices, vec3(0.0f), vec3(1.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f));
        New_Sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
        delete New_Sphere;
                        
        // Build Ship spheres for ship
        for (float x = -2.0f; x <= 2.0f; x+=4.0f) {
                for (float z = -2.0f; z <= 2.0f; z+=4.0f) {
                        MeshPack * Ship_sphere = Mesh::Sphere(1.0, slices, slices, vec3(x, 0.0f, z), vec3(0.5f), vec3(0.0f, 0.0f, 1.0f));
                        Ship_sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete Ship_sphere;
                }
        }
                        
        // Build bottom spheres for sihp
        for (float x = -2.0f; x <= 2.0f; x+=4.0f) {
                for (float z = -2.0f; z <= 2.0f; z+=4.0f) {
                        MeshPack * bot_sphere = Mesh::Sphere(1.0, slices, slices, vec3(x, -4.0f, z), vec3(0.5f), vec3(1.0f, 0.0f, 0.0f));
                        bot_sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete bot_sphere;
                }
        }

        // Build rocket booster cylinders
        float rotation = -45.0f;
                        
        for (float x = -2.0f; x <= 2.0f; x+=4.0f) {
                for (float z = -2.0f; z <= 2.0f; z+=4.0f) {

                        MeshPack * cyl = Mesh::Cylinder(1.0f, 1.0f, slices, slices, vec3(x, -4.0f, z), vec3(0.5f, 4.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f), 0.0f, false);
                        cyl->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete cyl;

                        // Fancy fans
                        MeshPack * cyl2 = Mesh::Cylinder(1.0f, 2.0f, slices, slices, vec3(x, -4.0f, z), vec3(0.5f, 1.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f), 0.0f, false);
                        cyl2->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete cyl2;

                        // Wings
                        MeshPack * wing = Mesh::Cylinder(0.5f, 1.0f, slices, slices, vec3(1.5f, 0.5f, 0.0f), vec3(1.5f, 2.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), rotation, true);
                        wing->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete wing;

                        rotation += 90.0f;
                }
        }
}

bool Ship::Initialize(int slices)
{
	if (this->GLReturnedError("Ship::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (slices <= 0)
		slices = 1;

	this->colors[0] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	this->colors[1] = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	mat4 m;

	const vec3 n = normalize(vec3(1.0f, 0.0f, 0.0f)); // DA FUQ...
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const float increment =  360.0f / float(slices);
		
	/*	for each slice:
			compute Ship triangle geometry
			compute vectors to visualize normals for Ship triangle (BuildNormalVisualizationGeometry())
			compute bottom triangle geometry
			compute vectors to visualize normals for bottom triangle (BuildNormalVisualizationGeometry())
	*/

	//MeshPack * New_Cylinder = Mesh::Cylinder(slices, vec3(0.5f, 0.1f, 1.0f));
	//for (float x = -1.0f; x <= 1.0f; x+=2.0f) {
	//	for (float z = -1.0f; z <= 1.0f; z+=2.0f) {	
			//MeshPack * New_Cylinder = Mesh::Experimental(1, slices, slices, vec3(x, 0.0f, z));

			//MeshPack * New_Cylinder = Mesh::Experimental(5.0, slices, slices, vec3(0.0f, 0.0f, 0.0f));
			//New_Cylinder->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
			//delete New_Cylinder;
			
			

	//	}
	//}
	cout << endl << "PRE_RENDERSPACESHIP" << endl;
	RenderSpaceship(slices);
	cout << endl << "POST_RENDER" << endl;

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	/*	The VertexAttributesPCN class stores vertex attributes: position, color and normal in that order.

		Vertex attributes are stored in an interleaved manner aiding speed of vertex processing.
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
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

	if (!this->shader.Initialize("top_shader.vert", "top_shader.frag"))
		return false;

	//if (!this->shader.Initialize("mars_shader.vert", "mars_shader.frag"))
	//	return false;

	//if (!this->solid_color.Initialize("solid_shader.vert", "solid_shader.frag"))
	//	return false;

	//if (!this->stripes_model_space.Initialize("stripe_model_space.vert", "stripe_model_space.frag"))
	//	return false;

	this->shaders.push_back(&this->shader);
	this->shaders.push_back(&this->solid_color);
	this->shaders.push_back(&this->stripes_model_space);

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Ship::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Ship::Draw(const ivec2 & size)
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

void Ship::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time, const int CameraMode)
{
	if (this->GLReturnedError("Ship::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	//modelview = rotate(modelview, time * 10.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();

	glViewport(0, 0, size.x, size.y);

	this->GLReturnedError("Ship::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), CameraMode);
	this->GLReturnedError("Ship::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glPointSize(0.5f);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Ship::Draw - after draw");
	glUseProgram(0);

	if (this->GLReturnedError("Ship::Draw - on exit"))
		return;
}