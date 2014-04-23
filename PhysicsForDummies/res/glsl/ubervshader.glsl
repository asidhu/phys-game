#version 330
layout(location=0) in vec4 vPosition;
layout(location=1) in vec2 m_location;
layout(location=2) in float m_rotation;
layout(location=3) in vec2 m_scaling;
layout(location=4) in vec4 m_color;
layout(location=5) in int m_texID;
layout(location=6) in float zIndex;
layout(location=7) in int shadermode;
uniform mat4 worldMat;
flat out int shader_mode;
flat out vec4 color;
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
	tmp = RotationMatrix*tmp;
	tmp.xy+=m_location;
	tmp.z = zIndex;
	UV = (vPosition.xy+ vec2(.5f,.5f))*m_texS;
	gl_Position = (worldMat * tmp);
	color=m_color;
	texID=m_texID;
	shader_mode = shadermode;
}