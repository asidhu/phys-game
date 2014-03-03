#version 330
layout(location = 0) out vec4 colorOut;
flat in int InstanceID;
uniform vec4 color[256];
void main(){
	colorOut = color[InstanceID];
}