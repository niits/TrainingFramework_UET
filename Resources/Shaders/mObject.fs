precision mediump float;
uniform sampler2D u_texture_map;
uniform sampler2D u_texture_grass;
uniform sampler2D u_texture_dirt;
uniform sampler2D u_texture_rock;
varying vec2 v_uv;

void main()
{
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	vec2 uvTiling = 1.0 * v_uv;
	vec4 blendMap = texture2D(u_texture_map, v_uv);
	vec4 grass = texture2D(u_texture_grass, uvTiling);
	vec4 dirt = texture2D(u_texture_dirt, uvTiling);
	vec4 rock = texture2D(u_texture_rock, uvTiling);
	vec4 finalColor = (rock * blendMap.r + grass * blendMap.g + dirt * blendMap.b) / (blendMap.r + blendMap.g + blendMap.b);
	//gl_FragColor = texture2D(u_texture_map, v_uv);
	gl_FragColor = vec4(finalColor.rgb, 1.0);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

}
