#version 440 core

// fragment shader con textura y tint multiplicador
in vec2 gTexCoord; // UVs recibidas del geometry shaders
in vec3 gNormal; // normales recibidas del geometry shader
in vec4 gPrimitivePosition;// posici�n del centro del triangulo recibida del geometry shader

out vec4 fragColor;

uniform sampler2D uTexture; // textura de la instancia
uniform vec2 windowSize;
uniform vec4 uTint; // tinte de color de la trexura
uniform bool uHasTexture; // indica si la instancia tiene textura o no
uniform bool uEnableLighting; // indica si la instancia se ilumina, false para moon y sun

// Sun Directional Light
uniform vec3 uSunDirection;
uniform vec3 uSunColor;

// Moon Directional Light
uniform vec3 uMoonDirection;
uniform vec3 uMoonColor;

// Ambient Light
uniform vec3 lightColor;
uniform float ambientStrength;

// Spotlight
uniform vec3 spotlightPos;
uniform vec3 spotlightDir;

uniform vec3 spotlightColor;
uniform float spotlightIntensity;
uniform float spotlightRange;

uniform float spotlightInnerCutOff;
uniform float spotlightOuterCutOff;

void main()
{
    vec3 normal = normalize(gNormal);

    //assignar tetura si la tinene, sino color blanco
    vec4 baseColor = vec4(1.0);
    if (uHasTexture) 
    {
    // la textura ya viene volteada en Y al cargarla, no hay que volver a invertirla aqui
        baseColor = texture(uTexture, gTexCoord);
    }
    
    if (uEnableLighting) 
    {
        //sol
        vec3 SunDirection = normalize(uSunDirection);
        float SunLightAngle = max(dot(normal, SunDirection), 0.15); //clap a 0.15 para evitar que quede completamente oscuro
        vec3 sunDiffuse = SunLightAngle * uSunColor;

        //luna
        vec3 MoonDirection = normalize(uMoonDirection);
        float MoonLightAngle = max(dot(normal, MoonDirection), 0.15);
        vec3 moonDiffuse = MoonLightAngle * uMoonColor;

        vec3 ambient = ambientStrength * lightColor;
        //calculamos el diffuse total
        vec3 diffuse = sunDiffuse + moonDiffuse;

        // Spotlight calculations
        vec3 toLight = spotlightPos - gPrimitivePosition.xyz; 
        float dist = length(toLight);
        vec3 L = toLight / max(dist, 0.0001);

        // angulo entre direccion y el objeto
        float theta = dot(-L, normalize(spotlightDir));

        // calcular cono de luz con smoothstep
        float cone = smoothstep(spotlightOuterCutOff, spotlightInnerCutOff, theta);

        // Ley del cuadrado inverso para atenuacion realista,
        //Codigo hehco con IA para rangos de ~30 a 50 unidades, con mitigacion de corte abrupto al aproximarse al spotlightRange
        //Prompt: Aplicar el cuadrado inverso para hacer que la luz pierde intensidad según la distancia.
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float atenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));
    
        // Mitigación de corte abrupto: Desvanecemos a cero suavemente al aproximarse al spotlightRange (tmb IA)
        float rangeFade = clamp(1.0 - dist / spotlightRange, 0.0, 1.0);
        atenuation *= rangeFade;

        float diff = max(dot(normal, L), 0.0);
        vec3 spot = spotlightColor * spotlightIntensity * diff * cone * atenuation;


        // suma de todas las luces
        vec3 lighting = ambient + diffuse + spot;
     

        fragColor = vec4(baseColor.rgb * lighting, 1.0) * uTint;
    }
    else
    {
            fragColor = baseColor * uTint;
    }
}