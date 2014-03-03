#version 330
layout(location = 0) out vec4 colorOut;
uniform vec4 color;
void main(){
	colorOut = color;
}