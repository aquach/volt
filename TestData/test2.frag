uniform sampler2D colorMap;

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 colorMapColor = texture2D(colorMap, texCoords);
    vec4 c2 = texture2D(colorMap, vec2(texCoords.y, -texCoords.x));
    vec4 color = colorMapColor * 0.5 + c2 * 0.5;

    gl_FragColor = color;
}
