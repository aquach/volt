uniform sampler2D lightMap;

float GetLightMapDistanceH(vec2 texCoords) {
    float u = texCoords.x;
    float v = texCoords.y;

    u = abs(u - 0.5f) * 2.0f;
    v = v * 2.0f - 1.0f;
    float v0 = v / u;
    v0 = (v0 + 1.0f) / 2.0f;

    vec2 newCoords = vec2(texCoords.x, v0);
    // Horizontal info was stored in the R component.
    return tex2D(lightMap, newCoords).r;
}

float GetLightMapDistanceV(vec2 texCoords) {
    float u = texCoords.y;
    float v = texCoords.x;

    u = abs(u - 0.5f) * 2.0f;
    v = v * 2.0f - 1.0f;
    float v0 = v / u;
    v0 = (v0 + 1.0f) / 2.0f;

    vec2 newCoords = vec2(texCoords.y, v0);
    // Vertical info was stored in the Green component.
    return tex2D(lightMap, newCoords).g;
}

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    float distance = length(texCoords - 0.5f);
    distance -= 0.005f;

    // Convert to [-1, 1].
    float x = 2.0f * (texCoords.x - 0.5f);
    float y = 2.0f * (texCoords.y - 0.5f);

    float lightMapDistance;
    if (abs(y) < abs(x)) {
        lightMapDistance = GetLightMapDistanceH(texCoords);
    } else {
        lightMapDistance = GetLightMapDistanceV(texCoords);
    }

    if (distance < lightMapDistance) {
        vec4 result = vec4(1.0f - distance * distance);
        result.a = 1.0f;
        gl_FragColor = result;
    } else {
        discard;
    }
}
