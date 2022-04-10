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
uniform mat4 projection;

uniform vec4 light_dir;
uniform vec3 viewpos;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform float light_strength;
uniform vec3 ambient;

vec3 r_normal;
vec3 viewDir;
vec3 reflectDir;
vec3 n_light_dir;
float spec;
vec3 specular;
vec4 diff;
float light;
float light_dist;
vec4 f_shadow;

float pcf(sampler2D shadowmap, vec3 projcoords, float currentDepth) {
  float shadow;
  float bias = 0.0001;
  vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      float pcfDepth =
          texture(shadowmap, projcoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    }
  }
  if (projcoords.z > 1.0)
    shadow = 0.0;
  shadow /= 9.0;
  return (shadow);
}

void main() {  // shadows are directional light, normalmap is point light
  r_normal =
      normalize(tbn * (texture(material.normalmap, texCoord).rgb * 2.0 - 1.0));
  // r_normal = normal;
  // n_light_dir = normalize(light_dir.xyz);
  n_light_dir = f_world_pos.xyz - light_pos;

  light_dist = length(n_light_dir);
  n_light_dir = normalize(n_light_dir);
  float facing = dot(normalize(normal), normalize(-light_pos));

  viewDir = normalize(f_world_pos.xyz - viewpos);

  f_shadow = projection * light_view * f_world_pos;
  vec3 projcoords = f_shadow.xyz / f_shadow.w;
  projcoords = projcoords * 0.5 + 0.5;

  float closestDepth = texture(shadowmap, projcoords.xy).r;

  float currentDepth = projcoords.z;

  float shadow;

  shadow = pcf(shadowmap, projcoords, currentDepth);

  shadow = facing >= 0 ? 1.0 : shadow;

  reflectDir = reflect(-n_light_dir, r_normal);
  spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  specular = texture(material.specularmap, texCoord).rgb *
             material.specular_strength * spec * light_color;
  diff = vec4(texture(material.diffuse, texCoord));
  light =
      2 * max((-dot((normalize(vec4(r_normal, 1.0))).xyz, n_light_dir)), 0.0);
  light_dist = 1;  //
  // shadow = 0;
  float mult = ((1 / light_dist) * light_strength * 1) * (1.0 - shadow) *
               max(-facing, 0);  // * texture(shadowmap, );
  // light = 1;                    //
  // specular = vec3(0, 0, 0);     //
  FragColor.xyz = (light * diff.xyz + specular) * mult;
  FragColor.xyz = vec3(max(FragColor.x, diff.x * ambient.x),
                       max(FragColor.y, diff.y * ambient.y),
                       max(FragColor.z, diff.z * ambient.z));
  // FragColor = texture(shadowmap, projcoords.xy);
  vec4 color = projection * light_view * f_world_pos;
  // color = color / color.w;
  // FragColor = vec4(color.xyz, 1.0);
}