#version 330 core //Version
uniform sampler2D tex0;
uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDirection;

uniform int lightType;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor; //Returns a Color
//Simple shader that colors the model Red
void main()
{
    vec3 lightDir;
    vec4 pixelColor = texture(tex0, texCoord);

    if(pixelColor.a < 0.1){
        discard;
    }


    //vec3 normal = normalize(normCoord);
    vec3 normal = texture(norm_tex, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    
    // computation for lighting
    if (lightType == 0){
        lightDir = normalize(lightPos - fragPos);
    }

   
    
    if(lightType == 0){
     float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambientCol = ambientColor * ambientStr;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    vec3 specColor = spec * specStr * lightColor;
     /*Attenuation*/ 
    float distance = length(lightPos - fragPos); //Distance from the Light and Object
    float intensity = clamp(10.0 / distance, 0.0, 1.0);

    /*Incorporating the intensity to the light*/
    specColor *= intensity;
    ambientCol *= intensity;
    diffuse *= intensity;
      FragColor = (vec4(specColor + ambientCol + diffuse, 1.0) * pixelColor);
    }
   
  
}