precision mediump float;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
uniform float time;

#ifdef _VERTEX_
attribute vec3 in_Position;
attribute vec3 in_TexCoord;
varying vec3 pass_TexCoord;
void main(void) {
	pass_TexCoord  = in_TexCoord;
    gl_Position = projMatrix * modelviewMatrix * vec4(in_Position,1.0);
}
#endif

#ifdef _FRAGMENT_
varying vec3 pass_TexCoord;
void main() {
   gl_FragColor.xyz = pass_TexCoord;//*time;
   gl_FragColor.z = 1.0;
   gl_FragColor.w = 1.0;
   gl_FragColor.xyz /= 2.0;
}
#endif 