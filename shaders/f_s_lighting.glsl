#version 410

in VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 world_pos;
  vec3 n_light_dir;
  vec4 lightspace_pos;
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
uniform vec3 light_pos;

float pcf(sampler2D shadowmap,
          vec3 proj_coords,
          float current_depth,
          float facing) {
  float shadow;
  float bias = 0.000001;
  int samples = 3;
  bias = max(bias * (1.0 - facing), bias / 5);
  // bias = 0;

  vec2 texel_size = 1.0 / textureSize(shadowmap, 0);
  for (int x = -samples; x <= samples; ++x) {
    for (int y = -samples; y <= samples; ++y) {
      float pcf_depth =
          texture(shadowmap, proj_coords.xy + vec2(x, y) * texel_size).r;
      shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
    }
  }
  if (proj_coords.z > 1.0)
    shadow = 0.0;
  shadow /= ((1 + 2 * samples) * (1 + 2 * samples));
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
  float attenuation;
  float facing;
  float roughness = 0.15;

  r_normal =
      normalize(fs_in.tbn *
                (texture(material.normalmap, fs_in.tex_coord).rgb * 2.0 - 1.0));
  f_lightspace = fs_in.lightspace_pos;
  proj_coords = f_lightspace.xyz / f_lightspace.w;
  proj_coords = proj_coords * 0.5 + 0.5;
  closest_depth = texture(shadowmap, proj_coords.xy).r;
  current_depth = proj_coords.z;
  facing = -dot(normalize(fs_in.normal), fs_in.n_light_dir);
  shadow = pcf(shadowmap, proj_coords, current_depth, abs(facing));
  facing = max(facing, roughness);
  // shadow = current_depth > closest_depth ? 1.0 : 0.0;
  shadow = facing <= 0.0 ? 1.0 : shadow;
  reflect_dir = reflect(-fs_in.n_light_dir, r_normal);
  specular = texture(material.specularmap, fs_in.tex_coord).rgb *
             material.specular_strength * 5 *
             pow(max(dot(fs_in.view_dir, reflect_dir), 0.0), 32) * light_color;
  diff = texture(material.diffuse, fs_in.tex_coord);
  light = max((-dot((normalize(vec4(r_normal, 1.0))).xyz, fs_in.n_light_dir)),
              0.0) *
          light_strength * 15;
  attenuation = 1 / length(fs_in.world_pos.xyz - light_pos);
  FragColor.xyz =
      (light * diff.xyz + specular) * (1.0 - shadow) * facing * attenuation;
  FragColor.xyz = vec3(max(FragColor.x, diff.x * ambient.x / 2),
                       max(FragColor.y, diff.y * ambient.y / 2),
                       max(FragColor.z, diff.z * ambient.z / 2));
}