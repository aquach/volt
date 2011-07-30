uniform sampler2D colorMap;
uniform sampler2D paperMap;
uniform float distortionFactor;

uniform bool useAlpha;
uniform float alpha;

uniform bool useCenterGlow;

uniform bool lightningOn;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightningStart;
uniform vec3 lightningTarget;
uniform float ticksSinceLightningStart;

uniform vec2 paperOffset;
uniform vec2 pixelSize;

varying vec2 lightningStartScreenPos;
varying vec2 lightningEndScreenPos;

float kernel[9] = {
        1.0, 2.0, 1.0,
        0.0, 0.0, 0.0,
        -1.0, -2.0, -1.0
};

vec4 sobelFilter (vec2 pos) {
        vec4 midColor = texture2D(colorMap, pos);
        //if (dot(normalize(midColor), normalize(vec4(1.0, 1.0, 1.0, 1.0))) > 0.9)
        //      return 0;

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

        float M = 0.8; float D = 0.005 + distortionFactor;
        vec2 colorMapCoords = texCoords + ((texture2D(paperMap, texCoords + paperOffset) - M / 2) * D);

    vec4 colorMapColor = texture2D(colorMap, colorMapCoords);
        vec4 paperMapColor = texture2D(paperMap, colorMapCoords * 4 + paperOffset);

        // Change paper map based on how far you are from center
        vec2 distVec = vec2(0.5, 0.5) - texCoords;
        paperMapColor = paperMapColor * (1 - length(distVec) * 1.25);

    vec4 sobelFilterColor = sobelFilter(colorMapCoords);

    // Change color map based on lightning position
    float lightningMult = 1.0;
        if (lightningOn) {
                vec4 ls = projectionMatrix * modelViewMatrix * vec4(lightningStart, 1.0);
                vec4 lt = projectionMatrix * modelViewMatrix * vec4(lightningTarget, 1.0);

                vec2 lightningStartScreenPos = ls.xy / ls.w;
                vec2 lightningTargetScreenPos = lt.xy / lt.w;

                lightningStartScreenPos = (lightningStartScreenPos + 1) * 0.5;
                lightningTargetScreenPos = (lightningTargetScreenPos + 1) * 0.5;

                vec2 w = normalize(lightningTargetScreenPos - lightningStartScreenPos);
                vec2 v = texCoords - lightningStartScreenPos;

                vec2 proj = dot(v, w) * w;

                float t = (texCoords.x - lightningStartScreenPos.x) * (lightningTargetScreenPos.x - lightningStartScreenPos.x)
                                + (texCoords.y - lightningStartScreenPos.y) * (lightningTargetScreenPos.y - lightningStartScreenPos.y);

                t /= pow(length(lightningTargetScreenPos - lightningStartScreenPos), 2.0);

                float dist = length(v - proj);

                // Make the middle glow and fall off based on distance
                lightningMult = 1.5 - dist * 2;

                // Make the middle part of the bolt glow more than the ends
                float lengthDist = abs(t - 0.5); // Distance from middle of the point to end
                lightningMult *= clamp(1 - lengthDist * 0.5, 0, 1);

                lightningMult = clamp(lightningMult, 0.4, 100);

                // Animate multiplier based on when tick started
                // Start with a flash and quickly dwindle down to normal
                lightningMult *= clamp(4 - pow(ticksSinceLightningStart * 0.01, 2.0), 1, 100);
        }

        colorMapColor *= lightningMult;

        float W = 1.0; float P = 0.05; float S = 0.35;
        vec4 color = (1 - W) + colorMapColor * W - paperMapColor * P - sobelFilterColor * S;

        if (useAlpha) {
                color = alpha * color + (1 - alpha);
        }

        if (useCenterGlow) {
                float dist = clamp(pow(length(distVec), 0.25), 0.0, 1.0);
                color = color * dist + (1 - dist);
        }

        gl_FragColor = color;
}
