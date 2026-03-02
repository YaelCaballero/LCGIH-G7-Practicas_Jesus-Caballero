#version 330 core
out vec4 color;
in vec3 ourColor;

uniform vec3 overrideColor; 
uniform bool useOverride;   

void main() {
    if(useOverride) {
        color = vec4(overrideColor, 1.0f);
    } else {
        color = vec4(ourColor, 1.0f);
    }
}
