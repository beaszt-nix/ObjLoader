void camera_mouse_callback( GLFWwindow *, double, double);
void camera_key_callback( GLFWwindow *, int, int, int, int);
void camera_scroll_callback( GLFWwindow *, double, double);

class Camera {
  friend void camera_mouse_callback( GLFWwindow *, double, double);
  friend void camera_key_callback( GLFWwindow *, int, int, int, int);
  friend void camera_scroll_callback( GLFWwindow *, double, double);
  private:
    float horizontalAngle
        , verticalAngle
        , field_of_view
        , moveSpeed
        , lookSpeed 
        , deltaTime;
    glm::vec3 position
            , direction
            , right
            , up;
    GLFWwindow * window;
    const float initial_fov;
    const glm::vec3 default_position;
    void get_vectors();
  public:
    Camera(
          const float fov
        , const glm::vec3 init_pos
        , GLFWwindow *window);
    Camera(const Camera &);
    void reset();
    void render_view(Object &, GLuint);
};