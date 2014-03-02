attribute vec2 texture_coord;
varying vec2 UV;
void main(){
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	UV=texture_coord;
}