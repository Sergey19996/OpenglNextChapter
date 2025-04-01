#version 330 core
layout (location = 0) in vec4 vertex;
out vec3 fragPos;

out float time;
out vec2 TexCoord;
uniform float timeValue;

uniform mat4 model;
uniform mat4 projection;

//for bricks
uniform int tileIndex;
int TextureSize = 1024;
int tiles_per_row = TextureSize / 128;  // 4 тайла на строку

void main(){

 // Генерация случайного смещения для каждой вершины
 //   float uniqueOffset = vertex.x * vertex.y * 0.4; 

gl_Position = projection * model * vec4(vertex.xy,0.0,1.0);
 
 //brick index calc
int x = tileIndex % tiles_per_row; // Колонка (номер по X)
int y = tileIndex / tiles_per_row; //колонка  (номер по Y)

float pxlX = x * 128.0;
float pxlY = y * 128.0;

float uvX = float(pxlX) / TextureSize;
float uvY = float(pxlY) / TextureSize;
//
    TexCoord = vec2(vertex.z + uvX,vertex.w + uvY);
    fragPos = vec3(vertex.xy,1.0);
  
    time = timeValue;
}