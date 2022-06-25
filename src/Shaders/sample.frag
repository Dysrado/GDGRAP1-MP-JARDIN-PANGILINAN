#version 330 core //Version
uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDirection;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

uniform int lightType;

uniform int lit;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor; //Returns a Color
//Simple shader that colors the model Red
void main()
{
    if (lit == 0){
     FragColor = texture(tex0, texCoord);
    }
    else if (lit == 1){
    vec3 lightDir;
    vec3 normal = normalize(normCoord);
     if (lightType == 0){
        lightDir = normalize(lightPos - fragPos);
    }
    else if(lightType == 1){
    

        lightDir = normalize(-lightDirection);
    }
    
    float diff = max(dot(normal, lightDir),0.0);
    vec3 diffuse = diff* lightColor;
    
    vec3 ambientCol = ambientColor*ambientStr;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.1),specPhong);
    vec3 specColor = spec * specStr * lightColor;

    if(lightType == 0){
        /*Attenuation*/ 
        float distance = length(lightPos - fragPos); //Distance from the Light and Object
       float intensity = clamp(10.0 / distance, 0.0, 1.0); // Formula of intensity given
    
        /*Incorporating the intensity to the light*/
        specColor *= intensity;
        ambientCol *= intensity;
        diffuse *= intensity;
    }
    FragColor = vec4(specColor + ambientCol + diffuse, 1.0) * texture(tex0,texCoord);
}
    
    


}