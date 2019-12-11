#version 330 core

//These come from binding our VAO and VBO in the Model class
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_textureCoords;
layout (location = 3) in vec3 vertex_tangent;

//These are passed to the fragment shader
out vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 vertexNormal;

//These are set as uniforms in the main.cpp file and then the model_matrix in the Model class
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

uniform vec3 light_position_world;

//The camera's position
uniform vec3 viewPos;

void main() {
    vertexNormal = vertex_normal;
    
    FragPos = vec3(model_matrix * vec4(vertex_position, 1.0));
    TexCoords = vertex_textureCoords;

    //Calculate the Tangent, Bitangent, Normal matrix to be used to
    //convert the normal in the fragment shader from tangent space to
    //view space.
    mat3 normalMatrix = transpose(inverse(mat3(model_matrix)));
    
    vec3 T = normalize(normalMatrix * vertex_tangent);
    vec3 N = normalize(normalMatrix * vertex_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * light_position_world;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;

    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);
}