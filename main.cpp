#include "headers/common.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0,0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(int argc, char *argv[]){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800,600, "Window", NULL , NULL);

    if (window == NULL) {
        std :: cout << "Failed to create window"  << std :: endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "GLAD COULDN'T BE INITIALIZED"  << std :: endl;
      return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    GLuint shader_id = loadShaders("shaders/vtx_shader.glsl", "shaders/fragment_shader.glsl");
    GLuint light_id = glGetUniformLocation(shader_id, "LightPosition_worldspace");
    Object object((const char *)argv[1], (const char *)argv[2], shader_id);
    
    add_camera(glm::vec3(2, 2, 2), 45.0f, window, "main");
    select_camera("main");
    Camera *current = get_current_camera();
    if (current != NULL)
      printf("Camera Fetched\n");
    else{
      perror("Camera couldn't load");
      exit(-1);
    }
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        // Render red background       
        glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render Object
        current->render_view(object, light_id);
        object.render_object();

        // Buffer management
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}