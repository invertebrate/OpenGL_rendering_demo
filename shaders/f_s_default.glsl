#version 410
in vec2 tex_coord;
out vec4 FragColor;
uniform sampler2D shadowmap;
void main() {
  // FragColor = vec4(vec3(texture(shadowmap, tex_coord).r), 1.0);
}