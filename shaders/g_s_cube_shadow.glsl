#version 410
#define MAX_LIGHTS 12

layout(triangles) in;
layout(triangle_strip, max_vertices = 18)
    out;       // output more vertices when rendering to multiple cubemaps
out vec4 pos;  // FragPos from GS (output per emitvertex)
uniform mat4 cube_view[6];  // one set of view matrices for each light
uniform mat4 light_proj;
uniform int light_count;
void main() {
  for (int face = 0; face < 6; ++face) {
    gl_Layer = face;              // built-in variable: to which face we render.
    for (int i = 2; i >= 0; --i)  // for each triangle vertex
    {
      pos = gl_in[i].gl_Position;
      gl_Position = light_proj * cube_view[face] * pos;
      EmitVertex();
    }
    EndPrimitive();
  }
}