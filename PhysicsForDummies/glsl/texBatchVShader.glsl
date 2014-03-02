#version 330
layout(location=0) in vec4 vPosition;
uniform mat4 worldMat;
uniform vec2 m_scaling[256];
uniform float m_rotation[256];
uniform vec2 m_location[256];
uniform vec2 m_texS[256];
smooth out vec2 UV;
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
	vec4 textmp = tmp;
	tmp = RotationMatrix*tmp;
	tmp.x += m_location[gl_InstanceID].x;
	tmp.y += m_location[gl_InstanceID].y;
	gl_Position = (worldMat * tmp);
	//textmp.x*=m_texS[gl_InstanceID].x;
	//textmp.y*=m_texS[gl_InstanceID].y;
	textmp.z*=m_texS[gl_InstanceID].x;
	textmp = RotationMatrix*textmp;
	UV = textmp.xy+ m_location[gl_InstanceID].xy;//vPosition.xy + vec2(.5,.5);
	InstanceID = gl_InstanceID;
}