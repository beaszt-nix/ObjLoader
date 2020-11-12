#include "../headers/common.h"
#include <fstream>
#include <sstream>
#include <ios>

GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
  GLuint vtx_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint vtx_fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
  
  std :: string vtx_shader_code, frag_shader_code;
  std :: ifstream vtx_shader_stream(vertex_file_path, std::ios::in);
  std :: ifstream frag_shader_stream(fragment_file_path, std::ios::in);
  
  if (vtx_shader_stream.is_open()){
    std :: stringstream sstr;
    sstr << vtx_shader_stream.rdbuf();  
    vtx_shader_code = sstr.str();
    vtx_shader_stream.close();
  }
  else {
    printf("Couldn't load %s\n", vertex_file_path);
  }

  if (frag_shader_stream.is_open()){
    std :: stringstream sstr;
    sstr << frag_shader_stream.rdbuf();  
    frag_shader_code = sstr.str();
    frag_shader_stream.close();
  }
  else {
    printf("Couldn't load %s\n", fragment_file_path);
  }
  
  GLint result = GL_FALSE;
  int info_len;

  printf("Compiling Shader : %s\n", vertex_file_path);
  char const * vtx_source_ptr = vtx_shader_code.c_str();
  glShaderSource(vtx_shader_id, 1, &vtx_source_ptr, NULL);
  glCompileShader(vtx_shader_id);
  
  glGetShaderiv(vtx_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vtx_shader_id, GL_INFO_LOG_LENGTH, &info_len);
  if (info_len > 0) {
    std :: vector<char> vtx_shader_err(info_len + 1);
    glGetShaderInfoLog(vtx_shader_id, info_len, NULL, &vtx_shader_err[0]);
    printf("%s\n", &vtx_shader_err[0]);
  }

  printf("Compiling Shader : %s\n", fragment_file_path);
  char const * frag_source_ptr = frag_shader_code.c_str();
  glShaderSource(vtx_fragment_id, 1, &frag_source_ptr, NULL);
  glCompileShader(vtx_fragment_id);
  
  glGetShaderiv(vtx_fragment_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vtx_fragment_id, GL_INFO_LOG_LENGTH, &info_len);
  if (info_len > 0) {
    std :: vector<char> vtx_shader_err(info_len + 1);
    glGetShaderInfoLog(vtx_shader_id, info_len, NULL, &vtx_shader_err[0]);
    printf("%s\n", &vtx_shader_err[0]);
  }

  printf("Linking Program \n");
  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vtx_shader_id);
  glAttachShader(program_id, vtx_fragment_id);
  glLinkProgram(program_id);

  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_len);
  if (info_len > 0){
    std :: vector <char> program_err(info_len + 1);
    glGetProgramInfoLog(program_id, info_len, NULL, &program_err[0]);
    printf("%s\n", &program_err[0]);
  }

  glDetachShader(program_id, vtx_shader_id);
  glDetachShader(program_id, vtx_fragment_id);

  glDeleteShader(vtx_shader_id);
  glDeleteShader(vtx_fragment_id);
  return program_id;
}
