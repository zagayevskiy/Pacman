#ifdef GL_ES
    precision mediump float;
#endif
varying vec4 vColor;
varying vec2 vPosition;
uniform float uBrushSize;
void main(){
    float x = vPosition.x;
    float y = vPosition.y;
    float nX = floor(x * 4.0);
    float nY = floor(y * 4.0);
    float centerX = nX / 4.0 + 1.0 / 8.0;
    float centerY = nY / 4.0 + 1.0 / 8.0;
    float distX = abs(x - centerX);
    float distY = abs(y - centerY);
    float distance = sqrt(distX*distX + distY*distY);
    gl_FragColor = vec4(1.0, 0.1, 0.0, 1.0*(1.0 - distance / (1.0/(8.0 + (nX + (nY*4.0))))));
};