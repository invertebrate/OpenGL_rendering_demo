#version 410
layout(location = 0) in vec3 attr_pos;
layout(location = 1) in vec2 attr_tex;
layout(location = 2) in vec3 attr_nor;
layout(location = 3) in vec3 attr_tangent;
layout(location = 4) in vec3 attr_bitangent;

#define MAX_LIGHTS 12

out VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 world_pos;
  vec3 p_light_dir[MAX_LIGHTS];
  vec3 d_light_dir[MAX_LIGHTS];
  vec3 light_to_pos[MAX_LIGHTS];
  float facing[MAX_LIGHTS];
  vec4 lightspace_pos;
  vec3 view_dir;
  mat3 tbn;
}
vs_out;

uniform mat4 camera_view;
uniform mat4 world;
uniform mat4 light_view[16];
uniform mat4 light_proj;
uniform vec3 p_light_pos[MAX_LIGHTS];
uniform vec3 d_light_pos[MAX_LIGHTS];
uniform vec3 d_light_dir[MAX_LIGHTS];
uniform int p_light_count;
uniform vec3 view_pos;

vec3 T;
vec3 B;
vec3 N;
mat3 TBN;

void set_p_light_out_value(int i) {
  (vs_out.p_light_dir)[i] = normalize(vs_out.world_pos.xyz - p_light_pos[i]);
  (vs_out.light_to_pos)[i] = -(vs_out.world_pos.xyz - p_light_pos[i]);
  (vs_out.facing)[i] =
      dot(normalize(vs_out.normal), normalize((vs_out.light_to_pos)[i]));
}

void set_p_light_out_values() {
  set_p_light_out_value(0);
  set_p_light_out_value(1);
  set_p_light_out_value(2);
  set_p_light_out_value(3);
  set_p_light_out_value(4);
  set_p_light_out_value(5);
  set_p_light_out_value(6);
  set_p_light_out_value(7);
  set_p_light_out_value(8);
  set_p_light_out_value(9);
  set_p_light_out_value(10);
  set_p_light_out_value(11);
}

void main() {
  T = normalize(vec3(world * vec4(attr_tangent, 0.0)));
  B = normalize(vec3(world * vec4(attr_bitangent, 0.0)));
  N = normalize(vec3(world * vec4(attr_nor, 0.0)));
  TBN = mat3(T, B, N);

  gl_Position = camera_view * vec4(attr_pos, 1.0);
  vs_out.normal = (world * vec4(attr_nor, 0.0)).xyz;
  vs_out.world_pos = world * vec4(attr_pos, 1.0);

  set_p_light_out_values();

  vs_out.view_dir = normalize(vs_out.world_pos.xyz - view_pos);
  float noffset = 0.01;
  vec4 owpos = vs_out.world_pos + vec4(vs_out.normal * noffset, 0.0);
  vs_out.lightspace_pos = light_proj * light_view[0] * owpos;
  vs_out.tex_coord = attr_tex;

  vs_out.tbn = TBN;
}