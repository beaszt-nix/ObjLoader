class Skybox {
private:
    std::vector<float> vertices;
    GLuint VAO, VBO, texture, view_id, proj_id, texture_id;
    std::vector<std::string> faces;
    unsigned char * loadBMP(const char * imagepath, unsigned char *data, int * wd, int *ht);
    unsigned int loadCubemap(void);
public:
    Shader shader;
    Skybox(const std::vector<std::string> &);
    void render_sky();
    std::vector<GLuint> get_ids(); 
};