precision mediump float;
uniform sampler2D tex;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;

#ifdef _VERTEX_
attribute vec3 in_Position;
attribute vec3 in_Normal;
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
   gl_FragColor.xy = pass_TexCoord.st;//texture2D(tex, pass_TexCoord.st);
   gl_FragColor.z = 0.0;
   gl_FragColor = texture2D(tex, pass_TexCoord.st);
   gl_FragColor.y = 0.5;
}
#endif  