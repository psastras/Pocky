precision mediump float;
uniform sampler2D tex;
uniform vec2 texScale;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
uniform vec3 lightpositions[50];
uniform int nLights;
#ifdef _VERTEX_
attribute vec3 in_Position;
attribute vec3 in_TexCoord;
varying vec3 pass_TexCoord;
varying float pass_dL;
void main(void) {
	pass_TexCoord  = in_TexCoord;
	vec4 vertpos = modelviewMatrix * vec4(in_Position,1.0);
	pass_dL = 0.0;
	for(int i=0; i<nLights;i++) 
	{
		float l = (dot(normalize(lightpositions[i] - vertpos.xyz), vec3(0.0, 0.0, 1.0)) * 2.5);
		pass_dL += l*l;
	}
   	pass_dL = min(pass_dL, 1.0);
	//pass_dL *= (pass_dL);
    gl_Position = projMatrix * vertpos;
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