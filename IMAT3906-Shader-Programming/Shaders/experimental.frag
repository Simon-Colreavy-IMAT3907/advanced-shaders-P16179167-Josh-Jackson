#version 410

uniform vec3 object_colour;
uniform vec3 light_colour;
uniform vec3 light_position_world;
uniform vec3 view_position;

out vec4 frag_colour;

in vec3 Normal;
in vec3 FragPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position_world - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_colour;

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light_colour;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_position - FragPos);
    vec3 reflectDir = normalize(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_colour;

    vec3 result = (ambient + diffuse + specular) * object_colour;
	frag_colour = vec4 (result, 1.0);
}
