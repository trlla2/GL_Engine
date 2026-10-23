#version 440 core

in vec2 gTexCoord;
in vec3 gNormal;
in vec4 gPrimitivePosition;

out vec4 fragColor;

uniform sampler2D uTexture;
uniform vec2 windowSize;
uniform vec4 uTint;
uniform bool uHasTexture;
uniform bool uEnableLighting;

// Luz direccional del sol
uniform vec3 uSunDirection;
uniform vec3 uSunColor;

// Luz direccional de la luna
uniform vec3 uMoonDirection;
uniform vec3 uMoonColor;

// Luz ambiental
uniform vec3 lightColor;
uniform float ambientStrength;

// Linterna (spotlight)
uniform vec3 spotPosition;
uniform vec3 spotDirection;
uniform float spotInnerCutOff;
uniform float spotOuterCutOff;
uniform float spotIntensity;
uniform float spotRange;

void main()
{
    vec3 normal = normalize(gNormal);

    vec4 baseColor = vec4(1.0);
    if (uHasTexture)
    {
        baseColor = texture(uTexture, gTexCoord);
    }

    if (uEnableLighting)
    {
        // Minimo de 0.15 para que las caras en sombra no queden completamente negras
        vec3 SunDirection = normalize(uSunDirection);
        float SunLightAngle = max(dot(normal, SunDirection), 0.15);
        vec3 sunDiffuse = SunLightAngle * uSunColor;

        vec3 MoonDirection = normalize(uMoonDirection);
        float MoonLightAngle = max(dot(normal, MoonDirection), 0.15);
        vec3 moonDiffuse = MoonLightAngle * uMoonColor;

        vec3 ambient = ambientStrength * lightColor;
        vec3 diffuse = sunDiffuse + moonDiffuse;

        float spotLight = 0;

        vec3 lightDirection = normalize(spotPosition - gPrimitivePosition.xyz);
        float lightDistance = length(spotPosition - gPrimitivePosition.xyz);

        float theta = dot(lightDirection, - spotDirection);
        if(theta > spotOuterCutOff)
        {
            // Transicion suave entre el cono interior y el exterior
            float epsilon = spotInnerCutOff - spotOuterCutOff;
            float intensity = clamp((theta - spotOuterCutOff) / epsilon, 0.0, 1.0);

            // Ley del inverso del cuadrado: I = I0 / d^2
            float Id = spotIntensity / (lightDistance * lightDistance);

            // Atenuacion lineal hasta cero al llegar a spotRange
            float clampDistance = clamp(1.0 - lightDistance / spotRange, 0.0, 1.0);

            spotLight = intensity * Id * clampDistance;
        }

        vec3 lighting = ambient + diffuse + spotLight;

        fragColor = vec4(baseColor.rgb * lighting, 1.0) * uTint;
    }
    else
    {
        fragColor = baseColor * uTint;
    }
}
