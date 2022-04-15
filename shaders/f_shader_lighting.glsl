#version 410
in vec2 texCoord;
in vec3 normal;
in vec4 fragpos;
in mat3 tbn;
in vec4 f_world_pos;
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D normalmap;
  sampler2D specularmap;
  float specular_strength;
  float shininess;
};
uniform sampler2D shadowmap;

uniform Material material;
uniform mat4 screen;
uniform mat4 world;

uniform mat4 light_view;
uniform mat4 light_proj;

uniform vec3 light_dir;
uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 light_pos;
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
  vec3 view_dir;
  vec3 reflect_dir;
  vec3 n_light_dir;
  vec3 specular;
  vec4 diff;
  float light;
  vec4 f_lightspace;
  float facing;
  vec3 proj_coords;
  float closest_depth;
  float current_depth;
  float shadow;

  r_normal =
      normalize(tbn * (texture(material.normalmap, texCoord).rgb * 2.0 - 1.0));

  n_light_dir = normalize(light_dir);
  // n_light_dir = normalize(vec3(1, -1, 1));
  facing = dot(normalize(normal), n_light_dir);
  view_dir = normalize(f_world_pos.xyz - view_pos);

  f_lightspace = light_proj * light_view * f_world_pos;
  proj_coords = f_lightspace.xyz / f_lightspace.w;
  proj_coords = proj_coords * 0.5 + 0.5;
  closest_depth = texture(shadowmap, proj_coords.xy).r;
  current_depth = proj_coords.z;
  shadow = pcf(shadowmap, proj_coords, current_depth);
  shadow = facing >= 0 ? 1.0 : shadow;
  reflect_dir = reflect(-n_light_dir, r_normal);
  specular = texture(material.specularmap, texCoord).rgb *
             material.specular_strength *
             pow(max(dot(view_dir, reflect_dir), 0.0), 32) * light_color;
  diff = vec4(texture(material.diffuse, texCoord));
  light = max((-dot((normalize(vec4(r_normal, 1.0))).xyz, n_light_dir)), 0.0) *
          light_strength * 5;
  FragColor.xyz =
      (light * diff.xyz + specular) * (1.0 - shadow) * max(-facing, 0);
  FragColor.xyz = vec3(max(FragColor.x, diff.x * ambient.x),
                       max(FragColor.y, diff.y * ambient.y),
                       max(FragColor.z, diff.z * ambient.z));
}