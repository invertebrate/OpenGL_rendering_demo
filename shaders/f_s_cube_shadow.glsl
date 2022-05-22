#version 410
in vec4 pos;
layout(location = 0) out float depth;

uniform vec3 light_pos;
uniform float far_plane;
void main() {
  float light_distance = length(pos.xyz - light_pos) / far_plane;
  // map to [0;1] range by dividing by far_plane
  depth = light_distance;
}