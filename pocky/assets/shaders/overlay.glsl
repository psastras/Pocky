
precision mediump float;
uniform sampler2D tex;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
uniform float height;
uniform float progress;
#ifdef _VERTEX_
attribute vec3 in_Position;
attribute vec3 in_TexCoord;
varying vec3 pass_TexCoord;
void main(void) {
	pass_TexCoord = in_TexCoord;
    gl_Position = projMatrix * modelviewMatrix * vec4(in_Position,1.0);
}
#endif

#ifdef _FRAGMENT_
varying vec3 pass_TexCoord;
void main() {
	if(pass_TexCoord.t <= height)
    	gl_FragColor = vec4(0.6, 0.0, 0.6, 0.9);
    else {
    	if(pass_TexCoord.s <= progress) {
    		if(pass_TexCoord.s >= progress - 0.0025)
    			gl_FragColor = vec4(0.6, 0.0, 0.6, 0.9);
    		else
    			gl_FragColor = vec4(0.3, 0.0, 0.3, 0.9);
    	} else {
    		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.9);
    	}
    }
}
#endif