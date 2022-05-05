#version 410
in vec4 pos;
layout(location = 0) out vec4 depth;

#define MAX_LIGHTS 12
uniform samplerCube shadow_cubemap;
uniform vec3 light_pos;
uniform float far_plane;
// out vec4 FragColor;
void main() {
  // get distance between fragment and light source
  vec3 dir = pos.xyz - light_pos;
  float light_distance = length(dir);
  // map to [0;1] range by dividing by far_plane
  light_distance = light_distance / far_plane;
  // write this as modified depth
  // gl_FragDepth = light_distance;
  float old_depth = texture(shadow_cubemap, dir).r;
  // if (old_depth > light_distance)
  depth.xyz = vec3(light_distance);
  // else
  //   depth.r = old_depth;
  // else
  //   depth.r = light_distance;
  // FragColor = light_distance;
}