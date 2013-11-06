#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 tex_coord;

uniform mat4 modelview_matrix;
uniform int CameraMode;

const float shininess = 40.0f;
// we want light position to change based on what camera mode we are in
// camera modes 0,1,2 we want the light to rotate w/ modelview matrix
vec3 light_position = (CameraMode != 2 && CameraMode != 1 && CameraMode != 0) ? vec3((modelview_matrix) * vec4(200, 50.0, -100.0, 1)) : vec3(200, 50.0, 200.0);

vec3 genColor(vec3 position) {
	float maxVal = abs(position.x);

	if (abs(position.y) > maxVal)	maxVal = abs(position.y);

	if (abs(position.z) > maxVal)	maxVal = abs(position.z);

	vec3 normalizedPosition = vec3(position.x / maxVal, position.y / maxVal, position.z/maxVal);

	normalizedPosition = vec3(inverse(modelview_matrix) * vec4(normalizedPosition, 1));
	float red = (normalizedPosition.x + 1.0f)/2.0f;
	float green = (normalizedPosition.y + 1.0f)/2.0f;
	float blue = (normalizedPosition.z + 1.0f)/2.0f;

	return vec3(red, green, blue);
}

vec3 ads( )
{
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);
  vec3 v = normalize(-position);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);
  float n_dot_pos = max(0.0, dot(n, light_position));

  vec3 frag_color = genColor(position);

  return frag_color * n_dot_pos / 250 
	   + frag_color * ( (s_dot_n > 0) ? frag_color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0)) / 2;

}

void main()
{
	FragColor = vec4(ads(), 1.0);
}





void main2 (void)
{
         
// Material Color:
vec4 color0 = vec4(0.8, 0.0, 0.0, 1.0);
         
// Silhouette Color:
vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
         
// Specular Color:
vec4 color2 = vec4(0.8, 0.0, 0.0, 1.0);
            
// Lighting
vec3 eyePos = vec3((modelview_matrix) * vec4(0.0,0.0,5.0, 1));
vec3 lightPos = light_position;
vec3 Normal = normal;
vec3 EyeVert = normalize(eyePos - position);
vec3 LightVert = normalize(lightPos - position);
vec3 EyeLight = normalize(LightVert+EyeVert);
// Simple Silhouette
float sil = max(dot(Normal,EyeVert), 0.0);
if (sil < 0.3) FragColor = color1;
else 
  {
   FragColor = color0;
   // Specular part
   float spec = pow(max(dot(Normal,EyeLight),0.0), shininess);
   if (spec < 0.2) FragColor *= 0.8;
   else FragColor = color2;
   // Diffuse part
   float diffuse = max(dot(Normal,LightVert),0.0);
   if (diffuse < 0.5) FragColor *=0.8;
   }
}