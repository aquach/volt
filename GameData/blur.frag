uniform sampler2D lightMap;
uniform vec2 pixelSize;

float gaussianCoeff[9] = {
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
};

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 color;
    int size = 1;
    for (int x = -size; x <= size; x++) {
        for (int y = -size; y <= size; y++) {
            vec4 pixel = texture2D(lightMap, texCoords + pixelSize * vec2(x, y));
            color += pixel * gaussianCoeff[(x + 1) + (y + 1) * 3];
        }
    }
    //color *= 0.2;
    color /= 16.0f;
    //color = texture2D(lightMap, texCoords);

    gl_FragColor = color;
}
