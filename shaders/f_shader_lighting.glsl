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

void main() {
  r_normal =
      normalize(tbn * (texture(material.normalmap, texCoord).rgb * 2.0 - 1.0));
  // n_light_dir = normalize(light_dir.xyz);
  n_light_dir = f_world_pos.xyz - light_pos;
  light_dist = length(n_light_dir);
  n_light_dir = normalize(n_light_dir);
  viewDir = normalize(f_world_pos.xyz - viewpos);

  f_shadow = projection * light_view * f_world_pos;
  // f_shadow = f_shadow;
  vec3 projcoords = f_shadow.xyz / f_shadow.w;
  projcoords = projcoords * 0.5 + 0.5;

  float closestDepth = texture(shadowmap, projcoords.xy).r;

  float currentDepth = projcoords.z;
  float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

  // float bias = 0.005;
  // shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

  reflectDir = reflect(-n_light_dir, r_normal);
  spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  specular = texture(material.specularmap, texCoord).rgb *
             material.specular_strength * spec * light_color;
  diff = vec4(texture(material.diffuse, texCoord));
  light =
      2 * max((-dot((normalize(vec4(r_normal, 1.0))).xyz, n_light_dir)), 0.0);
  diff = light * diff;
  light_dist = 1;  //
  float mult = ((1 / light_dist) * light_strength * 3) *
               (1.0 - shadow);  // * texture(shadowmap, );
  FragColor.xyz = (light * diff.xyz + specular) * mult;
  FragColor.xyz = vec3(max(FragColor.x, diff.x * ambient.x),
                       max(FragColor.y, diff.y * ambient.y),
                       max(FragColor.z, diff.z * ambient.z));
  // FragColor = texture(shadowmap, projcoords.xy);
  vec4 color = projection * light_view * f_world_pos;
  // color = color / color.w;
  // FragColor = vec4(color.xyz, 1.0);
}