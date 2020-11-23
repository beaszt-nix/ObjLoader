#include "../headers/common.h"
#include <sstream>

TextBox::TextBox(const char *texture, int size)
:size(size){
    this->shader = Shader("shaders/text_vtx_shader.glsl", "shaders/text_frag_shader.glsl");
    shader.use();
    cursor = -1;
    texture_id = Object::loadDDS(texture);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_buffer_id);
    glGenBuffers(1, &uv_buffer_id);
    uniform_id = shader.getUniformLocation("myTextureSampler");
}

void TextBox::display_content(int x, int y){
    unsigned int length = text.length();
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> uvs;
    for (int i = 0; i < length; i++){
        glm::vec2 vtx_tl = glm::vec2( x + i * size       , y + size);
        glm::vec2 vtx_tr = glm::vec2( x + i * size + size, y + size);
        glm::vec2 vtx_dr = glm::vec2( x + i * size + size, y );
        glm::vec2 vtx_dl = glm::vec2( x + i * size       , y );

        vertices.push_back(vtx_tl);
        vertices.push_back(vtx_dl);
        vertices.push_back(vtx_tr);

        vertices.push_back(vtx_dr);
        vertices.push_back(vtx_tr);
        vertices.push_back(vtx_dl);

        char letter = text[i];
        float uv_x, uv_y, xoffset= 1.0f/20.0f, yoffset= 1.0f/16.0f;
        uv_x = (letter % 16) / 16.0f;
        uv_y = (letter / 16) / 16.0f;

        glm::vec2 uv_tl = glm::vec2(uv_x, uv_y);
        glm::vec2 uv_tr = glm::vec2(uv_x + xoffset, uv_y);
        glm::vec2 uv_dr = glm::vec2(uv_x + xoffset, uv_y + yoffset);
        glm::vec2 uv_dl = glm::vec2(uv_x, uv_y + yoffset);

        uvs.push_back(uv_tl);
        uvs.push_back(uv_dl);
        uvs.push_back(uv_tr);

        uvs.push_back(uv_dr);
        uvs.push_back(uv_tr);
        uvs.push_back(uv_dl);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glUniform1i(uniform_id, 0);

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glDisable(GL_BLEND);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
    }
}

TextBox::~TextBox(){
    glDeleteBuffers(1, &vertex_buffer_id);
    glDeleteBuffers(1, &uv_buffer_id);
    glDeleteTextures(1, &texture_id);
}

void TextBox::put_text(std::string &s){
    text = s;
}

void TextBox::put_char(char c){
    if (cursor == -1)
        cursor = 0;
    text.insert(cursor, 1, c);
    cursor++;
}

void TextBox::space(){
    text.insert(cursor, " ");
}

void TextBox::backspace_text(){
    if (cursor == -1)
        return;
    text.erase(text.begin() + cursor);
    cursor--;
}

void TextBox::move_cursor(int dir){
    if (cursor == -1 && dir == -1)
        return;
    else if (cursor == text.length()-1 && dir == 1)
        return;
    cursor += dir;
}

void TextBox::change_size(int diff){
    if (5 <= size && size <= 60)
        size += diff;
}

std::string TextBox::get_text(){
    return text;
}

void TextBox::print_multi_string(int x, int y, std::string &input){
    std::stringstream ss(input);
    std::string to;
    if (!input.empty()){
        int i = 0;
        while (std::getline(ss, to, '\n')){
            this->text = to;
            this->display_content(x, y - i++ * size);
        }
    }
}