#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
  
uniform sampler2D texture_diffuse;
uniform int isTex;

void main()
{
  if(isTex == 1){
    FragColor = texture(texture_diffuse, TexCoords);
  } else if(isTex == 0) {
    FragColor = vec4(1.0f);
  }
}
