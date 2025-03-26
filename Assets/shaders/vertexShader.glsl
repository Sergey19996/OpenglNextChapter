#version 330 core
layout (location = 0) in vec4 vertex;
out vec3 fragPos;

out float time;
out vec2 TexCoord;
uniform float timeValue;

uniform float x_offset;
uniform float y_offset;

uniform mat4 model;
uniform mat4 projection;

//for bricks
uniform int tileIndex;
int tiles_per_row = 512 / 128;  // 4 ����� �� ������

void main(){

 // ��������� ���������� �������� ��� ������ �������
 //   float uniqueOffset = vertex.x * vertex.y * 0.4; 

gl_Position = projection * model * vec4(vertex.xy,0.0,1.0);
 
 //brick index calc
int x = tileIndex % tiles_per_row; // ������� (����� �� X)
int y = tileIndex / tiles_per_row; //�������  (����� �� Y)

float pxlX = x * 128.0;
float pxlY = y * 128.0;

float uvX = float(pxlX) / 512.0;
float uvY = float(pxlY) / 512.0;
//
    TexCoord = vec2(vertex.z + uvX,vertex.w + uvY);
    fragPos = vec3(vertex.xy,1.0);
  
    time = timeValue;
}