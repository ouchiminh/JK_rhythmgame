precision highp float;

uniform float t; // time
uniform vec2  r; // resolution

float PI = 3.1415926535;

void wave(vec2 p, inout vec3 dest, float speed, float offset_time, vec3 base_color, float amp, float width){
	float time_param = t * speed + offset_time;
	float org_sin = sin(p.x * (1.0/width) * PI + time_param) * amp;
	dest += 0.01 / abs(p.y - org_sin) * base_color;
}

void main(void){
    vec2 p = (gl_FragCoord.xy * 2.0 - r) / min(r.x, r.y);
    vec3 dest;
    vec3 linec[3];
    linec[0] = vec3(.5, .8, 0);
    linec[1] = vec3(.8, 0, .5);
    linec[2] = vec3(0, .5, .8);
    
    for(float i = 0.0; i < 3.0; i++)
    	wave(p, dest, i / PI + .2, i, linec[int(i)], 0.2, cos(t) / 1.0 + 3.0 + i);
    

    gl_FragColor = gl_Color + vec4(dest, t/5.0);
}
