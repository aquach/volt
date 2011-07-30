uniform sampler2D parabolicMap;
uniform vec2 pixelSize;

void main() {
    // Reduce the left and right halves of the texture into a single column
    // with the pixel value being the closest occluder's distance.
    vec2 texCoords = gl_TexCoord[0].st;
    vec2 minDist;
    if (texCoords.x < 0.4f) {
        float x = 0;
        minDist = texture2D(parabolicMap, vec2(x, texCoords.y)).rg;
        while (x < 0.5f) {
            minDist = min(minDist, texture2D(parabolicMap, vec2(x, texCoords.y)).rg);
            x += pixelSize.x;
        }
    } else {
        float x = 1;
        minDist = texture2D(parabolicMap, vec2(x, texCoords.y)).rg;
        while (x >= 0.5f) {
            minDist = min(minDist, texture2D(parabolicMap, vec2(x, texCoords.y)).rg);
            x -= pixelSize.x;
        }
    }

    gl_FragColor = vec4(minDist.r, minDist.g, 0, 1);
}
