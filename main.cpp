#include "headers/common.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

using std::endl;

bool showMenu, rotate;
TextBox *input;
Object *selected;
Camera *curcam;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

std::vector<std::string> faces = {
    "models/skybox/Yokohama/posx.jpg",
    "models/skybox/Yokohama/negx.jpg",
    "models/skybox/Yokohama/negy.jpg",
    "models/skybox/Yokohama/posy.jpg",
    "models/skybox/Yokohama/posz.jpg",
    "models/skybox/Yokohama/negz.jpg"
};

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    showMenu = true;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
    showMenu = false;
  }
  if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
    rotate = true;
  }
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
    if (rotate)
      glfwSetKeyCallback(window,NULL);
    rotate = false;
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
    curcam->reset();
    selected->reset_model();
  }
}

void object_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
    selected->rotateX(5);
  if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
    selected->rotateX(-5);
  if (key == GLFW_KEY_UP && mods == 0 && action == GLFW_REPEAT)
    selected->rotateY(5);
  if (key == GLFW_KEY_DOWN && mods == 0 && action == GLFW_REPEAT)
    selected->rotateY(-5);
  if (key == GLFW_KEY_UP && mods == GLFW_MOD_SUPER && action == GLFW_REPEAT)
    selected->rotateZ(5);
  if (key == GLFW_KEY_DOWN && mods == GLFW_MOD_SUPER && action == GLFW_REPEAT)
    selected->rotateZ(-5);
}

int main(int argc, char *argv[])
{
  showMenu = true;
  rotate = false;
  std::ostringstream ss;

  ss << "The following commands can be used" << endl;
  ss << "Use WASD keys to move object, \n    Mouse to Look around\n\n\n" << endl;
  ss << "Hit R_SHIFT to turn off Menu" << endl;
  ss << "Hit L_SHIFT to display this Menu\n\n\n" << endl;
  ss << "Hit BACKSPACE to enable Rotate Mode" << endl;
  ss << "Hit M to disable Rotate Mode" << endl;
  ss << "Hit ESC to quit program" << endl;
  ss << "Hit R to reset to starting scene\n\n\n" << endl;
  ss << "IN ROTATE MODE, " << endl;
  ss << "Up/Down Arrow to Rotate Forward/Backward" << endl;
  ss << "Left/Right Arrow to Rotate about Clockwise\nor Anti-Clockwise" << endl;
  ss << "Super+Up/Down Arrow to Rotate Left/Right" << endl;
  
  std::string input;
  input = ss.str();

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(1280, 720, "Window", NULL, NULL);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  if (window == NULL)
  {
    std ::cout << "Failed to create window" << std ::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "GLAD COULDN'T BE INITIALIZED" << std ::endl;
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH_CLAMP);

  Object object((const char *)argv[1], (const char *)argv[2], "shaders/vtx_shader.glsl", "shaders/fragment_shader.glsl");
  Skybox skybox(faces);
  TextBox text("models/font2.dds", 18);

  add_camera(glm::vec3(0, 0, 5), 70.0f, window, "main");
  select_camera("main");

  curcam = get_current_camera();
  selected = &object;
  
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    
    // Render red background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // Input Box
    if (showMenu){
      text.print_multi_string(20, 550, input);
    }

    else{
      curcam->render_view(object);
      object.render_object();

      curcam->render_sky(skybox);
      skybox.render_sky();
    }

    // Rotate Object
    if (rotate){
      glfwSetKeyCallback(window, object_key_callback);
    }
    // Buffer management
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
