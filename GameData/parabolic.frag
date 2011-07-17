uniform sampler2D distanceMap;

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
    float horizontal = texture2D(distanceMap, newCoords).r;
    float vertical = texture2D(distanceMap, newCoords.yx).r;

    gl_FragColor = vec4(horizontal, vertical, 0, 1);
}
