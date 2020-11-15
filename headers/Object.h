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

protected:
  bool loadOBJ(const char *objpath);

public:
  Object(const char *, const char *, Shader &);
  Object(Shader &);
  struct model get_ids();
  void set_mvp(glm::mat4 result);
  static GLuint loadDDS(const char *imagepath);
  void render_object(void);
};