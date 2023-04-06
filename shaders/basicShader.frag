#version 330 core

out vec4 FragColor;
uniform vec4 col;

void main() {
    FragColor = vec4(col);
}
