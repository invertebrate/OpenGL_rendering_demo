#version 410
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D s;
uniform mat4 rotation;
uniform vec4 light_dir;
in vec3 normal;
void main() {
  vec4 nlight_dir = normalize(light_dir);
  vec4 diff = texture(s, texCoord);
  vec4 normal_t = normalize(rotation * vec4(normal, 1.0));
  float light = 2 * (-dot(normal_t, nlight_dir) + 1);
  diff.xyz = light * diff.xyz;
  FragColor = vec4(diff);
}