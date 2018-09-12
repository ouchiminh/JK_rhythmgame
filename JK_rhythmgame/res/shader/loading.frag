precision mediump float;

uniform float t; // time
uniform vec2  r; // resolution

const float PI = 3.1415926535;
const float radius = 0.3;

float calc_displacement(float cycle, float offset){
	float time_param = t + offset;
	return cycle / (-2.0 * PI) * sin(-2.0 * PI / cycle * time_param) + 2.0 * PI * time_param/cycle;
}

void main(void){
    vec2 p = (gl_FragCoord.xy * 2.0 - r) / min(r.x, r.y);
    vec3 base_color = vec3(1.0, 160.0 / 255.0, 0.0);
    
    vec2 light[7];
    for(int i = 0; i < 7; i++) light[i] = vec2(0, -radius);
    
    for(float i = 0.0; i < 7.0; i++){
    	float angle = calc_displacement(4.0, i / 6.7);
    	light[int(i)] = mat2(cos(angle), -sin(angle), sin(angle), cos(angle)) * light[int(i)];
    	gl_FragColor += vec4(step(.8, .01 / length(p - light[int(i)])) * base_color, t/5.0);
    }
}
