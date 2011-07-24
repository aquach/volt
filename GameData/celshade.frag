uniform sampler2D colorMap;

uniform sampler2D paperMap;

float kernel[9];

vec4 sobelFilter (vec2 pos) {
	 kernel[0] = 1.0;
	 kernel[1] = 2.0;
	 kernel[2] = 1.0;
	 kernel[3] = 0.0;
	 kernel[4] = 0.0;
	 kernel[5] = 0.0;
	 kernel[6] = -1.0;
	 kernel[7] = -2.0;
	 kernel[8] = -1.0;

	 vec2 pixelSize = vec2(1.0f /1024.0f, 1.0f /768.0);

	vec4 midColor = texture2D(colorMap, pos);
	//if (dot(normalize(midColor), normalize(vec4(1.0, 1.0, 1.0, 1.0))) > 0.9)
	//	return 0;

	vec4 xSum;
	vec4 ySum;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			vec4 color = texture2D(colorMap, pos + pixelSize * vec2(x, y));
			xSum += color * kernel[(x + 1) * 3 + y + 1];
			ySum += color * kernel[x + 1 + (y + 1) * 3];
		}
	}

	return abs(xSum) + abs(ySum);
}

void main() {
	vec2 texCoords = gl_TexCoord[0].st;

	float M = 1; float D = 0.000;
	vec2 colorMapCoords = texCoords + ((texture2D(paperMap, texCoords).st - M * 0.5) * D);

    vec4 colorMapColor = texture2D(colorMap, colorMapCoords);
    vec4 sobelFilterColor = sobelFilter(colorMapCoords);

	float W = 1.0; float S = 0.1;
   	vec4 color = (1.0 - W) + colorMapColor * W;

	float intensity = length(sobelFilterColor);
	if (intensity > 0.2)
		color -= vec4(intensity) * S;

	gl_FragColor = color;
}
