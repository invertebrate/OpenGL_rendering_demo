#version 410
in vec4 pos;
layout(location = 1) out vec4 depth;

uniform vec3 light_pos;
uniform float far_plane;
// out vec4 FragColor;
void main() {
  // get distance between fragment and light source
  float light_distance = length(pos.xyz - light_pos);
  // map to [0;1] range by dividing by far_plane
  light_distance = light_distance / far_plane;
  // write this as modified depth
  gl_FragDepth = light_distance;
  depth.r = light_distance;
  // FragColor = light_distance;
}