uniform sampler2D depthMap;

float GetDistance (vec2 texCoords) {
    vec4 shadowMapColor = texture2D(depthMap, texCoords);

    // For non-white pixels, compute distance from light.
    if (shadowMapColor.r > 0.9f) {
        return 1;
    }
    vec2 dir = texCoords - 0.5f;
    float dist = length(dir);

    return dist;
}

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    // Translate u and v into [-1, 1] domain.
    float u0 = texCoords.x * 2.0f - 1;
    float v0 = texCoords.y * 2.0f - 1;

    // As u0 approaches 0 (the center), v should also approach 0.
    v0 = v0 * abs(u0);
    // Convert back from [-1, 1] domain to [0, 1] domain.
    v0 = (v0 + 1) / 2.0f;

    vec2 newCoords = vec2(texCoords.x, v0);
    float horizontal = GetDistance(newCoords);
    float vertical = GetDistance(newCoords.yx);

    gl_FragColor = vec4(horizontal, vertical, 0, 1);
}
