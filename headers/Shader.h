class Shader{
    private:
        unsigned int shader_id;
        const char *vtx_shader;
        const char *frag_shader;
    public:
        Shader(const char *,const char *);
        unsigned int getUniformLocation(const char *);
        static void putUniformM4(unsigned int, glm::mat4 &);
        static void putUniform3f(unsigned int, float , float , float );
        static void putUniform1i(unsigned int, int );
        void use();
};