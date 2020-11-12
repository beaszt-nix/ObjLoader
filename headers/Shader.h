class Shader{
    private:
        unsigned int shader_id;
        const char *vtx_shader;
        const char *frag_shader;
    public:
        Shader(char *, char *);
        unsigned int getUniformLocation(char *);
        void putUniformM4(unsigned int, glm::mat4 &);
        void putUniform3f(unsigned int, float, float, float);
        void putUniform1i(unsigned int, int);
        void use();
};