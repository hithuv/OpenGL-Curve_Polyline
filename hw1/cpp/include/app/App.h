#ifndef APP_H
#define APP_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "app/Window.h"
#include "shape/Pixel.h"


class Shader;
class Renderable;

enum Mode {
  LINE_MODE = 1,
  POLYLINE_MODE = 3,
  ELLIPSE_MODE = 4,
  CIRCLE_MODE = 8
};




class App : private Window
{
public:
    static App & getInstance();

    void run();

private:
    // GLFW callbacks.
    static void cursorPosCallback(GLFWwindow *, double, double);
    static void framebufferSizeCallback(GLFWwindow *, int, int);
    static void keyCallback(GLFWwindow *, int, int, int, int);
    static void mouseButtonCallback(GLFWwindow *, int, int, int);
    static void scrollCallback(GLFWwindow *, double, double);

    static void perFrameTimeLogic(GLFWwindow *);
    static void processKeyInput(GLFWwindow *);

    // from CMakeLists.txt, compile definition
    static constexpr char kWindowName[] {WINDOW_NAME};
    static constexpr int kWindowWidth {1000};
    static constexpr int kWindowHeight {1000};

private:
    /// Bresenham line-drawing algorithm for line (x0, y0) -> (x1, y1) in screen space,
    /// given that its slope m satisfies 0.0 <= m <= 1.0 and that (x0, y0) is the start position.
    /// All pixels on this line are appended to path.
    static void bresenhamLine(std::vector<Pixel::Vertex> & path, int x0, int y0, int x1, int y1);

    App();

    void render();

    // Shaders.
    // In principle, a shader could be reused across multiple objects.
    // Thus, these shaders are not designed as members of object classes.
    std::unique_ptr<Shader> pPixelShader {nullptr};

    // Objects to render.
    std::vector<std::unique_ptr<Renderable>> shapes;

    // Object attributes affected by GUI.
    bool animationEnabled {true};
    bool showPreview {false};

    // Frontend GUI
    double timeElapsedSinceLastFrame {0.0};
    double lastFrameTimeStamp {0.0};

    bool mousePressed {false};
    glm::dvec2 mousePos {0.0, 0.0};

    // Note lastMouseLeftClickPos is different from lastMouseLeftPressPos.
    // If you press left button (and hold it there) and move the mouse,
    // lastMouseLeftPressPos gets updated to the current mouse position
    // (while lastMouseLeftClickPos, if there is one, remains the original value).
    glm::dvec2 lastMouseLeftClickPos {0.0, 0.0};
    glm::dvec2 lastMouseLeftPressPos {0.0, 0.0};

    //defined by Srihith
    //test if C is pressed for polyline completion to polygon
    bool cPressed = false;
    //to save corners of polyline:
    std::vector<std::vector<glm::dvec2>> polylineCorners;
    //to track mode
    Mode currentMode = LINE_MODE;
    //draw Ellipse
    static void drawEllipse(std::vector<Pixel::Vertex> & path, int x0, int y0, int x1, int y1);
    //to track if Shift is pressed in ellipse Mode
    bool shiftPressed = false;


};


#endif  // APP_H
