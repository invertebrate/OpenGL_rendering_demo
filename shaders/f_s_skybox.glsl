#version 410
out vec4 FragColor;
in vec3 tex_coords;
uniform samplerCube skybox;
void main() {
  FragColor.r = texture(skybox, tex_coords).r;
}