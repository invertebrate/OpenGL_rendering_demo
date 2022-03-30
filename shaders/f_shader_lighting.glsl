#version 410
in vec2 texCoord;
in vec3 normal;
in vec3 fragpos;
in mat3 tbn;
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

vec3 r_normal;
vec3 viewDir;
vec3 reflectDir;
vec3 n_light_dir;
float spec;
vec3 specular;
vec4 diff;
float light;

void main() {
  r_normal =
      normalize(tbn * (texture(material.normalmap, texCoord).rgb * 2.0 - 1.0));
  // n_light_dir = normalize(light_dir.xyz);
  n_light_dir = normalize(vec3((screen * vec4(light_pos, 1.0))).xyz - fragpos);
  viewDir = normalize(viewpos - fragpos);
  reflectDir = reflect(-n_light_dir, r_normal);
  spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  specular = texture(material.specularmap, texCoord).rgb *
             material.specular_strength * spec * light_color;
  diff = texture(material.diffuse, texCoord);
  light =
      2 * max((-dot((normalize(vec4(r_normal, 1.0))).xyz, n_light_dir)), 0.0);
  diff.xyz = light * diff.xyz + specular;
  FragColor = diff;
}