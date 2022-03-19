#version 410
in vec2 texCoord;
in vec3 normal;
in vec3 fragpos;
in mat3 tbn;
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D normalmap;
  vec3 specular;
  float specular_strength;
  float shininess;
};
uniform Material material;
uniform mat4 rotation;
uniform vec4 light_dir;
uniform vec3 viewpos;

void main() {
  vec3 r_normal = normalize((rotation * vec4(normal, 1.0)).xyz);
  r_normal = texture(material.normalmap, texCoord).rgb;
  r_normal = r_normal * 2.0 - 1.0;
  r_normal = normalize(tbn * r_normal);

  // r_normal = normal;
  // r_normal = texture(material.normalmap, texCoord).xyz;
  // r_normal = normalize(r_normal * 2.0 - 1.0);

  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 n_light_dir = normalize(light_dir.xyz);
  // n_light_dir = normalize(vec3(0.0, 1.0, 0.0));
  vec3 viewDir = normalize(viewpos - fragpos);
  vec3 reflectDir = reflect(-n_light_dir, r_normal);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = material.specular_strength * spec * lightColor;

  vec4 diff = texture(material.diffuse, texCoord);
  float light =
      2 * max((-dot((normalize(vec4(r_normal, 1.0))).xyz, n_light_dir)), 0.0);
  // diff.xyz = light * diff.xyz;

  diff.xyz = light * diff.xyz + specular;
  // diff.xyz = normalize(normal);
  FragColor = diff;
}