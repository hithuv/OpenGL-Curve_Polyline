#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.h"
#include "shape/Line.h"
#include "shape/Mesh.h"
#include "shape/Sphere.h"
#include "shape/Tetrahedron.h"
#include "shape/Icosahedron.h"
#include "shape/Ellipsoid.h"
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        // Check and call events and swap the buffers
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}


void App::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    app.mousePos.x = xpos;
    app.mousePos.y = App::kWindowHeight - ypos;

    if (app.mousePressed)
    {
        // Note: Must calculate offset first, then update lastMouseLeftPressPos.
        glm::dvec2 offset = app.mousePos - app.lastMouseLeftPressPos;
        app.lastMouseLeftPressPos = app.mousePos;
        app.camera.processMouseMovement(offset.x, offset.y);
    }
}


void App::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void App::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if(key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3 || key == GLFW_KEY_4
        || key == GLFW_KEY_5 || key == GLFW_KEY_6 || key == GLFW_KEY_7 || key == GLFW_KEY_8 || key == GLFW_KEY_9 || key == GLFW_KEY_0){
        app.userMode = key - GLFW_KEY_0;
        return;
    }

    if(key == GLFW_KEY_X && action == GLFW_PRESS){
        app.renderAxes = !app.renderAxes;
        return;
    }

    if(app.userMode == 1){
        if (action == GLFW_PRESS){
            if(key == GLFW_KEY_F1){
                app.currentRenderMode = RenderMode::Wireframe;
            }
            else if(key == GLFW_KEY_F2){
                app.currentRenderMode = RenderMode::Flat;
            }
            else if(key == GLFW_KEY_F4){
                app.currentRenderMode = RenderMode::Smooth;
            }
        }
    }
    else if(app.userMode == 2){
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            app.icosahedron->subdivide();
        }
    }
    else if(app.userMode == 3){
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            app.ellipsoid->subdivide();
        }
    }

    
}


void App::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            app.mousePressed = true;
            app.lastMouseLeftClickPos = app.mousePos;
            app.lastMouseLeftPressPos = app.mousePos;
        }
        else if (action == GLFW_RELEASE)
        {
            app.mousePressed = false;
        }
    }
}


void App::scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));
    app.camera.processMouseScroll(yoffset);
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
    // Camera control
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kLeft, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kRight, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kBackWard, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kForward, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kUp, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kDown, app.timeElapsedSinceLastFrame);
    }
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
    glLineWidth(2.0f);
    glPointSize(1.0f);
    glEnable(GL_DEPTH_TEST);

    initializeShadersAndObjects();
}


void App::initializeShadersAndObjects()
{
    pLineShader = std::make_unique<Shader>("src/shader/line.vert.glsl",
                                           "src/shader/line.frag.glsl");

    pMeshShader = std::make_unique<Shader>("src/shader/mesh.vert.glsl",
                                           "src/shader/phong.frag.glsl");

    pSphereShader = std::make_unique<Shader>("src/shader/sphere.vert.glsl",
                                             "src/shader/sphere.tesc.glsl",
                                             "src/shader/sphere.tese.glsl",
                                             "src/shader/phong.frag.glsl");

    shapes.emplace_back(
            std::make_unique<Line>(
                    pLineShader.get(),
                    std::vector<Line::Vertex> {
                            {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                            {{3.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                            {{0.0f, 3.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                            {{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 1.0f}},
                    },
                    glm::mat4(1.0f)
            )
    );

    shapes.emplace_back(
            std::make_unique<Tetrahedron>(
                    pMeshShader.get(),
                    "var/tetrahedron.txt",
                    glm::translate(glm::mat4(1.0f), {-2.0f, 0.0f, 0.0f})
            )
    );

    shapes.emplace_back(
            std::make_unique<Mesh>(
                    pMeshShader.get(),
                    std::vector<Mesh::Vertex> {
                            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
                            {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
                            {{0.0f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                    },
                    glm::rotate(
                            glm::translate(glm::mat4(1.0f), {2.0f, 0.0f, 0.0f}),
                            glm::radians(45.0f), {0.0f, 1.0f, 0.0f}
                    )
            )
    );

    shapes.emplace_back(
            std::make_unique<Sphere>(
                    pSphereShader.get(),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    1.0f,
                    glm::vec3(1.0f, 0.5f, 0.31f),
                    glm::mat4(1.0f)
            )
    );

    axes = std::make_unique<Line>(
            pLineShader.get(),
            std::vector<Line::Vertex> {
                    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                    {{10.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                    {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                    {{0.0f, 10.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                    {{0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 1.0f}},
            },
            glm::mat4(1.0f)
    );

    tetrahedron = std::make_unique<Tetrahedron>(
            pMeshShader.get(),
            "var/tetrahedron.txt",
            glm::translate(glm::mat4(1.0f), {2.0f, 0.0f, 0.0f})
    );

    cube = std::make_unique<Tetrahedron>(
            pMeshShader.get(),
            "var/cube.txt",
            glm::translate(glm::mat4(1.0f), {4.0f, 0.0f, 0.0f})
    );

    octahedron = std::make_unique<Tetrahedron>(
            pMeshShader.get(),
            "var/octahedron.txt",
            glm::translate(glm::mat4(1.0f), {3.0f, 2.0f, 0.0f})
    );

    icosahedron = std::make_unique<Icosahedron>(
            pMeshShader.get(),
            "var/icosahedron.txt",
            glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 0.0f})
    );

    // Example scaling factors for the ellipsoid
    // float scaleX = 2.0f; // Scale along the x-axis
    // float scaleY = 1.0f; // Scale along the y-axis
    // float scaleZ = 0.5f; // Scale along the z-axis

    // Define the model matrix with scaling for the ellipsoid
    glm::mat4 ellipsoidModel = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 0.0f));
    ellipsoidModel = glm::scale(ellipsoidModel, glm::vec3(2.0f, 1.0f, 0.5f)); // Example scaling factors

    // Instantiate Ellipsoid with corrected arguments
    ellipsoid = std::make_unique<Ellipsoid>(
        pMeshShader.get(),                             // Shader*
        std::string("var/icosahedron.txt"),       // std::string
        ellipsoidModel                           // glm::mat4
    );





}


void App::render()
{
    auto t = static_cast<float>(timeElapsedSinceLastFrame);

    // Update shader uniforms.
    view = camera.getViewMatrix();
    projection = glm::perspective(glm::radians(camera.zoom),
                                  static_cast<GLfloat>(kWindowWidth) / static_cast<GLfloat>(kWindowHeight),
                                  0.01f,
                                  100.0f);

    pLineShader->use();
    pLineShader->setMat4("view", view);
    pLineShader->setMat4("projection", projection);

    pMeshShader->use();
    pMeshShader->setMat4("view", view);
    pMeshShader->setMat4("projection", projection);
    pMeshShader->setVec3("ViewPos", camera.position);
    pMeshShader->setVec3("lightPos", lightPos);
    pMeshShader->setVec3("lightColor", lightColor);

    pSphereShader->use();
    pSphereShader->setMat4("view", view);
    pSphereShader->setMat4("projection", projection);
    pSphereShader->setVec3("ViewPos", camera.position);
    pSphereShader->setVec3("lightPos", lightPos);
    pSphereShader->setVec3("lightColor", lightColor);

    // Render.
    // for (auto & s : shapes)
    // {
    //     s->render(t);
    // }

    if(renderAxes)
    {
        axes->render(t);
    }
    if(currentRenderMode == RenderMode::Wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(currentRenderMode == RenderMode::Flat){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if(currentRenderMode == RenderMode::Smooth){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // if (currentRenderMode == RenderMode::Flat) {
    //     pMeshShader->setInt("displayMode", 1); // Flat
    // } else if (currentRenderMode == RenderMode::Smooth) {
    //     pMeshShader->setInt("displayMode", 2); // Flat
    // }

    if(userMode == 1){
        tetrahedron->render(t);
        cube->render(t);
        octahedron->render(t);
    }
    else if(userMode == 2){
        icosahedron->render(t);
    }
    else if(userMode == 3){
        ellipsoid->render(t);
    }
}
