class TextBox{
    private:
        std::string text;
        GLuint texture_id;
        GLuint vertex_buffer_id;
        GLuint VAO;
        GLuint uv_buffer_id;
        GLuint uniform_id;
        int cursor;
        int size;
    public:
        Shader shader;
        TextBox(const char *, int=32);
        void put_text(std::string &s);
        void put_char(char);
        void move_cursor(int);
        void space();
        void backspace_text();
        void display_content(int x, int y);
        void change_size(int);
        void print_multi_string(int x, int y, std::string &);
        std::string get_text();
        ~TextBox();
};