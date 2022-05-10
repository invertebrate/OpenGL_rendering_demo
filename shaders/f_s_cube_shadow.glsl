#version 410
in vec4 pos;
flat in int index;
layout(location = 0) out float depth;

// uniform samplerCube depth1s;
// uniform samplerCube depth2s;

#define MAX_LIGHTS 12
#define FLT_MAX 3.402823466e+38
uniform vec3 light_pos;
uniform int light_count;
uniform float far_plane;
// out vec4 FragColor;
void main() {
  // get distance between fragment and light source
  vec3 dir = pos.xyz - light_pos;
  float light_distance = length(dir) / far_plane;
  // map to [0;1] range by dividing by far_plane
  depth = light_distance;
}