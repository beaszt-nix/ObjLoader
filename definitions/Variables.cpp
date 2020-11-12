#include "../headers/common.h"
#include <iostream>
#include <vector>

std::map<std::string,Camera *> cameras;
Camera *current;

void add_camera(
    glm::vec3 position
  , const float fov
  , GLFWwindow *window
  , std::string name)
{
  cameras[name] = new Camera(fov, position, window);
}

void select_camera(std::string name){
  current = cameras[name];
}

std::vector<std::string> list_cameras(){
  std::vector<std::string> nlist;
  for (auto const &element : cameras){
    std::cout << "\t" << element.first  << std::endl;
    nlist.push_back(element.first);
  }
  return nlist;
}

Camera * get_current_camera(){
  return current;
}