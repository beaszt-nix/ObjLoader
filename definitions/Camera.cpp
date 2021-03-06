#include "../headers/common.h"
#include <functional>
#include <utility>
#include <iostream>


Camera::Camera(const Camera &ting)
: initial_fov(ting.initial_fov)
, default_position(ting.default_position){
  this->horizontalAngle = ting.horizontalAngle;
  this->verticalAngle = ting.verticalAngle;
  this->field_of_view = ting.field_of_view;
  this->position = ting.position;
  this->direction = ting.direction;
  this->right = ting.right;
  this->up = ting.up;
  this->window = ting.window;
}

Camera::Camera(
          const float fov
        , const glm::vec3 init_pos
        , GLFWwindow *window): 
          initial_fov(fov)
       ,  default_position(init_pos)
       ,  window(window)
{
  this->lookSpeed = LOOK_SPEED;
  this->moveSpeed = MOVE_SPEED;
  this->deltaTime = 1 / FPS;
  reset();
}

void Camera::reset() {
  horizontalAngle = 3.14f;
  verticalAngle = 0.0f;
  field_of_view = initial_fov;
  position = default_position;
  get_vectors();
}

void Camera::get_vectors(){
  direction = glm::vec3(
          cos(verticalAngle)  * sin(horizontalAngle),
          sin(verticalAngle),
          cos(verticalAngle) * cos(horizontalAngle)
        );

  right = glm::vec3(
      sin(horizontalAngle - 3.14f/2.0f)
     ,0
     ,cos(horizontalAngle  - 3.14f/2.0f)
      );
  
  up = glm::cross(right, direction);
}

void camera_mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  int width, height;
  Camera *current = get_current_camera();
  glfwGetWindowSize(current->window, &width, &height);
  float mid_width = (float)width/2.0f, mid_height = (float)height/2.0f;
  current->horizontalAngle += current->lookSpeed * current->deltaTime * float(mid_width - xpos);
  current->verticalAngle += current->lookSpeed * current->deltaTime * float(mid_height- ypos);
  glfwSetCursorPos(current->window, mid_width, mid_height);
}

void camera_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera * current= get_current_camera();
    if (current->field_of_view >= 1.0f && current->field_of_view <= 90.0f)
        current->field_of_view -= (yoffset * 2.0f);
    if (current->field_of_view <= 1.0f)
        current->field_of_view = 1.0f;
    if (current->field_of_view >= 90.0f)
        current->field_of_view = 90.0f;
}

void camera_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
  Camera * current = get_current_camera();
  if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS) )
    current->position += current->direction * current->deltaTime * current->moveSpeed;
  if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS) )
    current->position -= current->direction * current->deltaTime * current->moveSpeed;
  if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS) )
    current->position += current->right * current->deltaTime * current->moveSpeed;
  if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS) )
    current->position -= current->right * current->deltaTime * current->moveSpeed;
}

void Camera::render_view(Object & obj)
{
  Shader &val = obj.shader;
  val.use();
  glfwSetCursorPosCallback(window, camera_mouse_callback);
  glfwSetScrollCallback(window, camera_scroll_callback);  
  glfwSetKeyCallback(window, camera_key_callback);
  

  get_vectors();
  glm::mat4 projection = glm::perspective(glm::radians(float(field_of_view)), 16.0f/9.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(
        position,
        position+direction,
        up
      );

  struct model input = obj.get_ids();
  glm::vec3 lightPos = glm::vec3(0,5,0) + direction + position;

  
  //glm::mat4 nview = glm::mat4(glm::mat3(view));
  glm::mat4 result = projection * view * input.model;

  Shader::putUniformM4(input.model_matrix_id, input.model);
  Shader::putUniformM4(input.view_matrix_id, view);
  Shader::putUniform3f(input.light_id, lightPos.x, lightPos.y, lightPos.z);
  obj.set_mvp(result);
  return;
}


void Camera::render_sky(Skybox & obj)
{
  Shader &val = obj.shader;
  val.use();
  glfwSetCursorPosCallback(window, camera_mouse_callback);
  glfwSetScrollCallback(window, camera_scroll_callback);  
  glfwSetKeyCallback(window, camera_key_callback);
  

  get_vectors();
  glm::mat4 projection = glm::perspective(glm::radians(float(field_of_view)), 16.0f/9.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(
        position,
        position+direction,
        up
      );

  std::vector<GLuint> input = obj.get_ids();

  Shader::putUniformM4(input[0], view);
  Shader::putUniformM4(input[1], projection);

  return;
}
