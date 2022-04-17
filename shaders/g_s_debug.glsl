#version 410
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
  vec2 texCoord;
  vec3 normal;
  vec4 fragpos;
  mat3 tbn;
}
gs_in[];

out GS_OUT {
  vec2 texCoord;
  vec3 normal;
  vec4 fragpos;
  mat3 tbn;
}
gs_out;

uniform mat4 screen;
uniform mat4 world;

float magnitude = 0.1;

void create_line(int i) {
  vec3 normal =
      normalize((screen * (inverse(world) * vec4(gs_in[i].normal, 1.0))).xyz);
  gl_Position = gl_in[i].gl_Position;
  EmitVertex();
  gl_Position = gl_in[i].gl_Position + vec4(magnitude * normal, 1.0);
  EmitVertex();
  EndPrimitive();
}

void main() {
  for (int i = 0; i < 3; i++)
    create_line(i);
}