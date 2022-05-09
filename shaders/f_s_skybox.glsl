#version 410
out vec4 FragColor;
in vec3 tex_coords;
uniform samplerCube skybox;
uniform samplerCube skybox2;
void main() {
  vec4 color1 = texture(skybox, tex_coords);
  vec4 color2 = texture(skybox2, -tex_coords);
  FragColor = (color1) / 2;
  // FragColor = color2;
}