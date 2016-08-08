in vec2 vert;
uniform float elapsed;

void main()
{
	gl_FragColor = vec4(vert.x * sin(elapsed / 200.0),
			vert.y * sin(elapsed / 20.0),
			vert.x * sin(elapsed / 80.0),
			1);
}
