precision mediump float;
uniform sampler2D tex;
uniform vec2 texScale;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
uniform float translate;
#ifdef _VERTEX_
attribute vec3 in_Position;
attribute vec3 in_TexCoord;
varying vec3 pass_TexCoord;
void main(void) {
	pass_TexCoord  = in_TexCoord;
	pass_TexCoord.s  += translate;
//	if(pass_TexCoord.s > 1.0 / texScale.s)
//		pass_TexCoord.s = pass_TexCoord.s + ;
    gl_Position = projMatrix * modelviewMatrix * vec4(in_Position,1.0);
}
#endif

#ifdef _FRAGMENT_
varying vec3 pass_TexCoord;
void main() {
   gl_FragColor = texture2D(tex, texScale*pass_TexCoord.st)*0.1;
}
#endif 