attribute vec3 v_position;
attribute vec2 textCoord;
varying vec2 textCoord0;
uniform mat4 mvpMatrix;
void main (void)
{	
	gl_Position = mvpMatrix * vec4(v_position, 1.0);
	textCoord0 = textCoord;
}