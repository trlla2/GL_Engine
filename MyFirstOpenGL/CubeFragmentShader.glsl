#version 440 core

// fragment shader con textura y tint multiplicador
in vec2 gTexCoord; // UVs recibidas del geometry shaders
in vec3 gNormal; // normales recibidas del geometry shader
in vec4 gPrimitivePosition;// posición del centro del triángulo recibida del geometry shader

out vec4 fragColor;

uniform sampler2D uTexture; // textura de la instancia
uniform vec2 windowSize;
uniform vec4 uTint; // tinte de color de la trexura

// Directional light
uniform vec3 ligthSource;
uniform vec3 lightColor;
uniform float ambientStrength;

void main()

{
    vec2 adjustedTexCoord = vec2(gTexCoord.x, 1.0 - gTexCoord.y);
    vec4 baseColor = texture(uTexture, adjustedTexCoord);

    vec3 lightDirection = normalize(ligthSource);
    float sourceLightAngle = dot(gNormal, lightDirection);

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = sourceLightAngle * lightColor;

    fragColor = vec4(baseColor.rgb * (ambient + diffuse), 1.0) * uTint;
}