#version 330 core

in vec3 vertex;
uniform float scale;
uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

void main()  {
  gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
