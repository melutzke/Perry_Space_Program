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

		mat4 m(1.0f);
		m = scale(m, vec3(1.0f, 4.0f, 1.0f));

        MeshPack * New_Sphere = Mesh::Sphere(m, 1.0, slices, slices, vec3(0.0f, 0.0f, 1.0f));
        New_Sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
        delete New_Sphere;
                        
        // Build Mars spheres for ship
        for (float x = -2.0f; x <= 2.0f; x+=4.0f) {
                for (float z = -2.0f; z <= 2.0f; z+=4.0f) {
						m = mat4(1.0f);
						m = translate(m, vec3(x, 0.0f, z));
						m = scale(m, vec3(0.5f));

                        MeshPack * Ship_sphere = Mesh::Sphere(m, 1.0, slices, slices, vec3(0.0f, 0.0f, 1.0f));
                        Ship_sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete Ship_sphere;
                }
        }
                        
        // Build bottom spheres for sihp
        for (float x = -2.0f; x <= 2.0f; x+=4.0f) {
                for (float z = -2.0f; z <= 2.0f; z+=4.0f) {
						m = mat4(1.0f);
						m = translate(m, vec3(x, -4.0f, z));
						m = scale(m, vec3(0.5f));

                        MeshPack * bot_sphere = Mesh::Sphere(m, 1.0, slices, slices, vec3(1.0f, 0.0f, 0.0f));
                        bot_sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
                        delete bot_sphere;
                }
        }

        // Build rocket booster cylinders
        float rotation = -45.0f;

        for (float x = -2.0f; x <= 2.0f; x+=4.0f) {
                for (float z = -2.0f; z <= 2.0f; z+=4.0f) {
						
						// coords, scale, ---, rotation
						m = mat4(1.0f);
						m = translate(m, vec3(x, -4.0f, z));
						m = scale(m, vec3(0.5f, 4.0f, 0.5f));

                        MeshPack * cyl = Mesh::Cylinder(m, 1.0f, 1.0f, slices, slices, vec3(0.0f, 0.0f, 1.0f));
                        

						m = mat4(1.0f);
						m = translate(m, vec3(x, -4.0f, z));
						m = scale(m, vec3(0.5f, 1.0f, 0.5f));

                        // Fancy fans
                        MeshPack * cyl2 = Mesh::Cylinder(m, 1.0f, 2.0f, slices, slices, vec3(0.0f, 0.0f, 1.0f));
                        
						m = mat4(1.0f);
						m = rotate(m, rotation, vec3(0.0f, 1.0f, 0.0f));
						m = rotate(m, -90.0f, vec3(0.0f, 0.0f, 1.0f));
						m = translate(m, vec3(1.5f, 0.5f, 0.0f));
						m = scale(m, vec3(1.5f, 2.5f, 0.5f));

                        // Wings
                        MeshPack * wing = Mesh::Cylinder(m, 0.5f, 1.0f, slices, slices, vec3(0.0f, 0.0f, 1.0f));
                        
						cyl->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
						cyl2->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
						wing->addToScene(this->vertices, this->vertex_indices, this->normal_indices);

						delete cyl;
						delete cyl2;
                        delete wing;

                        rotation += 90.0f;
				}
		}
           
}

void Ship::RenderSatellite(int slices) {
		// Reminder of Mesh functions:
		// MeshPack * Mesh::Sphere(float radius, unsigned int stacks, unsigned int slices, vec3 coords, vec3 scaleVec, vec3 color)
		// MeshPack* Mesh::Cylinder(float Mars_radius, float bot_radius, unsigned int stacks, unsigned int slices, glm::vec3 coords, glm::vec3 scaleVec, glm::vec3 color, float rotation, bool isWing)

		mat4 m(1.0f);
		m = scale(m, vec3(2.25f));

        // Build main body of spaceship
        MeshPack * body_sphere = Mesh::Sphere(m, 1.0, slices, slices, vec3(0.85f));
        body_sphere->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
        delete body_sphere;

        // Build rocket booster cylinders
        float rotation = 0.0f;

		m = mat4(1.0f);
              
        for (float x = -4.0f; x <= 4.0f; x+=8.0f) {
                float z = 0.0f;
				m = mat4(1.0f);
				m = translate(m, vec3(x, 0.0f, z));
				m = scale(m, vec3(0.4f, 6.0f, 2.5f));

				//slices/stacks set to 6 to get the hexagonal tie fighter wing shape
                MeshPack * panel = Mesh::Sphere(m, 1.0f, 6, 6, vec3(0.9f));

                // Wings
				m = mat4(1.0f);
				m = rotate(m, rotation, vec3(0.0f, 1.0f, 0.0f));
				m = rotate(m, -90.0f, vec3(0.0f, 0.0f, 1.0f));
				m = translate(m, vec3(0.0f, 1.25f, 0.0f));
				m = scale(m, vec3(1.0f, 2.75f, 1.0f));

                MeshPack * wing = Mesh::Cylinder(m, 0.5f, 1.25f, slices, slices, vec3(0.85f));

                wing->addToScene(this->vertices, this->vertex_indices, this->normal_indices);
				panel->addToScene(this->vertices, this->vertex_indices, this->normal_indices);

                delete panel;
				delete wing;

                rotation += 180.0f;
        }
}


bool Ship::Initialize(int slices, bool isSpaceship)
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
		

	if (isSpaceship) {
		cout << endl << "PRE_RENDERSPACESHIP" << endl;
		RenderSpaceship(slices);
		cout << endl << "POST_RENDERSPACESHIP" << endl;
	} else {
		cout << endl << "PRE_RENDERSATELLITE" << endl;
		RenderSatellite(slices);
		cout << endl << "POST_RENDERSATELLITE" << endl;
	}
	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 1));	// Same
	
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

	if (!this->shader.Initialize("ship_ads.vert", "ship_ads.frag"))
		return false;

	if (!this->noise.Initialize("ship_noise.vert", "ship_noise.frag"))
		return false;

	this->shaders.push_back(&this->shader);
	this->shaders.push_back(&this->noise);
	//this->shaders.push_back(&this->stripes_model_space);

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Ship::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->noise.TakeDown();
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

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();

	glViewport(0, 0, size.x, size.y);

	this->GLReturnedError("Ship::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), CameraMode);
	this->GLReturnedError("Ship::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Ship::Draw - after draw");
	glUseProgram(0);

	if (this->GLReturnedError("Ship::Draw - on exit"))
		return;
}
