#include<iostream>
#include<stb/stb_image.h>
#include<glad/glad.h>
#include<GL/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Texture.h"
#include "Camera.h"
#include "Shaderclass.h"
#include "VAO.h"
#include "VBO.h"
#include<Filesystem.h>
const unsigned int width =1920;
const unsigned  int height=1080;


void ProcessInput(GLFWwindow *);
void mousecb(GLFWwindow *,double xpos,double ypos);
void scrollcb(GLFWwindow *,double xoffset,double yoffset);
unsigned int loadTexture(const char *path);

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));

bool firstMouse=true;
float lastX = width / 2.0f;
float lastY = height / 2.0f;
float deltaTime=0.0f;  //time between last frame and current frame
float lastFrame=0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLfloat vertices[]= {
	 // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,    -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,    -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f,    -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f,    -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,    -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,    -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,     1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,     1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,     1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,     1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,     1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,     1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,     0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,     0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,     0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,     0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,     0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,     0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,     0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,     0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,     0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,     0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,     0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,     0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,     0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,     0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,     0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,     0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,     0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,     0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,     0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,     0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,     0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,     0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,     0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,     0.0f,  0.0f, 1.0f
};
int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Light GL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mousecb);
    glfwSetScrollCallback(window, scrollcb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    
    Shader lightingShader("color.vert", "color.frag");
    Shader lightCubeShader("light_cube.vert", "light_cube.frag");

    VAO cubeVAO;
    VBO VBO1(vertices, sizeof(vertices));
    cubeVAO.Bind();
    cubeVAO.LinkAttrib(VBO1,0,3,GL_FLOAT,8*sizeof(float),(void*)0);   
	cubeVAO.LinkAttrib(VBO1,1,3,GL_FLOAT,8*sizeof(float),(void*)(3*sizeof(float)));
	cubeVAO.LinkAttrib(VBO1,2,2,GL_FLOAT,8*sizeof(float),(void*)(6*sizeof(float)));
	VAO lightcubeVAO;
	lightcubeVAO.Bind();
	VBO1.Bind();
	cubeVAO.LinkAttrib(VBO1,0,3,GL_FLOAT,8*sizeof(float),(void*)0);   
	
	unsigned int diffuseMap = loadTexture(FileSystem::getPath("Wcontainer.png").c_str());
	lightingShader.Activate(); 
    lightingShader.setInt("material.diffuse", 0);

	
	float rotation=0.0f;
   double prevTime=glfwGetTime();
	
	while(!glfwWindowShouldClose(window))
    {
    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        ProcessInput(window);

    
        	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       float lightX = 2.0f * sin(glfwGetTime());
       float lightY = 1.5f*sin(glfwGetTime());
       float lightZ = 2.0f * cos(glfwGetTime());
       glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);
       
       double crntTime=glfwGetTime();
		if(crntTime-prevTime>=1/60)
		{
			rotation +=0.2f;
			prevTime=crntTime;
		}
      
        lightingShader.Activate();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        glm::vec3 lightcolor;
        lightcolor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
        lightcolor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
        lightcolor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
        glm::vec3 diffusecolor = lightcolor   * glm::vec3(0.8f); 
        glm::vec3 ambientcolor = diffusecolor * glm::vec3(0.9f);
         lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
        //lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        //lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 128.0f);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width/ (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        cubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        
        
        lightCubeShader.Activate();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
       lightCubeShader.setMat4("model", model);
       
        lightcubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

  
    cubeVAO.Delete();
    lightcubeVAO.Delete();
    VBO1.Delete();

    glfwTerminate();
    return 0;
}
void ProcessInput(GLFWwindow *window)
{
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
void mousecb(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    
   camera.ProcessMouseMovement(xoffset, yoffset);

}
void scrollcb(GLFWwindow *window,double xoffset,double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load("Wcontainer.png", &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}