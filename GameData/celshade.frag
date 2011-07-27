uniform sampler2D colorMap;
uniform vec2 pixelSize;

vec4 sobelFilter (vec2 pos) {
	vec4 color;

	vec4 c1 = texture2D(colorMap, pos - pixelSize * vec2(-1, -1));
	vec4 c2 = texture2D(colorMap, pos - pixelSize * vec2(0, -1));
	vec4 c3 = texture2D(colorMap, pos - pixelSize * vec2(1, -1));
	vec4 c4 = texture2D(colorMap, pos - pixelSize * vec2(-1, 0));
	vec4 c5 = texture2D(colorMap, pos - pixelSize * vec2(1, 0));
	vec4 c6 = texture2D(colorMap, pos - pixelSize * vec2(-1, 1));
	vec4 c7 = texture2D(colorMap, pos - pixelSize * vec2(0, 1));
	vec4 c8 = texture2D(colorMap, pos - pixelSize * vec2(-1, 1));

	vec4 xSum = c1 + c6 + 2 * (c4 - c5) - c3 - c8;
	vec4 ySum = c1 + c3 + 2 * (c2 - c7) - c6 - c8;

	return abs(xSum) + abs(ySum);
}

void main() {
	vec2 texCoords = gl_TexCoord[0].st;
	gl_FragColor = sobelFilter(texCoords);
}
