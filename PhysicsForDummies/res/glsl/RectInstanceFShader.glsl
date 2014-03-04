#version 330
layout(location =0) out vec4 colorOut;
flat in vec4 color;
void main(){
	colorOut = color;
}