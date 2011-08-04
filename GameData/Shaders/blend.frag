uniform sampler2D colorMap;
uniform vec4 blendColor;
uniform float amount;

void main() {
	vec2 texCoords = gl_TexCoord[0].st;
    vec4 color = texture2D(colorMap, texCoords);        
	gl_FragColor = (1 - amount) * color + amount * blendColor;
}
