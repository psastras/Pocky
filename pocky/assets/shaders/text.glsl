precision mediump float;
uniform sampler2D tex;
uniform vec2 texScale;
uniform vec2 texOffset;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;

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
   gl_FragColor = texture2D(tex, texOffset + pass_TexCoord.st * texScale) * vec4(1.0, 0.6, 1.0, 1.0);
}
#endif 