uniform sampler2D colorMap;

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    vec4 color = texture2D(colorMap, texCoords);
    color *= sin(texCoords.x * 1024) * sin(texCoords.y * 768) * 3;

    gl_FragColor = color;
}
