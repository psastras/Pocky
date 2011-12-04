precision mediump float;
uniform sampler2D tex;
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
   vec4 sum = vec4(0);
   vec2 texcoord = pass_TexCoord.st;

   sum += texture2D(tex, texcoord + vec2(-2, -2)*0.004) * 2.0;
   sum += texture2D(tex, texcoord + vec2(-2, -1)*0.004) * 4.0;
   sum += texture2D(tex, texcoord + vec2(-2, 0)*0.004) * 5.0;
   sum += texture2D(tex, texcoord + vec2(-2, 1)*0.004) * 4.0;
   sum += texture2D(tex, texcoord + vec2(-2, 2)*0.004) * 2.0;

   sum += texture2D(tex, texcoord + vec2(-1, -2)*0.004) * 4.0;
   sum += texture2D(tex, texcoord + vec2(-1, -1)*0.004) * 9.0;
   sum += texture2D(tex, texcoord + vec2(-1, 0)*0.004) * 12.0;
   sum += texture2D(tex, texcoord + vec2(-1, 1)*0.004) * 9.0;
   sum += texture2D(tex, texcoord + vec2(-1, 2)*0.004) * 4.0;

   sum += texture2D(tex, texcoord + vec2(0, -2)*0.004) * 5.0;
   sum += texture2D(tex, texcoord + vec2(0, -1)*0.004) * 12.0;
   sum += texture2D(tex, texcoord + vec2(0, 0)*0.004) * 15.0;
   sum += texture2D(tex, texcoord + vec2(0, 1)*0.004) * 12.0;
   sum += texture2D(tex, texcoord + vec2(0, 2)*0.004) * 5.0;
   
   sum += texture2D(tex, texcoord + vec2(1, -2)*0.004) * 4.0;
   sum += texture2D(tex, texcoord + vec2(1, -1)*0.004) * 9.0;
   sum += texture2D(tex, texcoord + vec2(1, 0)*0.004) * 12.0;
   sum += texture2D(tex, texcoord + vec2(1, 1)*0.004) * 9.0;
   sum += texture2D(tex, texcoord + vec2(1, 2)*0.004) * 4.0;
   
   sum += texture2D(tex, texcoord + vec2(2, -2)*0.004) * 2.0;
   sum += texture2D(tex, texcoord + vec2(2, -1)*0.004) * 4.0;
   sum += texture2D(tex, texcoord + vec2(2, 0)*0.004) * 5.0;
   sum += texture2D(tex, texcoord + vec2(2, 1)*0.004) * 4.0;
   sum += texture2D(tex, texcoord + vec2(2, 2)*0.004) * 2.0;
   gl_FragColor = sum * 0.08;//

}
#endif 