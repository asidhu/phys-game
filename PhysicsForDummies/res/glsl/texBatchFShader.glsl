#version 330
layout(location=0) out vec4 colorOut;
uniform sampler2D m_texture[7];
smooth in vec2 UV;
flat in int texID;
void main(){
	if(texID==3)
		colorOut = texture2D(m_texture[3],UV);
	else if(texID<3)
	{
		if(texID==1)
			colorOut = texture2D(m_texture[1],UV);
		else if(texID<1)
			colorOut = texture2D(m_texture[0],UV);
		else
			colorOut = texture2D(m_texture[2],UV);
	}
	else{
		if(texID==5)
			colorOut = texture2D(m_texture[5],UV);
		else if(texID<5)
			colorOut = texture2D(m_texture[4],UV);
		else
			colorOut = texture2D(m_texture[6],UV);
	}
	//
}