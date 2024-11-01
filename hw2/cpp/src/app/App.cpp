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



void App::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));
    app.mousePos.x = xpos;
    app.mousePos.y = App::kWindowHeight - ypos;
    app.currentMousePos = app.mousePos;

    if (app.selectedPointIndex != -1 && app.selectedSegmentIndex != -1) {
        app.dragControlPoint();
    }

}


void App::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void App::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));


    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        app.curveFinalized = false;
        app.inBezierMode = true;
        std::cout << "inBezierMode = true" << std::endl;
        app.splineSegments.clear();
    } else if (key == GLFW_KEY_INSERT && action == GLFW_PRESS) {
        app.insertControlPoint();
    } else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        app.deleteControlPoint();
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
        app.saveSplineToFile("config.txt");
    } else if (key == GLFW_KEY_L && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
        app.loadSplineFromFile("config.txt");
    }

}

// void App::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
//     App& app = *reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
//     if (app.inBezierMode) {
//         if (app.curveFinalized == false && action == GLFW_PRESS) {
//             if (button == GLFW_MOUSE_BUTTON_LEFT) {
//                 if (app.splineSegments.empty() || app.splineSegments.back().size() == 4) {
//                     app.addControlPoint(app.mousePos);
//                 } else if (app.splineSegments.back().size() == 3) {
//                     app.addControlPoint(app.mousePos);
//                     app.curveFinalized = true;
//                 }
//             }
//         } else if (app.curveFinalized && button == GLFW_MOUSE_BUTTON_LEFT) {
//             if (action == GLFW_PRESS) {
//                 app.selectControlPoint();
//             } else if (action == GLFW_RELEASE) {
//                 app.selectedPointIndex = -1;
//                 app.selectedSegmentIndex = -1;
//             }
//         }
//     }
// }


void App::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (app.inBezierMode) {
        if(app.curveFinalized==false && action == GLFW_PRESS){
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                app.addControlPoint(app.mousePos);
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT && app.splineSegments.size() > 0) {
                app.curveFinalized = true;
                app.addControlPoint(app.mousePos);
                
            }
        }
        else if(app.curveFinalized && button == GLFW_MOUSE_BUTTON_LEFT){
            if(action == GLFW_PRESS){
                app.selectControlPoint();
            }
            else if(action == GLFW_RELEASE){
                app.selectedPointIndex = -1;
                app.selectedSegmentIndex = -1;
            }
            
        }
    }
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

    //Sris
    pBezierShader = std::make_unique<Shader>("src/shader/bezier.vert.glsl",
                                             "src/shader/bezier.frag.glsl");

    //E-Sri
}


void App::render()
{
    auto t = static_cast<float>(timeElapsedSinceLastFrame);

    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));


    if (inBezierMode) {
        renderSpline();
        renderControlPoints();
    }

}

void App::renderSpline() {
    if (splineSegments.empty()) return;

    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));
    std::vector<glm::vec2> allCurvePoints;
    for (size_t i = 0; i < splineSegments.size(); ++i) {
        auto& segment = splineSegments[i];
        std::vector<glm::vec2> currentSegment = segment;

        if (segment.size() < 4) {
            if (i == splineSegments.size() - 1) {
                // For the last incomplete segment, use the mouse position as the last point
                while (currentSegment.size() < 4) {
                    currentSegment.push_back(mousePos);
                }
            } else {
                // Skip incomplete segments that are not the last one
                continue;
            }
        }

        for (float t = 0; t <= 1; t += 0.01f) {
            allCurvePoints.push_back(evaluateBezier(currentSegment, t));
        }
    }

    // std::vector<glm::vec2> allCurvePoints;
    // for (auto& segment : splineSegments) {
    //     if(segment.size()<3)continue;
    //     bool pop_last = false;
    //     if(segment.size() == 3){
    //         segment.push_back(mousePos);
    //         pop_last = true;
    //     }
    //     for (float t = 0; t <= 1; t += 0.01f) {
    //         allCurvePoints.push_back(evaluateBezier(segment, t));
    //     }
    //     if(pop_last)segment.pop_back();
    // }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allCurvePoints.size() * sizeof(glm::vec2), allCurvePoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    pBezierShader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f)); // Blue color for the curve
    glDrawArrays(GL_LINE_STRIP, 0, allCurvePoints.size());

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void App::renderControlPoints() {
    if (splineSegments.empty()) return;

    std::vector<glm::vec2> allControlPoints;
    for (const auto& segment : splineSegments) {
        allControlPoints.insert(allControlPoints.end(), segment.begin(), segment.end());
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allControlPoints.size() * sizeof(glm::vec2), allControlPoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    pBezierShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f)); // Red color for control points
    glPointSize(9.0f);
    glDrawArrays(GL_POINTS, 0, allControlPoints.size());
    glPointSize(1.0f);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void App::addControlPoint(const glm::vec2& point) {

    if(splineSegments.size() == 0){
        splineSegments.push_back({});
        splineSegments.back().push_back(point);
    }
    else if(splineSegments.size() == 1 && splineSegments.back().size() <3){
        splineSegments.back().push_back(point);
    }
    else if(splineSegments.back().size() == 3){
        splineSegments.back().push_back(point);
        if(curveFinalized)return;
        // splineSegments.push_back({});
        auto& prevSegment = splineSegments.back();
        glm::vec2 P0 = prevSegment[0], P1 = prevSegment[1], P2 = prevSegment[2], P3 = prevSegment[3];
        glm::vec2 P4 = P3 + (P3 - P2);
        glm::vec2 P5 = 2.0f * P4 - P3 + (P2 - P1);
        splineSegments.push_back({P3, P4, P5});
        std::cout<<"point are: p0 = "<<P0.x<<", "<<P0.y<<"; p1 = "<<P1.x<<", "<<P1.y<<";  p2 = "<<P2.x <<", "<<P2.y<<std::endl;
        std::cout<<"point are: p3 = "<<P3.x<<", "<<P3.y<<"; p4 = "<<P4.x<<", "<<P4.y<<";  p5 = "<<P5.x <<", "<<P5.y<<std::endl;
        // splineSegments.push_back({p0, p1, p2});
    }

}


void App::ensureC2Continuity() {
    if (splineSegments.size() < 2) return;
    
    auto& prevSegment = splineSegments[splineSegments.size() - 2];
    auto& currSegment = splineSegments.back();
    
    glm::vec2 v1 = prevSegment[3] - prevSegment[2];
    glm::vec2 v2 = currSegment[1] - currSegment[0];
    
    currSegment[1] = currSegment[0] + 2.0f * v1 - v2;
}

glm::vec2 App::evaluateBezier(const std::vector<glm::vec2>& controlPoints, float t) {
    float u = 1 - t;
    return u*u*u * controlPoints[0] + 
           3*u*u*t * controlPoints[1] + 
           3*u*t*t * controlPoints[2] + 
           t*t*t * controlPoints[3];
}

void App::selectControlPoint() {
    for (size_t i = 0; i < splineSegments.size(); ++i) {
        for (size_t j = 0; j < splineSegments[i].size(); ++j) {
            if (glm::distance(splineSegments[i][j], currentMousePos) < 10.0f) {
                selectedSegmentIndex = i;
                selectedPointIndex = j;
                return;
            }
        }
    }
}

void App::dragControlPoint() {
    splineSegments[selectedSegmentIndex][selectedPointIndex] = currentMousePos;
    // ensureC2Continuity();
}

void App::insertControlPoint() {
    if (selectedPointIndex != -1 && selectedSegmentIndex != -1) {
        glm::vec2 newPoint = (splineSegments[selectedSegmentIndex][selectedPointIndex] + 
                              splineSegments[selectedSegmentIndex][(selectedPointIndex + 1) % 4]) * 0.5f;
        splineSegments[selectedSegmentIndex].insert(splineSegments[selectedSegmentIndex].begin() + selectedPointIndex + 1, newPoint);
        // ensureC2Continuity();
    }
}

void App::deleteControlPoint() {
    if (selectedPointIndex != -1 && selectedSegmentIndex != -1) {
        splineSegments[selectedSegmentIndex].erase(splineSegments[selectedSegmentIndex].begin() + selectedPointIndex);
        // ensureC2Continuity();
    }
}

void App::saveSplineToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "2 2 " << getTotalControlPoints() << std::endl;
        for (const auto& segment : splineSegments) {
            for (const auto& point : segment) {
                file << point.x << " " << point.y << std::endl;
            }
        }
        file.close();
    }
}

void App::loadSplineFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int dim, continuity, totalPoints;
        file >> dim >> continuity >> totalPoints;
        
        splineSegments.clear();
        for (int i = 0; i < totalPoints; ++i) {
            float x, y;
            file >> x >> y;
            addControlPoint(glm::vec2(x, y));
        }
        file.close();
    }
}

int App::getTotalControlPoints() const {
    int total = 0;
    for (const auto& segment : splineSegments) {
        total += segment.size();
    }
    return total;
}

std::vector<glm::vec2> App::calculateNewSegmentPoints(const std::vector<glm::vec2>& prevSegment, const glm::vec2& newPoint) {
    glm::vec2 p0 = prevSegment[3];
    glm::vec2 p1 = 2.0f * prevSegment[3] - prevSegment[2];
    glm::vec2 p2 = (2.0f * p1 - p0 + newPoint) / 2.0f;
    return {p0, p1, p2};
}

