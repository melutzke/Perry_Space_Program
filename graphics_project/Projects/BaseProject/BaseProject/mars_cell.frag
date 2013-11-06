#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 tex_coord;

uniform mat4 modelview_matrix;
uniform int CameraMode;
uniform sampler2D diffuse_mat;

const float shininess = 40.0f;
// we want light position to change based on what camera mode we are in
// camera modes 0,1,2 we want the light to rotate w/ modelview matrix
vec3 light_position = (CameraMode != 2 && CameraMode != 1 && CameraMode != 0) ? vec3((modelview_matrix) * vec4(200, 50.0, -100.0, 1)) : vec3(200, 50.0, 200.0);

void main ()
{
         
	// Material Color:
	vec4 color0 = vec4(0.8, 0.0, 0.0, 1.0);
         
	// Silhouette Color:
	vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
         
	// Specular Color:
	vec4 color2 = vec4(0.8, 0.0, 0.0, 1.0);
            
	// Lighting
	vec3 eyePos = vec3(vec4(0.0,0.0,0.0, 1));
	vec3 lightPos = light_position;
	vec3 Normal = normal;
	vec3 EyeVert = normalize(eyePos - position);
	vec3 LightVert = normalize(lightPos - position);
	vec3 EyeLight = normalize(LightVert+EyeVert);

	// Simple Silhouette
	float sil = max(dot(Normal,EyeVert), 0.0);

	if (sil < 0.3) {
		FragColor = color1;
	} else {
	   FragColor = color0;
	   float spec = pow(max(dot(Normal,EyeLight),0.0), shininess);  // Specular part
	   if (spec < 0.2){
			FragColor *= 0.8;
	   } else {
			FragColor = color2;
	   }
	   // Diffuse part
	   float diffuse = max(dot(Normal,LightVert),0.0);
	   if (diffuse < 0.5) FragColor *=0.8;
	}
}