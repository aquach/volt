uniform sampler2D shadowMap;

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 shadowMapColor = texture2D(shadowMap, texCoords);

    // For non-white pixels, compute distance from light.
    if (shadowMapColor.r > 0.9f) {
        gl_FragColor = vec4(1, 0, 0, 1);
        return;
    }
    vec2 dir = texCoords - 0.5f;
    float dist = length(dir);

    gl_FragColor = vec4(dist, 0, 0, 1);
}
