class TextBox{
    private:
        std::string text;
        GLuint texture_id;
        GLuint vertex_buffer_id;
        GLuint uv_buffer_id;
        GLuint uniform_id;
        Shader &shader;
        int cursor;
        int size;
    public:
        TextBox(const char *, Shader &, int=32);
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