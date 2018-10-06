precision mediump float;

uniform float t; // time
uniform vec2  r; // resolution
uniform float load_progress;    // progress

const float PI = 3.14159265359;
const float radius = 0.955;
const int lt_cnt = 9;
const vec3 base_color = vec3(1.0, 160.0 / 255.0, 0.0);

float calc_displacement(float cycle, float offset){
	float time_param = t + offset;
	return cycle / (-2.0 * PI) * sin(-2.0 * PI / cycle * time_param) + 2.0 * PI * time_param/cycle;
}

void draw_point(vec2 p){
    vec4 dest = vec4(0.0);
    
    vec2 light[lt_cnt];
    for(int i = 0; i < lt_cnt; i++) light[i] = vec2(0, -radius);
    
    for(float i = 0.0; i < float(lt_cnt); i++){
    	float angle = calc_displacement(4.0, i / 6.0);
    	light[int(i)] = mat2(cos(angle), -sin(angle), sin(angle), cos(angle)) * light[int(i)];
    	dest += vec4(step(.8, radius / 29.0 / length(p - light[int(i)])) * base_color, t/5.0);
    }
    gl_FragColor = dest;
}

float to_rad(float progress){
	// 0 <= progress <= 1
	return 2.0 * PI * progress;
}

void draw_circle(float progress, vec2 p){
	mat2 rotate = mat2(cos(PI/2.0), -sin(PI/2.0), sin(PI/2.0), cos(PI/2.0));
	p *= rotate;
	float cur_angle = -sign(p.y) * acos(p.x / length(p)) + PI; 
	float color_scale = step(cur_angle, to_rad(progress));
	
	gl_FragColor = gl_Color + vec4(step(0.9, .1 / abs((length(p) - (radius * 0.8)))) * color_scale * base_color, 1.0);
}

void main(void){
    vec2 p = (gl_FragCoord.xy * 2.0 - r) / min(r.x, r.y);
    draw_point(p);
    draw_circle(load_progress, p);
}
