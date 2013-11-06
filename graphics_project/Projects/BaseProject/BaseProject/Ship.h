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

class Ship : public Object
{
public:
	Ship();
	void RenderSpaceship(int slices);
	void RenderSatellite(int slices);
	bool Initialize(int slices, bool isSpaceship);
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0, const int CameraMan = 0);
	void StepShader();
	void TakeDown();
	//void DrawNormals(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time);
	Shader shader;
	Shader noise;
	Shader rainbow;
	std::vector<Shader *> shaders;

private:
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
	int shader_index;
};
