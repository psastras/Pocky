precision mediump float;
uniform sampler2D tex;
uniform vec2 texScale;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
uniform vec3 lightpos;
uniform float lum;
#ifdef _VERTEX_
attribute vec3 in_Position;
attribute vec3 in_TexCoord;
varying vec3 pass_TexCoord;
varying float pass_dL;
void main(void) {
	pass_TexCoord  = in_TexCoord;
	vec4 vertpos = modelviewMatrix * vec4(in_Position,1.0);
   	pass_dL = dot(normalize(lightpos - vertpos.xyz), vec3(0.0, 0.0, 1.0)) * lum;
	pass_dL *= (pass_dL + 0.5);
    gl_Position = projMatrix * modelviewMatrix * vec4(in_Position,1.0);
}
#endif

#ifdef _FRAGMENT_
varying vec3 pass_TexCoord;
varying float pass_dL;
void main() {
   vec4 col = texture2D(tex, texScale*pass_TexCoord.st) * pass_dL ;
   col.st *= pass_TexCoord.st;
   gl_FragColor = col;
}
#endif 