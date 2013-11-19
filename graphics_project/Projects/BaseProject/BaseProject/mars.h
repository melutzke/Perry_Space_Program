/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"
#include "ilcontainer.h"

class Mars : public Object
{
public:
	Mars();
	void RenderSpaceship(int slices);
	bool Initialize(std::string the_file);
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::vec3 & eye, const glm::ivec2 & size, const float time = 0, const int CameraMan = 0);
	void StepShader();
	void StepTexture();
	void TakeDown();
	Shader shader;
	Shader cell;
	ILContainer * mars_texture;
	ILContainer * mars_height;
	ILContainer * perry_texture;
	std::vector<Shader *> shaders;
	std::vector<ILContainer *> textures;

private:
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
	int shader_index;
	int texture_index;
	float altitude_scale;
};
