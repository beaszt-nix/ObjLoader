#include "headers/common.h"
#include <cstdlib>
#include <iostream>

bool showMenu, rotate;
TextBox *input;
Object *selected;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

std::vector<std::string> faces = {
    "models/skybox/Skybox/posx.jpg",
    "models/skybox/Skybox/negx.jpg",
    "models/skybox/Skybox/negy.jpg",
    "models/skybox/Skybox/posy.jpg",
    "models/skybox/Skybox/posz.jpg",
    "models/skybox/Skybox/negz.jpg"
};

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    showMenu = true;
    glfwSetCursorPosCallback(window, NULL);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
    showMenu = false;
    glfwSetKeyCallback(window, NULL);
    glfwSetCharCallback(window, NULL);
  }
  if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
    if (rotate)
      glfwSetKeyCallback(window, NULL);
    rotate ^= true;
  }
}

void text_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    input->backspace_text();
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    input->move_cursor(1);
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    input->move_cursor(-1);
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    input->change_size(4);
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    input->change_size(-4);
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    input->space();    
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

void text_char_callback(GLFWwindow *window, unsigned int code){
  if (isprint(code))
    input->put_char((unsigned char)code);
}

int main(int argc, char *argv[])
{
  showMenu = false;
  rotate = false;
  std ::string input = "The following commands can be used.\n:l model_location texture_location\n:a new_camera_name\n:e for exit\nAAAAAAA";
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Window", NULL, NULL);

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

  TextBox text("models/font.dds", 12);

  add_camera(glm::vec3(0, 0, 5), 70.0f, window, "main");
  select_camera("main");

  Camera *current = get_current_camera();
  selected = &object;
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render red background
    glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    //Render Object
    current->render_view(object);
    object.render_object();
    
    current->render_sky(skybox);
    skybox.render_sky();

    // Rotate Object
    if (rotate){
      glfwSetKeyCallback(window, object_key_callback);
    }

    // Input Box
    if (showMenu){
      glfwSetKeyCallback(window, text_key_callback);
      text.print_multi_string(20, 550, input);
    }

    // Buffer management
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}