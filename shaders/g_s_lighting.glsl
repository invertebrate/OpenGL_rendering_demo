#version 410
layout(triangles) in;
layout(triangles, max_vertices = 3) out;
out vec4 pos;  // FragPos from GS (output per emitvertex)
in VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 world_pos;
  vec3 n_light_dir;
  vec4 lightspace_pos;
  vec3 view_dir;
  mat3 tbn;
}
gs_in

    out GS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 world_pos;
  vec3 n_light_dir;
  vec4 lightspace_pos;
  vec3 view_dir;
  mat3 tbn;
}
void main() {
  for (int face = 0; face < 6; ++face) {
    gl_Layer = face;             // built-in variable: to which face we render.
    for (int i = 0; i < 3; ++i)  // for each triangle vertex
    {
      pos = gl_in[i].gl_Position;
      gl_Position = cube_view[face] * pos;
      EmitVertex();
    }
    EndPrimitive();
  }
}