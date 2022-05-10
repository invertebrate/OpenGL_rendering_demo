#version 410
out vec4 FragColor;
in vec3 tex_coords;
uniform samplerCube skybox;
uniform samplerCube skybox2;
uniform samplerCube sky_array[2];
void main() {
  vec4 color1 = texture(skybox, tex_coords);
  vec4 color2 = texture(skybox2, -tex_coords);
  FragColor = (color1 + color2) / 2;
  color1 = texture(sky_array[0], tex_coords);
  color2 = texture(sky_array[1], -tex_coords);
  FragColor = (color1 + color2) / 2;
  // FragColor = color2;
}