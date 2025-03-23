#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1 ) in vec3 aColor; 
layout (location = 2) in vec2 aTexCoord;
out vec3 fragPos;
out vec3 ourColor;
out float time;
out vec2 TexCoord;
uniform float timeValue;

uniform float x_offset;
uniform float y_offset;
void main(){

gl_Position = vec4(aPos,1.0);
    
//    if(aPos.y > 0)
//    {
//        gl_Position = vec4(aPos.x+0.2 * sin(timeValue),aPos.y+0.2 * cos(timeValue),aPos.z,1.0);
//
//    }

    TexCoord =vec2(aTexCoord.x + x_offset,aTexCoord.y + y_offset);
    fragPos = aPos;
    ourColor = aColor;
    time = timeValue;
}