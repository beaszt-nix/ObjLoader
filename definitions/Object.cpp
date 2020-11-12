#include "../headers/common.h"

Object::Object(
          const char * objpath
        , const char * texture
        , Shader &shader) 
        : shader(shader)
{
  this->texture = loadDDS(texture);
  loadOBJ(objpath);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
        GL_ARRAY_BUFFER
      , this->vertices.size() * sizeof(glm::vec3)
      , &this->vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &UVBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
  glBufferData(
        GL_ARRAY_BUFFER
      , this->uvs.size() * sizeof(glm::vec2)
      , &this->uvs[0], GL_STATIC_DRAW);
 
  glGenBuffers(1, &NormalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
  glBufferData(
        GL_ARRAY_BUFFER
      , this->normals.size() * sizeof(glm::vec3)
      , &this->normals[0], GL_STATIC_DRAW);

  this->shader.use();
  this->texture_id = shader.getUniformLocation("myTextureSampler");
  this->mvp_id = shader.getUniformLocation("MVP");
  this->model_id = shader.getUniformLocation("M");
  this->view_id = shader.getUniformLocation("V");
  this->model = glm::mat4(1.0f);
}

void Object::render_object() {
    this->shader.use();
    Shader::putUniformM4(mvp_id, mvp);

    glBindTexture(GL_TEXTURE_2D, texture);
    Shader::putUniform1i(texture_id, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(
         0,
         3,
         GL_FLOAT,
         GL_FALSE,
         0,
         (void *) 0
        );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glVertexAttribPointer(
         1,
         2,
         GL_FLOAT,
         GL_FALSE,
         0,
         (void *) 0
        );
   
   glEnableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
   glVertexAttribPointer(
         2,
         3,
         GL_FLOAT,
         GL_FALSE,
         0,
         (void*)0
       );

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

struct model Object::get_ids(){
  struct model item;
  item.model = model;
  item.model_matrix_id = model_id;
  item.view_matrix_id = view_id;
  item.result_id = mvp_id;
  return item;
}

void Object::set_mvp(glm::mat4 result){
  this->mvp = result;
}