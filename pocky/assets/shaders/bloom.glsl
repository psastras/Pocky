precision mediump float;
uniform sampler2D tex;
uniform vec2 texScale;
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
void main()
{
   vec4 sum = vec4(0.0);
   vec2 texcoord = pass_TexCoord.st * texScale;
   sum += texture2D(tex, texcoord + vec2(-1, -1)*0.001);
   sum += texture2D(tex, texcoord + vec2(-1, 0)*0.001);
   sum += texture2D(tex, texcoord + vec2(-1, 1)*0.001);
   sum += texture2D(tex, texcoord + vec2(0, -1)*0.001);
   sum += texture2D(tex, texcoord + vec2(0, 0)*0.001);
   sum += texture2D(tex, texcoord + vec2(0, 1)*0.001);
   sum += texture2D(tex, texcoord + vec2(1, -1)*0.001);
   sum += texture2D(tex, texcoord + vec2(1, 0)*0.001);
   sum += texture2D(tex, texcoord + vec2(1, 1)*0.001);
   sum /= 9.0;
   gl_FragColor = sum;

}

#endif