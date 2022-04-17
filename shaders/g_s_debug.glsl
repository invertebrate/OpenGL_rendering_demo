#version 410
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 pos;
}
gs_in[];

out GS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 vtx_pos;
}
gs_out;

uniform mat4 screen;
uniform mat4 world;

float magnitude = 0.1;

void create_line(int i) {
  // vec3 normal = (screen * (vec4(gs_in[i].normal, 1.0))).xyz;
  vec3 normal = normalize(gs_in[i].normal);
  gl_Position = screen * gs_in[i].pos;
  EmitVertex();
  gl_Position = screen * (gs_in[i].pos + (magnitude * vec4(normal, 0.0)));
  EmitVertex();
  EndPrimitive();
}

void main() {
  for (int i = 0; i < 3; i++)
    create_line(i);
}