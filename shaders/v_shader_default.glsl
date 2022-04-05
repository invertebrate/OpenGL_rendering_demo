#version 410
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNor;
uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 screen;
out vec2 texCoord;
void main() {
  gl_Position = screen * vec4(aPos, 1.0);
  texCoord = aTex;
}