#version 410
layout(location = 0) in vec3 attr_pos;
layout(location = 1) in vec2 attr_tex;
layout(location = 2) in vec3 attr_nor;
layout(location = 3) in vec3 attr_tangent;
layout(location = 4) in vec3 attr_bitangent;
out VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 world_pos;
  mat3 tbn;
}
vs_out;
// import multiple light view matrices
uniform mat4 camera_view;
uniform mat4 world;
uniform mat4 light_view[16];
uniform mat4 light_proj;
vec3 T;
vec3 B;
vec3 N;
mat3 TBN;
void main() {
  T = normalize(vec3(world * vec4(attr_tangent, 0.0)));
  B = normalize(vec3(world * vec4(attr_bitangent, 0.0)));
  N = normalize(vec3(world * vec4(attr_nor, 0.0)));
  TBN = mat3(T, B, N);

  // gl_Position = light_proj * light_view[0] * vec4(attr_pos, 1.0);
  gl_Position = camera_view * vec4(attr_pos, 1.0);
  vs_out.tex_coord = attr_tex;
  vs_out.normal = (world * vec4(attr_nor, 0.0)).xyz;
  vs_out.world_pos = world * vec4(attr_pos, 1.0);
  vs_out.tbn = TBN;
}