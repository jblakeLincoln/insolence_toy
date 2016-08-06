in vec2 fs_colour;

void main()
{
	gl_FragColor = vec4(fs_colour.xy, 0.2, 1);
}
