#version 330

layout(location = 0) in vec4 vPosition;
uniform mat4 modelMat;
uniform mat4 worldMat;
void main(){
	gl_Position = worldMat * modelMat * vPosition;
}