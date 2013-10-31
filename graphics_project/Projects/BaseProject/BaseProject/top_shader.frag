#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
in vec3 position;
in vec3 normal;

uniform mat4 modelview_matrix;

const float shininess = 40.0f;
vec3 light_position = vec3((modelview_matrix) * vec4(0.0, 50.0, -500.0, 1));

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
  return color * n_dot_pos / 500 + color * (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0)) / 2;

  return color * s_dot_n + (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
}

void main()
{
	FragColor = vec4(ads(), 1.0);
}
