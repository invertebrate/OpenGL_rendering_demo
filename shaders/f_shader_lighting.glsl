#version 410
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D diffuse;
uniform sampler2D normalmap;
uniform mat4 rotation;
uniform vec4 light_dir;
in vec3 normal;
void main() {
  vec4 nlight_dir = normalize(light_dir);
  vec4 diff = texture(normalmap, texCoord);
  float light =
      7 * max((-dot(normalize(rotation * vec4(normal, 1.0)), nlight_dir)), 0.0);
  diff.xyz = light * diff.xyz;
  FragColor = vec4(diff);
}