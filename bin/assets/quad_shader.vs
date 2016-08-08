in vec2 in_verts;

in vec2 in_pos;
in vec2 in_scale;

out vec2 vert;

void main()
{
	vert = in_verts.xy;
	gl_Position = mat_proj * mat_view *
		vec4((in_verts.xy + in_pos.xy) * in_scale.xy, 0, 1);
}
