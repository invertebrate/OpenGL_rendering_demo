#version 410
in vec2 texCoord;
in vec3 normal;
in vec3 fragpos;
in mat3 tbn;
in vec3 f_world_pos;
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D normalmap;
  sampler2D specularmap;
  float specular_strength;
  float shininess;
};
uniform Material material;
uniform mat4 screen;
uniform vec4 light_dir;
uniform vec3 viewpos;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform float light_strength;

vec3 r_normal;
vec3 viewDir;
vec3 reflectDir;
vec3 n_light_dir;
float spec;
vec3 specular;
vec4 diff;
float light;
float light_dist;

void main() {
  r_normal =
      normalize(tbn * (texture(material.normalmap, texCoord).rgb * 2.0 - 1.0));
  // n_light_dir = normalize(light_dir.xyz);
  n_light_dir = f_world_pos - light_pos;
  light_dist = length(n_light_dir);
  n_light_dir = normalize(n_light_dir);
  viewDir = normalize(f_world_pos - viewpos);

  reflectDir = reflect(-n_light_dir, r_normal);
  spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  specular = texture(material.specularmap, texCoord).rgb *
             material.specular_strength * spec * light_color;
  diff = texture(material.diffuse, texCoord);
  light = 2 *
          max((-dot((normalize(vec4(r_normal, 1.0))).xyz, n_light_dir)), 0.0) *
          (1 / light_dist) * light_strength * 3;
  diff.xyz = light * diff.xyz + specular;
  FragColor = diff;
}