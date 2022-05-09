#version 410

#define MAX_LIGHTS 12

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

uniform samplerCube shadow_cubemap0;
uniform samplerCube shadow_cubemap1;
uniform samplerCube shadow_cubemap2;
uniform samplerCube shadow_cubemap3;
uniform samplerCube shadow_cubemap4;
uniform samplerCube shadow_cubemap5;
uniform samplerCube shadow_cubemap6;
uniform samplerCube shadow_cubemap7;
uniform samplerCube shadow_cubemap8;
uniform samplerCube shadow_cubemap9;
uniform samplerCube shadow_cubemap10;
uniform samplerCube shadow_cubemap11;
uniform samplerCube shadow_cubemap12;

uniform mat4 screen;
uniform mat4 world;
uniform mat4 camera_view;
uniform mat4 light_view[16];
// uniform mat4 cube_view[96];  // dir and pos data in these matrices
// uniform mat4 light_view;

uniform mat4 light_proj;
uniform vec3 p_light_color[MAX_LIGHTS];
uniform float p_light_strength[MAX_LIGHTS];
uniform vec3 ambient;
uniform vec3 p_light_pos[MAX_LIGHTS];

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

float pcf_cube(samplerCube shadow_cubemap, vec3 light_to_pos, float facing) {
  float shadow = 0.0;
  float bias = 0.05;
  bias = max(bias * (1.0 - facing), bias / 5);
  float samples = 4.0;
  float offset = 0.1;
  float current_depth = length(light_to_pos);
  for (float x = -offset; x < offset; x += offset / (samples * 0.5)) {
    for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
      for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {
        float closest_depth =
            texture(shadow_cubemap, light_to_pos + vec3(x, y, z)).r;
        closest_depth *= 100;  // change to far plane
        if (current_depth - bias > closest_depth)
          shadow += 1.0;
      }
    }
  }
  shadow /= (samples * samples * samples);
  return (shadow);
}
float get_shadow(samplerCube shadow_cubemap, int index) {
  // vec3 light_to_pos =
  //     -(fs_in.world_pos.xyz - p_light_pos[index]);  // double check sign
  // *facing = dot(normalize(fs_in.normal), normalize(light_to_pos));
  float shadow;
  shadow = pcf_cube(shadow_cubemap, (fs_in.light_to_pos)[index],
                    abs((fs_in.facing)[index]));
  return (fs_in.facing[index] <= 0.0 ? 1.0 : shadow);
}

float[MAX_LIGHTS] get_point_shadows() {
  float[MAX_LIGHTS] shadows;
  shadows[0] = get_shadow(shadow_cubemap0, 0);
  shadows[1] = get_shadow(shadow_cubemap1, 1);
  return (shadows);
}

// for directinal light
// run the code for every light
// save the result in an array
// calculate exposure from the array values
// add values together and multiply by exposure
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
  // float facing;
  float roughness = 0.15;

  r_normal =
      normalize(fs_in.tbn *
                (texture(material.normalmap, fs_in.tex_coord).rgb * 2.0 - 1.0));
  f_lightspace = fs_in.lightspace_pos;
  proj_coords = f_lightspace.xyz / f_lightspace.w;
  proj_coords = proj_coords * 0.5 + 0.5;
  closest_depth = texture(shadowmap, proj_coords.xy).r;
  current_depth = proj_coords.z;

  // facing = -dot(normalize(fs_in.normal), fs_in.p_light_dir[0]);
  float lfacing;
  shadow = pcf(shadowmap, proj_coords, current_depth, abs(fs_in.facing[0]));
  lfacing = max(fs_in.facing[0], roughness);
  shadow = current_depth > closest_depth ? 1.0 : 0.0;

  // cubeshadow

  // vec3 light_to_pos =
  //     -(fs_in.world_pos.xyz - p_light_pos[0]);  // double check sign
  // // float closestDepth = texture(shadow_cubemap, light_to_pos).r;  // not
  // // needed? closestDepth *= 100; //
  // // far_plane; float currentDepth = length(light_to_pos);
  // facing = dot(normalize(fs_in.normal), normalize(light_to_pos));
  // shadow = pcf_cube(shadow_cubemap, light_to_pos, abs(facing));
  // // shadow = currentDepth > closestDepth ? 1.0 : 0.0;

  // shadow = facing <= 0.0 ? 1.0 : shadow;
  float shadows[MAX_LIGHTS];
  shadows = get_point_shadows();
  vec3 light_to_pos1 = -(fs_in.world_pos.xyz - p_light_pos[0]);

  // shadow1 = pcf_cube(shadow_cubemap0, light_to_pos1, abs((fs_in.facing)[0]));

  reflect_dir = reflect(-fs_in.p_light_dir[0], r_normal);
  specular = texture(material.specularmap, fs_in.tex_coord).rgb *
             material.specular_strength * 5 *
             pow(max(dot(fs_in.view_dir, reflect_dir), 0.0), 32) *
             p_light_color[0];
  diff = texture(material.diffuse, fs_in.tex_coord);
  light =
      max((-dot((normalize(vec4(r_normal, 1.0))).xyz, fs_in.p_light_dir[0])),
          0.0) *
      p_light_strength[0] * 15;
  attenuation = 1 / length(fs_in.world_pos.xyz - p_light_pos[0]);
  attenuation = 1;  //./
  FragColor.xyz = (light * diff.xyz + specular) * (1.0 - shadows[0]) *
                  fs_in.facing[0] * attenuation;
  FragColor.xyz = vec3(max(FragColor.x, diff.x * ambient.x / 2),
                       max(FragColor.y, diff.y * ambient.y / 2),
                       max(FragColor.z, diff.z * ambient.z / 2));
  // FragColor.xyz = vec3(texture(shadow_cubemap, light_to_pos).r * 100);
  // FragColor.xyz = light_to_pos;
}