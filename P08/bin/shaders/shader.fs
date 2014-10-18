varying vec2 texCoord0;
varying vec3 normal0;
uniform sampler2D diffuse;
uniform vec3 lightDirection;

void main (void)
{
	gl_FragColor = texture2D(diffuse, texCoord0);
}