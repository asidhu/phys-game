#version 330
layout(location =0) out vec4 colorOut;
flat in int InstanceID;
uniform vec4 m_color[256];
void main(){
	colorOut = m_color[InstanceID];
}