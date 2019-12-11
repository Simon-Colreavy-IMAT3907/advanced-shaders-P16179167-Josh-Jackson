#version 410

//These come from the vertex shader
in vec3 position_eye;
in vec3 vertex_normal_passed;
in vec2 texture_coordinates;
in mat3 TBNMatrix;

//These are set as uniforms from the C++ code
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform vec3 light_position_world;
uniform vec3 light_colour_specular;
uniform vec3 light_colour_diffuse;
uniform vec3 light_colour_ambient;

//GLSL Sampler to retrieves diffuseTexels from a texture
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

uniform float useNormalMaps;

//Surface Reflectance Vectors (Will be set by the relevant textures/diffuseTexels)
vec3 surface_specular = vec3 (1.0, 1.0, 1.0); //Fully reflect specular light
vec3 surface_ambient = vec3 (1.0, 1.0, 1.0); //Fully reflect ambient light
vec3 surface_diffuse = vec3 (1.0, 1.0, 1.0); //Fully reflect diffuse light

vec3 normal; //The actual normal
vec3 normal_eye; //The normal as viewed from the camera's perspective.

float specular_exponent = 32.0; //Specular Power

out vec4 fragment_colour; //Final colour of surface

void main () {

	//--- GET TEXTURE INFORMATION ---

	//Get the texels from our texture coordinates
	vec4 diffuseTexel = texture(diffuse_texture, texture_coordinates);
	vec4 specularTexel = texture(specular_texture, texture_coordinates);
	vec4 normalTexel = texture(normal_texture, texture_coordinates);
	
	//Set the ambient and diffuse reflectance by the rgb values of the diffuseTexels
	surface_ambient = diffuseTexel.rgb;
	surface_diffuse = diffuseTexel.rgb;

	//Set the specular reflectance by the rgb values of the specularTexels
	surface_specular = specularTexel.rgb;

	//Set normals by the rgb values of the normalTexels.
    normal = normalTexel.rgb;

    //Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);

	//Convert the normal from tangent space to world space.
	normal = normalize(TBNMatrix * normal);

	normal_eye = vec3 (view_matrix * model_matrix * vec4 (vertex_normal_passed, 0.0));

	if (useNormalMaps != 0) {
		normal_eye = vec3 (view_matrix * model_matrix * vec4 (normal, 0.0));
	}


	//--- CALCULATE AMBIENT INTENSITY ---

	//Using the colour of the ambient light and the reflectance values of the surface_diffuse
	//and surface_ambient, calculate the overall ambient intensity of a pixel/fragment.
	vec3 ambient = light_colour_ambient * surface_diffuse * surface_ambient;


	//--- CALCULATE DIFFUSE INTENSITY ---

	//1. Calculate the direction vector between the light source and the pixel/fragment's position
	//Raise the light position to where the 'eye' is.
	//Calculate the direction vector by subtracting both vectors.
	//Normalise the resulting direction vector.
	vec3 light_position_eye = vec3 (view_matrix * vec4 (light_position_world, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye; 
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);

	//2. Calculate the atual diffuse intensity the light has on the current pixel/fragment.
	//Take the dot product of the normal and the light direction vector.
	float dot_prod = dot (direction_to_light_eye, normal_eye);
	dot_prod = max (dot_prod, 0.0);
	
	//Multiply the result of the dot product by the diffuse colour of the light
	//and the diffuse colour of the surface. This should now result in a darker
	//diffuse intensity the greater the angle is between the vectors.
	vec3 diffuse = light_colour_diffuse * surface_diffuse * dot_prod; //Final Diffuse Intensity
	
	//--- CALCULATE SPECULAR INTENSITY ---

	//The specular lighting is based on the light's direction vector and the
	//object's normal vectors as well as the view direction (eg. what direction is the player
	//looking at the fragment from)

	//1. Calculate a reflection vector by reflecting the light direction around the normal vector
	vec3 reflection_eye = reflect (-direction_to_light_eye, normal_eye);

	//2. Get the view direction as a unit vector
	vec3 surface_to_viewer_eye = normalize (-position_eye);

	//3. Calculate the angular distance between the reflection vector and the view direction
	//and the closer the angle between them, the higher the intensity.

	// Phong
	// float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);
	// dot_prod_specular = max (dot_prod_specular, 0.0);
	// float specular_factor = pow (dot_prod_specular, specular_exponent);

	// Blinn-Phong
	//Instead of using a reflection vector blinn-phong uses a half-way vector which is a unit vector 
	//that is exactly halfway between the view direction and the light direction. The closer the
	//halfway vector aligns with the surface's normal vector, the highest the specular factor.
	vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
	float dot_prod_specular = max (dot (half_way_eye, normal_eye), 0.0);
	float specular_factor = pow (dot_prod_specular, specular_exponent);

	//4. Calculate the final specular intensity by multiplying the light's specular colour
	//with the specular colour of the surface and the specular strength (factor)
	vec3 specular = light_colour_specular * surface_specular * specular_factor;

	
	//Final colour of the fragment
	fragment_colour = vec4 (ambient + diffuse + specular, 1.0);
}
