precision mediump float;
varying vec2 vTexture;
varying vec2 vMaskCoords;
uniform sampler2D uMap;
uniform sampler2D uMask;
uniform float uAlpha;
void main() {
    vec4 texColor = texture2D(uMap, vTexture);
    vec4 mask = texture2D(uMask, vMaskCoords);
    gl_FragColor = vec4(texColor.rgb, uAlpha*mask.a);
};