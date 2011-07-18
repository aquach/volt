uniform sampler2D lightMap;
uniform vec2 pixelSize;

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 color;
    int size = 1;
    float r = 3.0;
    for (int x = -size; x <= size; x++) {
        for (int y = -size; y <= size; y++) {
            float weight = exp(-(x * x + y * y) / (r * r));
            color += texture2D(lightMap, texCoords + pixelSize * vec2(x, y)) * weight;
        }
    }
    color *= 0.2;
    //color /= pow(size * 2 + 1, 2);
    //color = texture2D(lightMap, texCoords);

    gl_FragColor = color;
}
