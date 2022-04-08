#version 410
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNor;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
out vec2 texCoord;
out vec3 normal;
out vec4 fragpos;
out mat3 tbn;
out vec4 f_world_pos;

uniform mat4 screen;
uniform mat4 world;
vec3 T;
vec3 B;
vec3 N;
mat3 TBN;
void main() {
  T = normalize(vec3(world * vec4(aTangent, 0.0)));
  B = normalize(vec3(world * vec4(aBitangent, 0.0)));
  N = normalize(vec3(world * vec4(aNor, 0.0)));
  TBN = mat3(T, B, N);

  gl_Position = screen * vec4(aPos, 1.0);
  texCoord = aTex;
  normal = aNor;
  fragpos = vec4(aPos, 1.0);
  f_world_pos = world * vec4(aPos, 1.0);
  tbn = TBN;
}