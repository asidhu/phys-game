#version 330
layout(location =0) in vec4 vPosition;
uniform vec2 m_scaling[256];
uniform float m_rotation[256];
uniform vec2  m_location[256];
uniform mat4 worldMat;
flat out int InstanceID;
void main(){
	float rot = m_rotation[gl_InstanceID];
	float cs = cos(rot);
	float sn = sin(rot);
	mat4 RotationMatrix = mat4( cs, -sn, 0.0, 0.0,
		sn, cs, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 ); 
	vec4 tmp = vPosition;
	tmp.x *=m_scaling[gl_InstanceID].x;
	tmp.y *=m_scaling[gl_InstanceID].y;
	tmp = RotationMatrix*tmp;
	tmp.x += m_location[gl_InstanceID].x;
	tmp.y += m_location[gl_InstanceID].y;
	gl_Position = (worldMat * tmp);
	InstanceID=gl_InstanceID;
}