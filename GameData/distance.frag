uniform sampler2D shadowMap;

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 shadowMapColor = texture2D(shadowMap, texCoords);
    float distance = shadowMapColor.r < 0.9f ? length(texCoords - 0.5f) : 1.0f;
    gl_FragColor = vec4(distance, 0, 0, 1);
}
