#version 410

// tell the shader where the vertices are in the vao
layout(location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection_matrix, view_matrix, model_matrix;

void main() {
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4 (vertex_position, 1.0);
    FragPos = vec3(model_matrix * vec4(vertex_position, 1.0));
    Normal = vertex_normal;
}
