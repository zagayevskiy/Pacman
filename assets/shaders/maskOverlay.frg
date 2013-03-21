precision mediump float;
varying vec2 vTexture;
uniform sampler2D uMap;
void main() {
  gl_FragColor = texture2D(uMap, vTexture);
};