#version 410
in vec4 pos;
layout(location = 0) out vec4 depth1;
layout(location = 1) out vec4 depth2;

#define MAX_LIGHTS 12
#define FLT_MAX 3.402823466e+38
uniform vec3 light_pos[MAX_LIGHTS];
uniform float far_plane;
// out vec4 FragColor;
void main() {
  vec3 depth[MAX_LIGHTS];
  // get distance between fragment and light source
  for (int i = 0; i < 2; i++) {
    vec3 dir = pos.xyz - light_pos[i];
    float light_distance = length(dir);
    // map to [0;1] range by dividing by far_plane
    light_distance = light_distance / far_plane;
    depth[i] = vec3(light_distance);
  }
  depth1.xyz = depth[0];  // try ouput as array?
  depth2.xyz = depth[1];
  // gl_FragDepth = FLT_MAX;

  //!!WHEN LOOPING LIGHTS, CLEAR DEPTH VALUE EVERY LOOP SO CAN USE SAME BUFFER
  // else
  //   depth.r = old_depth;
  // else
  //   depth.r = light_distance;
  // FragColor = light_distance;
}