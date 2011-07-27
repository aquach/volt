uniform sampler2D lightMap;
uniform vec2 offset;

/* Implements a separated 5x5 Gaussian blur filter, using 3 samples instead of
 * 5. See http://prideout.net/archive/bloom/index.php */

float gaussianCoeff[3] = {
    5.0, 6.0, 5.0
};

void main() {
    vec2 texCoords = gl_TexCoord[0].st;

    vec4 color;
    color += gaussianCoeff[0] * texture2D(lightMap, texCoords - offset);
    color += gaussianCoeff[1] * texture2D(lightMap, texCoords);
    color += gaussianCoeff[2] * texture2D(lightMap, texCoords + offset);

    color /= 16.0f;

    gl_FragColor = color;
}
