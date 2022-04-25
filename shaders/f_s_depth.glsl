#version 410
in vec3 vtx_dlight[16];
in vec3 vtx_plight[96];  // transformed into lightspace
// out vec4 FragColor;
uniform mat4 light_view[16];
uniform mat4 cube_view[96];
void main() {
  // FragColor.xyz = vec3(0.5, 1.0, 0.2);
  // set render targets
  // loop lights;
}