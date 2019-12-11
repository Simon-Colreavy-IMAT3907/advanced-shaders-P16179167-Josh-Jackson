#version 330 core

out vec4 fragment_color;

//These come from the vertex shader
in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 vertexNormal;

//Textures setup in Model class
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 light_position_world;
uniform vec3 viewPos;
uniform float useNormalMaps;

float specularExponent = 32.0;

void main() {
    //Normals
    //1.Get normal rgb values from the texture file or from the vertex
    //  shader if useNormalMaps is set to false.
    //2.Transform the normal vector to range (-1, 1)
    vec3 normal;

    if (useNormalMaps == 1) {
        normal = texture(normalMap, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }
    else {
        normal = vertexNormal;
    }


    //Diffuse Lighting Colour
    vec3 diffuseColor = texture(diffuseMap, TexCoords).rgb;
    
    //Ambient Lighting, general lighting based on diffuse lighting colour
    vec3 ambient = 0.1 * diffuseColor;

    //Diffuse Lighting
    //1. Calculate the direction vector between the light source and the pixel/fragment's position
	//Calculate the direction vector by subtracting both vectors.
	//Normalise the resulting direction vector.
    //2. Calculate the actual diffuse intensity the light has on the current pixel/fragment.
	//  by taking the dot product of the normal and the light direction vector.
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * diffuseColor;

    //Specular Lighting
    //The specular lighting is based on the light's direction vector and the
	//object's normal vectors as well as the view direction (eg. what direction is the player
	//looking at the fragment from)

	//1. Calculate a reflection vector by reflecting the light direction around the normal vector
    //2. Get the view direction as a unit vector
    //3. Calculate a half-way vector which is a unit vector 
	//that is exactly halfway between the view direction and the light direction. The closer the
	//halfway vector aligns with the surface's normal vector, the highest the specular factor.
    //4. Calculate the final specular intensity by multiplying the light's specular colour
	//with the specular colour of the surface and the specular strength (factor)
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), specularExponent);
    vec3 specular = vec3(0.2) * specularFactor;

    //Final colour of the fragment
    fragment_color = vec4(ambient + diffuse + specular, 1.0);
}
