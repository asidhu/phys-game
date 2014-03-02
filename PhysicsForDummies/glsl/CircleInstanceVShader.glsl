#version 330
layout(location = 0) in vec4 vPosition;
flat out int InstanceID;
uniform float m_radius[256];
uniform vec2  m_location[256];
uniform mat4 worldMat;
void main(){
	vec4 tmp = vPosition;
	tmp.x *=m_radius[gl_InstanceID];
	tmp.y *=m_radius[gl_InstanceID];
	tmp.x += m_location[gl_InstanceID].x;
	tmp.y += m_location[gl_InstanceID].y;
	gl_Position = (worldMat * tmp );
	InstanceID=gl_InstanceID;
}