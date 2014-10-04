uniform sampler2D diffuse;
varying vec2 textCoord0;

void main (void)
{
	gl_FragColor = texture2D(diffuse, textCoord0);
}