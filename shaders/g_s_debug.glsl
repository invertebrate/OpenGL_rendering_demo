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
  vec4 pos;
}
gs_out;

uniform mat4 camera_view;
uniform mat4 world;

float magnitude = 0.5;

void create_line(int i) {
  gl_Position = camera_view * gs_in[i].pos;
  EmitVertex();
  gl_Position = camera_view * gs_in[i].pos +
                magnitude * normalize(camera_view * vec4(gs_in[i].normal, 0.0));
  EmitVertex();
  EndPrimitive();
}

void main() {
  for (int i = 0; i < 3; i++)
    create_line(i);
}