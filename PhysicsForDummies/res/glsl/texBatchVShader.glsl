#version 330
layout(location=0) in vec4 vPosition;
layout(location=1) in vec2 m_location;
layout(location=2) in float m_rotation;
layout(location=3) in vec2 m_scaling;
layout(location=4) in vec2 m_texS;
layout(location=5) in int m_texID;
uniform mat4 worldMat;

smooth out vec2 UV;
flat out int texID;
void main(){
	float cs = cos(m_rotation);
	float sn = sin(m_rotation);
	mat4 RotationMatrix = mat4( cs, sn, 0.0, 0.0,
		-sn, cs, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 ); 
	vec4 tmp = vPosition;
	tmp.xy*=m_scaling;
	vec4 textmp = tmp;
	tmp = RotationMatrix*tmp;
	tmp.xy += m_location;
	gl_Position = (worldMat * tmp);
	textmp.xy*=m_texS;
	textmp = RotationMatrix*textmp;
	UV = textmp.xy+ m_location.xy;
	texID=m_texID;
}