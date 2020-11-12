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
  Camera *current = get_current_camera();
  current->horizontalAngle += current->lookSpeed * current->deltaTime * float(400.0f - xpos);
  current->verticalAngle += current->lookSpeed * current->deltaTime * float(300.0f - ypos);
  glfwSetCursorPos(current->window, 800.0f/2.0f, 600.0f/2.0f);
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
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
    current->reset();
}

void Camera::render_view(Object & obj, GLuint light_id)
{
  glfwSetCursorPosCallback(window, camera_mouse_callback);
  glfwSetScrollCallback(window, camera_scroll_callback);  
  glfwSetKeyCallback(window, camera_key_callback);
  

  get_vectors();
  glm::mat4 projection = glm::perspective(glm::radians(float(field_of_view)), 4.0f/3.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(
        position,
        position+direction,
        up
      );
  glm::vec3 lightPos = glm::vec3(6,6,6);

  struct model input = obj.get_ids();
  glm::mat4 result = projection * view * input.model;

  glUniformMatrix4fv(input.model_matrix_id, 1, GL_FALSE, &input.model[0][0]);
  glUniformMatrix4fv(input.view_matrix_id, 1, GL_FALSE, &view[0][0]);
  glUniform3f(light_id, lightPos.x, lightPos.y, lightPos.z);
  obj.set_mvp(result);
  return;
}