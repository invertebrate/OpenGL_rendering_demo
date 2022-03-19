#version 410
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNor;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
mat4 screen;
out vec2 texCoord;
out vec3 normal;
out vec3 fragpos;
out mat3 tbn;
void main() {
  mat4 world = translation * scale * rotation * model;
  vec3 T = normalize(vec3(world * vec4(aTangent, 0.0)));
  vec3 B = normalize(vec3(world * vec4(aBitangent, 0.0)));
  vec3 N = normalize(vec3(world * vec4(aNor, 0.0)));
  mat3 TBN = mat3(T, B, N);

  screen = projection * view * world;
  gl_Position = screen * vec4(aPos, 1.0);
  texCoord = aTex;
  normal = aNor;
  fragpos = gl_Position.xyz;
  tbn = TBN;
}