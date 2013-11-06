#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 tex_coord;

uniform mat4 modelview_matrix;
uniform int CameraMode;
uniform sampler2D diffuse_mat;
uniform vec3 eye;

const float shininess = 40.0f;
// we want light position to change based on what camera mode we are in
// camera modes 0,1,2 we want the light to rotate w/ modelview matrix
vec3 light_position = (CameraMode != 2 && CameraMode != 1 && CameraMode != 0) ? vec3((modelview_matrix) * vec4(200, 50.0, -100.0, 1)) : vec3(200, 50.0, 200.0);

vec3 ads( )
{
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);
  vec3 v = normalize( -position);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);
  float n_dot_pos = max(0.0, dot(n, light_position));

  vec3 frag_color = texture2D(diffuse_mat, tex_coord).rgb;

  return frag_color * n_dot_pos / 250 
	   + frag_color * ( (s_dot_n > 0) ? frag_color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0)) / 2;

}

void main()
{
	FragColor = vec4(ads(), 1.0);
}
