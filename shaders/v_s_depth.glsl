#version 410
layout(location = 0) in vec3 attr_pos;
// uniform mat4 screen;
// uniform mat4 cube_view;
uniform mat4 light_view[16];
uniform mat4 light_proj;
uniform mat4 cube_view[96];
uniform mat4 camera_view;
uniform mat4 world;
out vec3 vtx_dlight[16];
out vec3 vtx_plight[96];  // transformed into lightspace
void main() {
  gl_Position = light_proj * light_view[0] * world * vec4(attr_pos, 1.0);
}