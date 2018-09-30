precision mediump float;

uniform float t;		// time
uniform vec2  r;		// resolution

vec2 center = vec2(0.3, -0.4);
float radius = 0.5;	// radius of circle
vec3 color = vec3(1.0,0.0,0.0);		// lane color

vec4 make_color(vec2 p){
	vec3 this_color = 0.4/length(center - p)*color;
	float alpha = step(radius, length(center - p));
	
	return vec4(this_color, alpha);
}

void main(void){
	vec2 p = (gl_FragCoord.xy * 2.0 - r) / min(r.x, r.y);
	gl_FragColor += make_color(p);
}
