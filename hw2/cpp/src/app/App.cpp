#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.h"
#include "shape/Circle.h"
#include "shape/Triangle.h"
#include "util/Shader.h"

App & App::getInstance()
{
    static App instance;
    return instance;
}


void App::run()
{
    while (!glfwWindowShouldClose(pWindow))
    {
        // Per-frame logic
        perFrameTimeLogic(pWindow);
        processKeyInput(pWindow);

        // Send render commands to OpenGL server
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        // Check and call events and swap the buffers
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}

//GPT
// void App::run() {
//     while (!glfwWindowShouldClose(pWindow)) {
//         render();
//         glfwSwapBuffers(pWindow);
//         glfwPollEvents();
//     }
// }

void App::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));
    app.mousePos.x = xpos;
    app.mousePos.y = App::kWindowHeight - ypos;
    app.currentMousePos = app.mousePos;

    // if (app.mousePressed)
    // {
    //     // // Note: Must calculate offset first, then update lastMouseLeftPressPos.
    //     // glm::dvec2 offset = app.mousePos - app.lastMouseLeftPressPos;
    //     app.lastMouseLeftPressPos = app.mousePos;
    // }
}


void App::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void App::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        app.inBezierMode = true;
        std::cout<<"inbezierMode = "<<app.inBezierMode<<std::endl;
        app.curveFinalized = false;
        app.controlPoints.clear();
    }

    // if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    // {
    //     app.animationEnabled = !app.animationEnabled;
    // }
}


void App::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (app.inBezierMode && action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && app.controlPoints.size() < 3) {
            app.addControlPoint(app.mousePos);
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT && app.controlPoints.size() == 3) {
            app.addControlPoint(app.mousePos);
            app.curveFinalized = true;
        }
    }

    // if (button == GLFW_MOUSE_BUTTON_LEFT)
    // {
    //     if (action == GLFW_PRESS)
    //     {
    //         app.mousePressed = true;
    //         app.lastMouseLeftClickPos = app.mousePos;
    //         app.lastMouseLeftPressPos = app.mousePos;
    //     }
    //     else if (action == GLFW_RELEASE)
    //     {
    //         app.mousePressed = false;

    //         #ifdef DEBUG_MOUSE_POS
    //         std::cout << "[ " << app.mousePos.x << ' ' << app.mousePos.y << " ]\n";
    //         #endif
    //     }
    // }

    // if (app.inBezierMode && action == GLFW_PRESS) {
    //     if ((button == GLFW_MOUSE_BUTTON_LEFT && app.clickCount < 3) ||
    //         (button == GLFW_MOUSE_BUTTON_RIGHT && app.clickCount == 3)) {
    //         app.addControlPoint(glm::vec2(app.mousePos.x, app.mousePos.y));
    //         app.clickCount++;
    //     }
    // }    
}


void App::scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{

}


void App::perFrameTimeLogic(GLFWwindow * window)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    double currentFrame = glfwGetTime();
    app.timeElapsedSinceLastFrame = currentFrame - app.lastFrameTimeStamp;
    app.lastFrameTimeStamp = currentFrame;
}


void App::processKeyInput(GLFWwindow * window)
{

}


App::App() : Window(kWindowWidth, kWindowHeight, kWindowName, nullptr, nullptr)
{
    // GLFW boilerplate.
    glfwSetWindowUserPointer(pWindow, this);
    glfwSetCursorPosCallback(pWindow, cursorPosCallback);
    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);
    glfwSetKeyCallback(pWindow, keyCallback);
    glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);
    glfwSetScrollCallback(pWindow, scrollCallback);

    // Global OpenGL pipeline settings
    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
    glPointSize(1.0f);

    // Initialize shaders and objects-to-render;
    // pTriangleShader = std::make_unique<Shader>("src/shader/triangle.vert.glsl",
    //                                            "src/shader/triangle.frag.glsl");
    // pCircleShader = std::make_unique<Shader>("src/shader/circle.vert.glsl",
    //                                          "src/shader/circle.tesc.glsl",
    //                                          "src/shader/circle.tese.glsl",
    //                                          "src/shader/circle.frag.glsl");

    //Sris
    pBezierShader = std::make_unique<Shader>("src/shader/bezier.vert.glsl",
                                             "src/shader/bezier.frag.glsl");

    //E-Sri

    // shapes.emplace_back(
    //         std::make_unique<Triangle>(
    //                 pTriangleShader.get(),
    //                 std::vector<Triangle::Vertex> {
    //                         // Vertex coordinate (screen-space coordinate), Vertex color
    //                         {{200.0f, 326.8f}, {1.0f, 0.0f, 0.0f}},
    //                         {{800.0f, 326.8f}, {0.0f, 1.0f, 0.0f}},
    //                         {{500.0f, 846.4f}, {0.0f, 0.0f, 1.0f}},
    //                 }
    //         )
    // );

    // shapes.emplace_back(
    //         std::make_unique<Circle>(
    //                 pCircleShader.get(),
    //                 std::vector<glm::vec3> {
    //                         // Coordinate (x, y) of the center and the radius (screen-space)
    //                         {200.0f, 326.8f, 200.0f},
    //                         {800.0f, 326.8f, 300.0f},
    //                         {500.0f, 846.4f, 400.0f}
    //                 }
    //         )
    // );
}


void App::render()
{
    auto t = static_cast<float>(timeElapsedSinceLastFrame);

    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));


    if (inBezierMode) {
        if(controlPoints.size() >= 3){
            renderBezierCurve();
        }
        renderControlPoints();
    }

}

void App::renderControlPoints() {
    if (controlPoints.empty()) return;

    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec2), controlPoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    // Set color for control points (e.g., red)
    pBezierShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));

    // Set larger point size
    glPointSize(9.0f);

    glDrawArrays(GL_POINTS, 0, controlPoints.size());

    // Reset point size
    glPointSize(1.0f);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}




void App::renderBezierCurve() {
    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));
    pBezierShader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f)); // Blue 

    bool pop_last = false;

    if(controlPoints.size() == 3){
        controlPoints.push_back(currentMousePos);
        pop_last = true;
    }

    std::vector<glm::vec2> curvePoints;
    for (float t = 0; t <= 1; t += 0.01f) {
        curvePoints.push_back(evaluateBezier(t));
    }

    if(pop_last){
        controlPoints.pop_back();
    }



    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(glm::vec2), curvePoints.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



void App::addControlPoint(const glm::vec2& point) {
    if (controlPoints.size() < 4) {
        controlPoints.push_back(point);
        std::cout<<"pushed ( "<< point.x<<", "<<point.y<<")"<<std::endl;
        std::cout<<"controlPoints.size() = "<<controlPoints.size()<<std::endl;
    }
}

glm::vec2 App::evaluateBezier(float t) {
    float u = 1-t;
    return u*u*u * controlPoints[0] +
           3*u*u*t * controlPoints[1] +
           3*u*t*t * controlPoints[2] +
           t*t*t * controlPoints[3];
}