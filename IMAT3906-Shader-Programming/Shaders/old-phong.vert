#version 410

//These come from binding our VAO and VBO in the Model class
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_textureUV;
layout (location = 3) in vec3 vertex_tangent;

//These are set as uniforms in the main.cpp file and then the model_matrix in the Model class
uniform mat4 projection_matrix, view_matrix, model_matrix;

//These are passed to the fragment shader
out vec3 position_eye;
out vec2 texture_coordinates;
out vec3 vertex_normal_passed;

out mat3 TBNMatrix;

void main () {
	//Calculate the Tangent, Bitangent, Normal matrix to be used to
	//convert the normal in the fragment shader from tangent space to
	//view space.
	vec3 T = normalize(vec3(model_matrix * vec4(vertex_tangent, 0.0)));
	vec3 N = normalize(vec3(model_matrix * vec4(vertex_normal, 0.0)));
	vec3 B = cross(N, T);

	TBNMatrix = mat3(T, B, N);

	vertex_normal_passed = vertex_normal;
	texture_coordinates = vertex_textureUV;
	position_eye = vec3 (view_matrix * model_matrix * vec4 (vertex_position, 1.0));
	gl_Position = projection_matrix * vec4 (position_eye, 1.0);
}
