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

// SPOTLIGHT
uniform vec3 spotPosition;
uniform vec3 spotDirection;

uniform float spotInnerCutOff;
uniform float spotOuterCutOff;

uniform float spotIntensity;

uniform float spotRange;



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
       
       float spotLight = 0;

       // calculamos la direccion de la luz de la spotlight respecto al tri affectado
       vec3 lightDirection = normalize(spotPosition - gPrimitivePosition.xyz);

       // calculamos la distancia entre luz y tri para el range
       float lightDistance = length(spotPosition - gPrimitivePosition.xyz);
        
       float theta = dot(lightDirection, - spotDirection);
       // calculamos si el tri esta dentro o fuera del radio
       if(theta > spotOuterCutOff) 
       {
            // cambiamos la intensidad segun si estan dentro o fuera del InnerCutOff
            float epsilon = spotInnerCutOff - spotOuterCutOff;
            float intensity = clamp((theta - spotOuterCutOff) / epsilon, 0.0, 1.0);

        //IA Prompt: Explain me the formula fot ligth attenuation
        //Ia response:
        //Physical Inverse-Square Law
        //In physics, light radiating from a single point spreads spherically over an area,
        //meaning its intensity drops proportionally to the square of the distance:
        //  (I(d) = I₀ / d^2) Where:
        // I(d) = Intensity at distance d, 
        // I₀ = Initial light intensity, 
        // d = Distance from the light source
        // A partir de esto, podemos calcular que:

        float Id = spotIntensity / (lightDistance * lightDistance);

    
            // 0 - 1 segun lo lejos que esta el tri de la luz segun el range
            float clampDistance = clamp(1.0 - lightDistance / spotRange, 0.0, 1.0); 

            // multiplicador externo * inensidad segun si esta fuera o dentro del inner * intensidad segun la distancia 
            spotLight = intensity * Id * clampDistance;
       }

        // suma de todas las luces
        vec3 lighting = ambient + diffuse + spotLight;
     

        fragColor = vec4(baseColor.rgb * lighting, 1.0) * uTint;
    }
    else
    {
            fragColor = baseColor * uTint;
    }
}