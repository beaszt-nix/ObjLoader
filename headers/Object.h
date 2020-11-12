class Object
{
private:
  GLuint texture_id,
      mvp_id,
      model_id,
      view_id,
      texture,
      VAO,
      VBO,
      UVBuffer,
      NormalBuffer;
  std ::vector<glm::vec3> vertices;
  std ::vector<glm::vec2> uvs;
  std ::vector<glm::vec3> normals;
  glm::mat4 model, mvp;
  Shader &shader;

  bool loadOBJ(const char *objpath);
  GLuint loadDDS(const char *imagepath);

public:
  Object(const char *, const char *, Shader &);
  struct model get_ids();
  void set_mvp(glm::mat4 result);
  void render_object();
};