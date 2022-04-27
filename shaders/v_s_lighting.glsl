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
  vec3 n_light_dir;
  vec4 lightspace_pos;
  vec3 view_dir;
  mat3 tbn;
}
vs_out;
// import multiple light view matrices
uniform mat4 camera_view;
uniform mat4 world;
uniform mat4 light_view[16];
uniform mat4 light_proj;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_dir;
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
  vs_out.normal = (world * vec4(attr_nor, 0.0)).xyz;
  vs_out.world_pos = world * vec4(attr_pos, 1.0);

  vs_out.n_light_dir = normalize(
      vs_out.world_pos.xyz - light_pos);  // this needs to change for dir lights
  vs_out.view_dir = normalize(vs_out.world_pos.xyz - view_pos);
  float noffset = 0.01;
  vec4 owpos = vs_out.world_pos + vec4(vs_out.normal * noffset, 0.0);
  vs_out.lightspace_pos = light_proj * light_view[0] * owpos;
  // vs_out.world_pos;  // do this in vertex shader to save computations
  vs_out.tex_coord = attr_tex;

  vs_out.tbn = TBN;
}