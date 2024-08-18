#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "RocketEquations.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static float window_width = 640;
static float window_height = 640;

static void FrameBuffer_Size_CallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    window_width = float(width);
    window_height = float(height);
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello Cruzeiro do Sul!", NULL, NULL);
    if (!window)
    {
        std::cout << "Falha ao criar a WINDOW!";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_CallBack);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao criar o GLAD!";
        glfwTerminate();
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    {
        Mesh mesh;
        mesh.LoadOBJ("res/meshes/REFPETCocacola.obj", false, true, true);
        std::vector<float> vertices = mesh.GetVertices();
        std::vector<unsigned int> indices = mesh.GetIndices();

        // Criar VAO e VBO
        VertexArrayID vaoID = VertexArrayManager::CreateBuffer();
        VertexBufferID vboID = VertexBufferManager::CreateBuffer();
        VertexBufferManager::AddData(vboID, &vertices[0], static_cast<unsigned int>(vertices.size() * sizeof(float)), GL_DYNAMIC_DRAW);

        // Configurar layout
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(3);
        VertexArrayManager::AddVertexBuffer(vaoID, vboID, layout);

        // Criar e configurar IBO
        IndexBufferID_Count iboID_Count = IndexBufferManager::CreateBuffer();
        IndexBufferManager::AddData(iboID_Count, &indices[0], static_cast<unsigned int>(indices.size()), GL_STATIC_DRAW);

        // Configurar shader
        Shader shader("res/shaders/3D-Graph.shader");

        Renderer renderer;


        float angle = 0.01f;
        float angleIncrement = 90.0f; // Quantidade a aumentar por segundo

        /*RocketEquations rocket;
        double volumeIncrement = 10;
        double time = 1e-3;
        std::unordered_map<double, double> finalVelocities;
        for (double startVolume = 300.0; startVolume <= 1800.0; startVolume += volumeIncrement)
        {
            double unitPressure = 13789520.0 / 200.0;
            double pressure = unitPressure * 200;
            double nRT = pressure * (2000.0 - startVolume); // pressão = nRT/V
            double velocity = 0.0;
            double volume = startVolume;
            while (volume > 0.0)
            {
                std::tuple<double, double, double> tuple = rocket.RocketVelocityEquation(pressure, volume, 225, time, nRT);
                velocity += std::get<0>(tuple);
                volume = std::get<1>(tuple);
                pressure = std::get<2>(tuple);
                if (pressure < 1013250.0)
                {
                    pressure = 1013250.0;
                }
               // std::cout << volume << std::endl;
            }
            finalVelocities[startVolume] = velocity;
        }
        
        double maxVelocity = std::numeric_limits<double>::lowest(); // Valor inicial para a maior velocidade
        double bestVolume = 0.0;
        // Iterar sobre o unordered_map para encontrar a maior velocidade
        for (const auto& pair : finalVelocities)
        {
            if (pair.second > maxVelocity)
            {
                maxVelocity = pair.second;
                bestVolume = pair.first;
            }
        }

        std::cout << "Usando volume de " << bestVolume << "cm3 temos a maior velocidade, que e: " << maxVelocity << std::endl;
        std::cout << "houve " << rocket.erros << " erros!";*/

        float fov = 45.0f; // Campo de visão em graus
       
   
        float nearPlane = 0.1f; // Plano próximo
        float farPlane = 100.0f; // Plano distante

        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }

            float aspectRatio = (float)window_width / (float)window_height; // Razão de aspecto da janela
            glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
            shader.SetUniformMat4f("u_Projection", projection);
            
            angle += angleIncrement * (float)glfwGetTime(); // Incrementa o ângulo com base no tempo decorrido
            glfwSetTime(0.0f); // Reseta o tempo para a próxima iteração
            glm::mat4 model = glm::mat4(1.0f); // Matriz identidade
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f)); // Transladar o objeto
            model = glm::rotate(model, glm::radians(angle), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))); // Rotacionar o objeto
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Escalar o objeto
            shader.SetUniformMat4f("u_Model", model);

            glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //shader.Bind();

            //VAO.Bind();
            //IBO.Bind();

            renderer.Draw(vaoID, iboID_Count, shader);

           // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    glfwTerminate();
    return 0;
}