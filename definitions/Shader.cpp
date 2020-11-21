#include "../headers/common.h"

Shader::Shader(const char * vtx_shader, const char * frag_shader){
    this->shader_id = loadShaders(vtx_shader, frag_shader);
    this->vtx_shader = vtx_shader;
    this->frag_shader = frag_shader;
}
Shader::Shader(void){
    shader_id = 0;
    vtx_shader =  NULL;
    frag_shader = NULL;
}

void Shader::use(){
    glUseProgram(shader_id);
}
unsigned int Shader::getUniformLocation(const char *name){
    return glGetUniformLocation(shader_id, name);
}
void Shader::putUniformM4(unsigned int loc, glm::mat4 &mat){
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}
void Shader::putUniform3f(unsigned int loc, float x, float y, float z){
    glUniform3f(loc, x, y, z);   
}
void Shader::putUniform1i(unsigned int loc, int v){
    glUniform1i(loc, v);
}