#version 330
layout(location =0) in vec4 vPosition;
layout(location=1) in vec2 m_location;
layout(location=2) in float m_rotation;
layout(location=3) in vec2 m_scaling;
layout(location=4) in vec4 m_color;
uniform mat4 worldMat;
flat out vec4 color;
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
	gl_Position = (worldMat * tmp);
	color=m_color;
}