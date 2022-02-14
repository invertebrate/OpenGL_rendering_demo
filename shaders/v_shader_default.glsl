#version 460
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
uniform mat4 transform;
uniform mat4 scale;
out vec2 texCoord;
out vec3 norm;
mat4 test;
void main() {
  test = scale * transform;
  gl_Position = test * vec4(aPos, 1.0);
  texCoord = aTex;
};