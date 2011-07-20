uniform sampler2D shadowMap;
uniform vec4 color;
uniform float coneAngle; // Width of the cone of light that the light emits.
uniform vec2 lightDir; // Direction that the light shines in.

float GetLightMapDistanceH(vec2 texCoords) {
    float u = texCoords.x;
    float v = texCoords.y;

    u = abs(u - 0.5f) * 2.0f;
    v = v * 2.0f - 1.0f;
    float v0 = v / u;
    v0 = (v0 + 1.0f) / 2.0f;

    vec2 newCoords = vec2(texCoords.x, v0);
    // Horizontal info was stored in the R component.
    return texture2D(shadowMap, newCoords).r;
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
    return texture2D(shadowMap, newCoords).g;
}

float DEG2RAD = 3.1415 / 180;

vec4 GetColor (vec2 texCoords) {
    vec2 dir = texCoords - 0.5f;
    float distance = length(dir);

    // Convert to [-1, 1].
    float x = 2.0f * (texCoords.x - 0.5f);
    float y = 2.0f * (texCoords.y - 0.5f);

    float cutoff = coneAngle * DEG2RAD / 2;
    if (acos(dot(dir / distance, lightDir)) > cutoff) {
        return vec4(0, 0, 0, 0);
    }

    distance -= 1.0 / 512;

    float lightMapDistance;
    if (abs(y) < abs(x)) {
        lightMapDistance = GetLightMapDistanceH(texCoords);
    } else {
        lightMapDistance = GetLightMapDistanceV(texCoords);
    }

    if (distance < lightMapDistance) {
        float mult = 1.0 - distance * distance * 4;
        vec4 result = color * mult;
        return result;
    } else {
        return vec4(0, 0, 0, 0);
    }
}

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec2 pixelSize = vec2(2.0 / 512.0);
    gl_FragColor = GetColor(texCoords);
}
