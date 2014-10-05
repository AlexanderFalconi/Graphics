attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec2 texCoord0;
varying vec3 normal0;
uniform mat4 mvp;
uniform mat4 Normal;
void main (void)
{	
	gl_Position = mvp * vec4(position, 1.0);
	texCoord0 = texCoord;
}