#version 330
layout(location = 0) in vec4 vPosition;
layout(location=1) in vec2 m_location;
layout(location=2) in float m_radius;
layout(location=3) in vec4 m_color;
uniform mat4 worldMat;
flat out vec4 color;
void main(){
	vec4 tmp = vPosition;
	tmp.xy *= m_radius;
	tmp.xy += m_location;
	gl_Position = (worldMat * tmp );
	color=m_color;
}