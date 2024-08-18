#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 color;

out vec3 v_Normal;
out vec4 v_Pos;
out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
    // Transformar a posição e a normal dos vértices
    vec4 worldPosition = u_Model * position;
    gl_Position = u_Projection * worldPosition;
    v_Pos = gl_Position;
    v_Normal = mat3(u_Model) * normal.xyz; // Transforma a normal
    v_Color = color;
}


#shader fragment
#version 330 core

layout (location = 0) out vec4 fragColor;

in vec3 v_Normal;
in vec4 v_Pos;
in vec4 v_Color;

uniform vec3 lightPos = vec3(0.1, -0.7, -0.2); // Posição fixa da luz

void main()
{
    // Normalização das normais
    vec3 norm = normalize(v_Normal);

    // Direção da luz
    vec3 lightDir = normalize(lightPos);

    // Componente difusa
    float diff = max(dot(norm, lightDir), 0.0);

    // Intensidade da luz
    float intensity = (diff * 0.5 + 0.5);

    // Cor final
    fragColor = vec4(v_Color.rgb * intensity, 1.0);
}