#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 tex_coord;
in vec3 addition;

uniform mat4 modelview_matrix;
uniform int CameraMode;

const float shininess = 40.0f;
vec3 light_position = (CameraMode != 4 && CameraMode != 2 && CameraMode != 1 && CameraMode != 0) ? vec3((modelview_matrix) * vec4(200, 50.0, -100.0, 1)) : vec3(200, 50.0, 200.0);

vec3 genColor(vec3 position) {
	position = vec3(inverse(modelview_matrix) * vec4(position, 1));
	float red =   (position.x + 1.0f) / 2.0f;
	float green = (position.y + 1.0f) / 2.0f;
	float blue =  (position.z + 1.0f) / 2.0f;

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
  float n_dot_pos = max(0.0, dot(n, normalize(light_position)));

  return genColor(position) * n_dot_pos + genColor(position) * (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0)) / 2;
}

void main()
{
	FragColor = vec4(ads(), 1.0);
}
