#version 410

in VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 world_pos;
  vec3 n_light_dir;
  float facing;
  vec3 view_dir;
  mat3 tbn;
}
fs_in;

out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D normalmap;
  sampler2D specularmap;
  float specular_strength;
  float shininess;
};
uniform Material material;
uniform sampler2D shadowmap;
uniform mat4 screen;
uniform mat4 world;
uniform mat4 light_view[16];
// uniform mat4 cube_view[96];  // dir and pos data in these matrices
// uniform mat4 light_view;
uniform mat4 cube_view;

uniform mat4 light_proj;
uniform vec3 light_color;
uniform float light_strength;
uniform vec3 ambient;

float pcf(sampler2D shadowmap, vec3 proj_coords, float current_depth) {
  float shadow;
  float bias = -0.000;
  vec2 texel_size = 1.0 / textureSize(shadowmap, 0);
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      float pcf_depth =
          texture(shadowmap, proj_coords.xy + vec2(x, y) * texel_size).r;
      shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
    }
  }
  if (proj_coords.z > 1.0)
    shadow = 0.0;
  shadow /= 9.0;
  return (shadow);
}

// for directinal light
void main() {
  vec3 r_normal;
  vec3 reflect_dir;

  vec3 specular;
  vec4 diff;
  float light;
  vec4 f_lightspace;
  vec3 proj_coords;
  float closest_depth;
  float current_depth;
  float shadow;

  r_normal =
      normalize(fs_in.tbn *
                (texture(material.normalmap, fs_in.tex_coord).rgb * 2.0 - 1.0));

  f_lightspace =
      light_proj * light_view[0] *
      fs_in.world_pos;  // do this in vertex shader to save computations
  proj_coords = f_lightspace.xyz / f_lightspace.w;
  proj_coords = proj_coords * 0.5 + 0.5;
  closest_depth = texture(shadowmap, proj_coords.xy).r;
  current_depth = proj_coords.z;
  shadow = pcf(shadowmap, proj_coords, current_depth);
  // shadow = current_depth > closest_depth ? 1.0 : 0.0;

  // closest_depth = texture(shadowmap, proj_coords.xy).r;
  // // get depth of current fragment from light’s perspective
  // current_depth = proj_coords.z;
  // // check whether current frag pos is in shadow
  // shadow = current_depth > closest_depth ? 1.0 : 0.0;

  shadow = fs_in.facing >= 0.0 ? 1.0 : shadow;
  // shadow = 0;  //
  reflect_dir = reflect(-fs_in.n_light_dir, r_normal);
  specular = texture(material.specularmap, fs_in.tex_coord).rgb *
             material.specular_strength *
             pow(max(dot(fs_in.view_dir, reflect_dir), 0.0), 32) * light_color;
  diff = texture(material.diffuse, fs_in.tex_coord);
  light = max((-dot((normalize(vec4(r_normal, 1.0))).xyz, fs_in.n_light_dir)),
              0.0) *
          light_strength * 5;
  light = 1;  //
  // fs_in.facing = -1;  //
  FragColor.xyz = (light * diff.xyz + specular * 0 /*<-delete 0*/) *
                  (1.0 - shadow) * max(-fs_in.facing, 0);
  FragColor.xyz = vec3(max(FragColor.x, diff.x * ambient.x),
                       max(FragColor.y, diff.y * ambient.y),
                       max(FragColor.z, diff.z * ambient.z));
}