uniform sampler2D lightMap;
uniform vec2 pixelSize;

float gaussianCoeff1[9] = {
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
};
float gaussianCoeff2[25] = {
    1.0, 4.0, 6.0, 4.0, 1.0,
    4.0, 16.0, 24.0, 16.0, 4.0,
    6.0, 24.0, 36.0, 24.0, 6.0,
    4.0, 16.0, 24.0, 16.0, 4.0,
    1.0, 4.0, 6.0, 4.0, 1.0
};

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 color;
    int size = 2;
    for (int x = -size; x <= size; x++) {
        for (int y = -size; y <= size; y++) {
            vec4 pixel = texture2D(lightMap, texCoords + pixelSize * vec2(x, y));
            color += pixel * gaussianCoeff2[(x + 2) + (y + 2) * 5];
        }
    }
    //color *= 0.2;
    color /= 256.0f;
    //color = texture2D(lightMap, texCoords);
    color.a /= 4;

    gl_FragColor = color;
}
