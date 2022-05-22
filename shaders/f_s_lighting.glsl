#version 410

#define MAX_LIGHTS 12
#define LIGHT_CUTOFF 0.05

in VS_OUT {
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

uniform samplerCube shadow_array[MAX_LIGHTS];

uniform mat4 screen;
uniform mat4 world;
uniform mat4 camera_view;
uniform mat4 light_view[16];
uniform float far_plane;

uniform mat4 light_proj;
uniform vec3 p_light_color[MAX_LIGHTS];
uniform float p_light_strength[MAX_LIGHTS];
uniform vec3 ambient;
uniform vec3 p_light_pos[MAX_LIGHTS];
uniform float p_light_falloff[MAX_LIGHTS];
uniform int p_light_count;

float pcf(sampler2D shadowmap,
          vec3 proj_coords,
          float current_depth,
          float facing) {
  float shadow;
  float bias = 0.01;
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

float pcf_cube(samplerCube shadow_cubemap, vec3 light_to_pos, float facing) {
  float shadow = 0.0;
  float bias = 0.3;
  bias = max(bias * (1.0 - facing), bias / 5);
  float samples = 2.0;
  float offset = 0.1;
  float current_depth = length(light_to_pos);
  for (float x = -offset; x < offset; x += offset / (samples * 0.5)) {
    for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
      for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {
        float closest_depth =
            texture(shadow_cubemap, light_to_pos + vec3(x, y, z)).r;
        closest_depth *= far_plane;
        if (current_depth - bias > closest_depth)
          shadow += 1.0;
      }
    }
  }
  shadow /= (samples * samples * samples);
  return (shadow);
}
float get_shadow(samplerCube shadow_cubemap[MAX_LIGHTS], int index) {
  float shadow;
  shadow = pcf_cube(shadow_cubemap[index], (fs_in.light_to_pos)[index],
                    abs((fs_in.facing)[index]));
  return (fs_in.facing[index] <= 0.0 ? 1.0 : shadow);
}

float[MAX_LIGHTS] get_point_shadows() {
  float[MAX_LIGHTS] shadows;
  for (int i = 0; i < p_light_count; i++) {
    shadows[i] = get_shadow(shadow_array, i);
  }
  return (shadows);
}

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

  float roughness = 0.15;

  r_normal =
      normalize(fs_in.tbn *
                (texture(material.normalmap, fs_in.tex_coord).rgb * 2.0 - 1.0));
  diff = texture(material.diffuse, fs_in.tex_coord);

  vec3 values[MAX_LIGHTS];
  vec3 sum = vec3(0);
  float shadows[MAX_LIGHTS] = get_point_shadows();
  float dist;
  float exposure = 0.18;

  exposure = 1 / (p_light_count * exposure);
  for (int i = 0; i < p_light_count; i++) {
    dist = length(fs_in.world_pos.xyz - p_light_pos[i]);
    float attenuation = min(1.0, 2 / dist);
    attenuation = pow(attenuation, p_light_falloff[i]);
    if (attenuation < LIGHT_CUTOFF) {
      values[i] = vec3(0);
      continue;
    }
    reflect_dir = reflect(-fs_in.p_light_dir[i], r_normal);
    specular = vec3(texture(material.specularmap, fs_in.tex_coord).g) *
               material.specular_strength *
               pow(max(dot(fs_in.view_dir, reflect_dir), 0.0), 32) *
               p_light_color[i];
    light =
        max((-dot((normalize(vec4(r_normal, 1.0))).xyz, fs_in.p_light_dir[i])),
            0.0) *
        p_light_strength[i] * 15;
    FragColor.xyz = p_light_color[i] * (light * diff.xyz + specular) *
                    (1.0 - shadows[i]) * fs_in.facing[i] * attenuation;
    values[i] = FragColor.xyz;
    sum += FragColor.xyz;
  }
  sum *= exposure;
  FragColor.xyz = vec3(max(sum.x, diff.x * ambient.x / 2),
                       max(sum.y, diff.y * ambient.y / 2),
                       max(sum.z, diff.z * ambient.z / 2));
}