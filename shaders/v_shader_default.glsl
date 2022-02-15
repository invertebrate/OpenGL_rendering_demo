#version 460
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 position;
mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec2 texCoord;
void main() {
  model = position * scale * rotation;
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  texCoord = aTex;
};