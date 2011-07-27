uniform sampler2D pass0;
uniform sampler2D pass1;
uniform sampler2D pass2;
uniform sampler2D pass3;

vec4 GetColor (vec2 texCoords) {
    vec2 dir = texCoords - 0.5f;
    float distance = length(dir);

    float value = (1.0 - distance * distance * 4);
    vec4 sum = texture2D(pass0, texCoords) +
               texture2D(pass1, texCoords) +
               texture2D(pass2, texCoords) +
               texture2D(pass3, texCoords);

    vec4 color = sum * value * 0.25f;
    return color;
}

void main() {
    vec2 texCoords = gl_TexCoord[0].st;
    gl_FragColor = GetColor(texCoords);
}
