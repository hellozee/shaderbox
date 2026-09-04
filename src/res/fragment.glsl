#version 330 core

out vec4 fragColor;

uniform vec3 iResolution;
uniform float iTime;
uniform float iTimeDelta;
uniform int iFrame;
uniform vec4 iMouse;

void main() {
    vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
    vec3 color = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0.0, 2.0, 4.0));
    float ring = 0.02 / abs(length(uv) - 0.35 - 0.04 * sin(iTime * 2.0));
    fragColor = vec4(color * (0.35 + ring), 1.0);
}
