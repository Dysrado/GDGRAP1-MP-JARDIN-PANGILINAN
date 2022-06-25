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
        vec3 lightDir = normalize(lightPos - fragPos);
        vec3 normal = normalize(normCoord);

        if(lightType == 1){
            float cutoff = cos(radians(12.5f));
            float theta = dot(lightDir, normalize(-lightDirection));
            float epsilon = (cos(radians(12.5f)) - cos(radians(17.5f)));
            float intensity = clamp((theta - cos(radians(17.5f))) / epsilon, 0.0, 1.0);  
            if(theta > cutoff){
                float diff = max(dot(normal, lightDir),0.0);
                vec3 diffuse = diff* lightColor;
    
                vec3 ambientCol = ambientColor*ambientStr;

                vec3 viewDir = normalize(cameraPos - fragPos);
                vec3 reflectDir = reflect(-lightDir, normal);
                float spec = pow(max(dot(reflectDir, viewDir), 0.1),specPhong);
                vec3 specColor = spec * specStr * lightColor;
               
                specColor *= intensity;
                diffuse *= intensity;
                FragColor = vec4(specColor + ambientCol + diffuse, 1.0) * texture(tex0,texCoord);
            }
            else{
             FragColor = vec4(0,0,0,0);
            }
        }
    
        if(lightType == 0){
            float diff = max(dot(normal, lightDir),0.0);
            vec3 diffuse = diff* lightColor;
    
            vec3 ambientCol = ambientColor*ambientStr;

            vec3 viewDir = normalize(cameraPos - fragPos);
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(reflectDir, viewDir), 0.1),specPhong);
            vec3 specColor = spec * specStr * lightColor;
            /*Attenuation*/ 
            float distance = length(lightPos - fragPos); //Distance from the Light and Object
            float intensity = clamp(10.0 / distance, 0.0, 1.0); // Formula of intensity given
    
            /*Incorporating the intensity to the light*/
            specColor *= intensity;
            ambientCol *= intensity;
            diffuse *= intensity;
            FragColor = vec4(specColor + ambientCol + diffuse, 1.0) * texture(tex0,texCoord);
        }
    
    }

}