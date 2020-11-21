#include "../glad/glad.h"
#include <vector>
#include <map>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/gtx/scalar_multiplication.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Skybox.h"
#include "Object.h"
#include "Camera.h"
#include "TextBox.h"


#define FPS 30.0f
#define LOOK_SPEED 0.05f
#define MOVE_SPEED 30.0f


GLuint loadShaders(const char *, const char *);

void add_camera(
    glm::vec3 position
  , const float fov
  , GLFWwindow *window
  , std::string name);
void select_camera(std::string name);
std::vector<std::string> list_cameras();

Camera * get_current_camera();

struct model {
  glm::mat4 model;
  GLuint  model_matrix_id
        , view_matrix_id
        , result_id
        , light_id;
};
