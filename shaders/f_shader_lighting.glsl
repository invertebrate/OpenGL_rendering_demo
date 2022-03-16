#version 410
in vec2 texCoord;
in vec3 normal;
in vec3 fragpos;
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D normalmap;
  vec3 specular;
  float specular_strength;
  float shininess;
};
uniform sampler2D d;
uniform Material material;
uniform mat4 rotation;
uniform vec4 light_dir;
uniform vec3 viewpos;

void main() {
  vec3 lightColor = vec3(1.0, 1.0, 1.0);

  vec3 n_light_dir = normalize(light_dir.xyz);
  // vec3 viewDir = normalize(viewpos - fragpos);
  // vec3 reflectDir = reflect(-n_light_dir, normal);

  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  // vec3 specular = material.specular_strength * spec * lightColor;

  vec4 diff = texture(d, texCoord);
  // float light = 7 * max((-dot(normalize(rotation * vec4(normal, 1.0)),
  //                             vec4(n_light_dir, 1.0))),
  //                       0.0);
  float light =
      4 *
      max((-dot((normalize(rotation * vec4(normal, 1.0))).xyz, n_light_dir)),
          0.0);
  diff.xyz = light * diff.xyz;

  // diff.xyz = light * diff.xyz + specular;
  FragColor = diff;
}