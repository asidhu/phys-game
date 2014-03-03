uniform sampler2D myTexture;
varying vec2 UV;
void main(){
	gl_FragColor = texture2D(myTexture, UV);
}