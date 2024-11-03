#ifndef APP_H
#define APP_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "app/Window.h"

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>


class Shader;
class Renderable;


class App : private Window
{
public:
    static App & getInstance();

    void run();

private:
    static void cursorPosCallback(GLFWwindow *, double, double);
    static void framebufferSizeCallback(GLFWwindow *, int, int);
    static void keyCallback(GLFWwindow *, int, int, int, int);
    static void mouseButtonCallback(GLFWwindow *, int, int, int);
    static void scrollCallback(GLFWwindow *, double, double);

    static void perFrameTimeLogic(GLFWwindow *);
    static void processKeyInput(GLFWwindow *);

    // from CMakeLists.txt, compile definition
    static constexpr char kWindowName[] {WINDOW_NAME};
    static constexpr int kWindowWidth {800};
    static constexpr int kWindowHeight {800};

    App();

    void render();

    void renderSpline();

    //Sri
    bool curveFinalized {false};
    glm::vec2 currentMousePos;

    void renderBezierPreview();
    void renderControlPoints(); 
    // void renderFinalBezierCurve();
    
    //ddd
    std::unique_ptr<Shader> pBezierShader {nullptr};
    std::unique_ptr<Shader> pControlPointShader {nullptr};
    std::vector<glm::vec2> controlPoints;
    std::vector<std::vector<glm::vec2>> splineSegments;
    bool inBezierMode {false};
    bool inCatmullRomMode {false};
    // bool mousePressed = false;

    void renderBezierCurve();
    void addBezierControlPoint(const glm::vec2& point);
    void addCatmullRomControlPoint(const glm::vec2& point);
    void ensureC2Continuity();
    glm::vec2 evaluateBezier(const std::vector<glm::vec2> &controlPoints, float t);
    std::vector<glm::vec2> calculateNewSegmentPoints(const std::vector<glm::vec2>& prevSegment, const glm::vec2& newPoint);
    //E-Sri

    // Shaders.

    
    // In principle, a shader could be reused across multiple objects.
    // Thus, these shaders are not designed as members of object classes.
    std::unique_ptr<Shader> pTriangleShader {nullptr};
    std::unique_ptr<Shader> pCircleShader {nullptr};

    // Objects to render.
    std::vector<std::unique_ptr<Renderable>> shapes;

    // Object attributes affected by GUI.
    bool animationEnabled {true};

    // Frontend GUI
    double timeElapsedSinceLastFrame {0.0};
    double lastFrameTimeStamp {0.0};

    bool mousePressed {false};
    bool insertPressed {false};
    bool deletePressed {false};
    glm::dvec2 mousePos {0.0, 0.0};

    // Note lastMouseLeftClickPos is different from lastMouseLeftPressPos.
    // If you press left button (and hold it there) and move the mouse,
    // lastMouseLeftPressPos gets updated to the current mouse position
    // (while lastMouseLeftClickPos, if there is one, remains the original value).
    glm::dvec2 lastMouseLeftClickPos {0.0, 0.0};
    glm::dvec2 lastMouseLeftPressPos {0.0, 0.0};
    void selectControlPoint();
    void dragBezierControlPoint();
    void buildBezierFromCatmullRom();
    void dragCatmullControlPoint();
    void insertControlPoint();
    void deleteControlPoint();
    void saveSplineToFile(const std::string &filename);
    void loadSplineFromFile(const std::string &filename);
    int getTotalControlPoints() const;
    uint tessellationShaderProgram;
    int selectedPointIndex = -1;
    int selectedSegmentIndex = -1;
};

#endif  // APP_H
