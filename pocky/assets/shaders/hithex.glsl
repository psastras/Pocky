precision mediump float;
uniform sampler2D tex;
uniform vec2 tcOffset;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
uniform float life;
uniform vec3 color;
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
   gl_FragColor = texture2D(tex, pass_TexCoord.st)*life;
   gl_FragColor.xyz *= color;
   //gl_FragColor.st *= tcOffset;
   gl_FragColor.w = 0.5;
}
#endif
