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

    if (app.inBezierMode && app.mousePressed && app.selectedPointIndex != -1 && app.selectedSegmentIndex != -1) {
        app.dragBezierControlPoint();
    }
    else if(app.inCatmullRomMode && app.mousePressed && app.selectedPointIndex != -1){
        app.dragCatmullControlPoint();
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
        app.inCatmullRomMode = false;
        std::cout << "inBezierMode = true" << std::endl;
        app.splineSegments.clear();
        app.controlPoints.clear();
        app.selectedSegmentIndex = -1;
        app.selectedPointIndex = -1;
    }
    else if(key == GLFW_KEY_3 && action == GLFW_PRESS){
        app.selectedSegmentIndex = -1;
        app.selectedPointIndex = -1;
        app.curveFinalized = false;
        app.inBezierMode = false;
        app.inCatmullRomMode = true;
        std::cout << "inCatmullRomMode = true" << std::endl;
        app.splineSegments.clear();
        app.controlPoints.clear();
    }
    
    else if(key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE && app.selectedPointIndex != -1){
        app.deleteControlPoint();
        app.selectedSegmentIndex = -1;
        app.selectedPointIndex = -1;
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
        std::cout<<"Saving file"<<std::endl;
        app.saveSplineToFile("./etc/config.txt");
    } else if (key == GLFW_KEY_L && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
        app.loadSplineFromFile("./etc/config.txt");
    }
    else if(key == GLFW_KEY_INSERT || key == GLFW_KEY_I){
        if(action == GLFW_PRESS){
            app.insertPressed = true;
        }
        else{
            app.insertPressed = false;
        }
    }
    
}

void App::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (app.inBezierMode) {
        if(app.curveFinalized==false && action == GLFW_PRESS){
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                app.addBezierControlPoint(app.mousePos);
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT && app.splineSegments.size() > 0) {
                app.curveFinalized = true;
                app.addBezierControlPoint(app.mousePos);
                
            }
        }
        else if(app.curveFinalized && button == GLFW_MOUSE_BUTTON_LEFT){
            if(action == GLFW_PRESS){
                if(action == GLFW_PRESS && app.selectedPointIndex != -1 && app.insertPressed){
                    app.insertControlPoint();
                    app.selectedPointIndex = -1;
                    app.selectedSegmentIndex = -1;
                }
                else if(app.selectedPointIndex == -1)app.selectControlPoint();
                else {
                    app.selectedPointIndex = -1;
                    app.selectedSegmentIndex = -1;
                }
                app.mousePressed = true;
            }
            else if(action == GLFW_RELEASE){
                app.mousePressed = false;
                
            }

        }
    }
    if(app.inCatmullRomMode){
        if(app.curveFinalized==false && action == GLFW_PRESS){
            if(action == GLFW_PRESS && app.selectedPointIndex != -1 && app.insertPressed){
                app.insertControlPoint();
                app.selectedPointIndex = -1;
                app.selectedSegmentIndex = -1;
            }
            else if (button == GLFW_MOUSE_BUTTON_LEFT) {
                // std::cout<<app.splineSegments.size()<<"click\n";
                app.addCatmullRomControlPoint(app.mousePos);
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT && app.splineSegments.size() > 0) {
                app.curveFinalized = true;
                app.addCatmullRomControlPoint(app.mousePos); 
            }
        }
        else if(app.curveFinalized && button == GLFW_MOUSE_BUTTON_LEFT){
            if(action == GLFW_PRESS && app.selectedPointIndex != -1 && app.insertPressed){
                app.insertControlPoint();
            }
            else if(action == GLFW_PRESS){
                if(app.selectedPointIndex == -1)app.selectControlPoint();
                else {
                    app.selectedPointIndex = -1;
                    app.selectedSegmentIndex = -1;
                }
                app.mousePressed = true;
            }
            else if(action == GLFW_RELEASE){
                app.mousePressed = false;
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
    // pBezierShader = std::make_unique<Shader>("src/shader/bezier.vert.glsl",
    //                                          "src/shader/bezier.frag.glsl");

    pBezierShader = std::make_unique<Shader>("src/shader/bezier.vert.glsl",
                                         "src/shader/new_tcs.glsl",
                                         "src/shader/new_tes.glsl",
                                         "src/shader/bezier.frag.glsl");

    pControlPointShader = std::make_unique<Shader>("src/shader/control_point.vert.glsl",
                                               "src/shader/control_point.frag.glsl");

    pBezierShader3D = std::make_unique<Shader>("src/shader/bezier3d.vert.glsl",
                                           "src/shader/bezier3d.tcs.glsl",
                                           "src/shader/bezier3d.tes.glsl",
                                           "src/shader/bezier3d.frag.glsl");



    

    //E-Sri
}


void App::render()
{
    auto t = static_cast<float>(timeElapsedSinceLastFrame);

    
    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));
    renderSpline();
    renderControlPoints();

    

    

}

void App::renderSpline() {
    if (splineSegments.empty()) return;

    pBezierShader->use();
    pBezierShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pBezierShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));

    // Set tessellation level
    float tessLevel = 64.0f; // Adjust for desired smoothness
    pBezierShader->setFloat("tessLevel", tessLevel);

    std::vector<glm::vec2> origcontrolPoints;
    std::vector<std::vector<glm::vec2>> origsplineSegments;
    if (inCatmullRomMode) {
        origcontrolPoints = controlPoints;
        origsplineSegments = splineSegments;
        if (!curveFinalized && controlPoints.size() > 2) {
            addCatmullRomControlPoint(mousePos);
        }
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (size_t i = 0; i < splineSegments.size(); ++i) {
        auto& segment = splineSegments[i];
        std::vector<glm::vec2> currentSegment = segment;

        if (segment.size() < 4) {
            if (i == splineSegments.size() - 1) {
                while (currentSegment.size() < 4) {
                    currentSegment.push_back(mousePos);
                }
            } else {
                continue;
            }
        }

        // Send control points to GPU
        glBufferData(GL_ARRAY_BUFFER, currentSegment.size() * sizeof(glm::vec2), currentSegment.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(0);

        pBezierShader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f)); // Blue color for the curve

        // Draw using patches
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glDrawArrays(GL_PATCHES, 0, 4);
    }

    if (inCatmullRomMode) {
        controlPoints = origcontrolPoints;
        splineSegments = origsplineSegments;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void App::renderControlPoints() {
    if (inBezierMode && splineSegments.empty()) return;

    std::vector<glm::vec2> allControlPoints;
    if(inBezierMode) {
        for (const auto& segment : splineSegments) {
            allControlPoints.insert(allControlPoints.end(), segment.begin(), segment.end());
        }
    } else if(inCatmullRomMode) {
        allControlPoints = controlPoints;
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allControlPoints.size() * sizeof(glm::vec2), allControlPoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    pControlPointShader->use();
    pControlPointShader->setFloat("windowWidth", static_cast<float>(kWindowWidth));
    pControlPointShader->setFloat("windowHeight", static_cast<float>(kWindowHeight));
    pControlPointShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f)); // Red color for control points

    glPointSize(9.0f);
    glDrawArrays(GL_POINTS, 0, allControlPoints.size());

    if (inCatmullRomMode && selectedPointIndex != -1 && selectedPointIndex < allControlPoints.size()) {
        pControlPointShader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f)); // White color
        glDrawArrays(GL_POINTS, selectedPointIndex, 1);
    }

    if (inBezierMode && selectedSegmentIndex != -1 && selectedPointIndex != -1) {
        size_t globalIndex = 0;
        for (size_t j = 0; j < selectedSegmentIndex; ++j) {
            globalIndex += splineSegments[j].size();
        }
        globalIndex += selectedPointIndex;
        pControlPointShader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f)); // White color
        glDrawArrays(GL_POINTS, globalIndex, 1);
    }

    glPointSize(1.0f);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}




void App::addBezierControlPoint(const glm::vec2& point) {

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
        glm::vec2 P5 = P1 + 2.0f * P4 - 2.0f * P2;
        splineSegments.push_back({P3, P4, P5});
        std::cout<<"point are: p0 = "<<P0.x<<", "<<P0.y<<"; p1 = "<<P1.x<<", "<<P1.y<<";  p2 = "<<P2.x <<", "<<P2.y<<std::endl;
        std::cout<<"point are: p3 = "<<P3.x<<", "<<P3.y<<"; p4 = "<<P4.x<<", "<<P4.y<<";  p5 = "<<P5.x <<", "<<P5.y<<std::endl;
        // splineSegments.push_back({p0, p1, p2});
    }

}

void App::addCatmullRomControlPoint(const glm::vec2& point) {

    if(controlPoints.size() <= 2 ){
        controlPoints.push_back(point);
    }
    else { //controlPoints.size() >= 3
        controlPoints.push_back(point);
        int sizeS = controlPoints.size();
        glm::vec2 Pi_m1 = controlPoints[sizeS-4];
        glm::vec2 Pi = controlPoints[sizeS-3];
        glm::vec2 Pi_p1 = controlPoints[sizeS-2];
        glm::vec2 Pi_p2 = controlPoints[sizeS-1];
        glm::vec2 v1 = (Pi_p1 + 6.0f*Pi - Pi_m1)/6.0f;
        glm::vec2 v2 = (Pi + 6.0f*Pi_p1 - Pi_p2)/6.0f;
        splineSegments.push_back({Pi, v1, v2, Pi_p1});
        // std::cout<<splineSegments.size()<<"Ow\n";
    }

}

glm::vec2 App::evaluateBezier(const std::vector<glm::vec2>& controlPoints, float t) {
    float u = 1 - t;
    return u*u*u * controlPoints[0] + 
           3*u*u*t * controlPoints[1] + 
           3*u*t*t * controlPoints[2] + 
           t*t*t * controlPoints[3];
}

void App::selectControlPoint() {

    if(inBezierMode){
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
    else if(inCatmullRomMode){
        for (size_t i = 0; i < controlPoints.size(); ++i) {
            if (glm::distance(controlPoints[i], currentMousePos) < 10.0f) {
                // selectedSegmentIndex = i;
                selectedPointIndex = i;
                return;
            }
        }
    }
}

void App::dragBezierControlPoint() {

    if (selectedSegmentIndex == -1 || selectedPointIndex == -1) return;



    if(inBezierMode){
        glm::vec2 oldPosition = splineSegments[selectedSegmentIndex][selectedPointIndex];
        glm::vec2 newPosition = currentMousePos;
        glm::vec2 delta = newPosition - oldPosition;
        // std::cout<<selectedSegmentIndex<< " "<<selectedPointIndex<<std::endl;

        splineSegments[selectedSegmentIndex][selectedPointIndex] = newPosition;
        if(selectedSegmentIndex > 0 && selectedSegmentIndex<splineSegments.size()-1 && selectedPointIndex == 3){
            splineSegments[selectedSegmentIndex+1][0] = newPosition;
            selectedSegmentIndex++;
            selectedPointIndex = 0;
        }

        // Update adjacent control points to maintain C2 continuity
        if (splineSegments.size() > 1) {
            if(selectedSegmentIndex == 0){    
                for (size_t i = 1; i < splineSegments.size(); ++i) {

                    glm::vec2 P1 = splineSegments[i-1][1];
                    glm::vec2 P2 = splineSegments[i-1][2];
                    glm::vec2 P3 = splineSegments[i-1][3];

                    glm::vec2 P4 = P3 + (P3 - P2);
                    glm::vec2 P5 = P1 + 2.0f * P4 - 2.0f * P2;
                    
                    splineSegments[i][0] = P3;
                    splineSegments[i][1] = P4;
                    splineSegments[i][2] = P5;

                    
                }
            }else {
                // Middle segments
                if (selectedPointIndex == 0) {
                    for(int i = selectedSegmentIndex; i<splineSegments.size(); ++i){
                        glm::vec2 P1 = splineSegments[i-1][1];
                        glm::vec2 P2 = splineSegments[i-1][2];
                        glm::vec2 P3 = splineSegments[i][0];
                        splineSegments[i-1][3] = P3;

                        glm::vec2 P4 = P3 + (P3 - P2);
                        glm::vec2 P5 = P1 + 2.0f * P4 - 2.0f * P2;
                        
                        splineSegments[i][0] = P3;
                        splineSegments[i][1] = P4;
                        splineSegments[i][2] = P5;
                    }
                    
                } else if (selectedPointIndex == 1) {
                    // P1 of middle segment moved (affects previous segment)
                    glm::vec2 P2 = splineSegments[selectedSegmentIndex - 1][2];
                    glm::vec2 P1 = splineSegments[selectedSegmentIndex - 1][1]; 
                    glm::vec2 P4 = splineSegments[selectedSegmentIndex][1];

                    splineSegments[selectedSegmentIndex - 1][3] = 0.5f*(P4+splineSegments[selectedSegmentIndex - 1][2]);
                    splineSegments[selectedSegmentIndex][0] = splineSegments[selectedSegmentIndex - 1][3];
                    splineSegments[selectedSegmentIndex][2] = P1 + 2.0f * P4 - 2.0f * P2;

                    for(int i = selectedSegmentIndex+1; i<splineSegments.size(); ++i){
                        glm::vec2 P1 = splineSegments[i-1][1];
                        glm::vec2 P2 = splineSegments[i-1][2];
                        glm::vec2 P3 = splineSegments[i][0];
                        splineSegments[i-1][3] = P3;

                        glm::vec2 P4 = P3 + (P3 - P2);
                        glm::vec2 P5 = P1 + 2.0f * P4 - 2.0f * P2;
                        
                        splineSegments[i][0] = P3;
                        splineSegments[i][1] = P4;
                        splineSegments[i][2] = P5;
                    }
                    
                } else if (selectedPointIndex == 2) {
                    glm::vec2 P2 = splineSegments[selectedSegmentIndex - 1][2];
                    glm::vec2 P1 = splineSegments[selectedSegmentIndex - 1][1]; 
                    glm::vec2 P5 = splineSegments[selectedSegmentIndex][2]; 
                    splineSegments[selectedSegmentIndex][1] = 0.5f*(P5 + 2.0f*P2 - P1);
                    glm::vec2 P4 = splineSegments[selectedSegmentIndex][1];
                    splineSegments[selectedSegmentIndex - 1][3] = 0.5f*(P4+splineSegments[selectedSegmentIndex - 1][2]);
                    splineSegments[selectedSegmentIndex][0] = splineSegments[selectedSegmentIndex - 1][3];
                    for(int i = selectedSegmentIndex+1; i<splineSegments.size(); ++i){
                        glm::vec2 P1 = splineSegments[i-1][1];
                        glm::vec2 P2 = splineSegments[i-1][2];
                        glm::vec2 P3 = splineSegments[i][0];
                        splineSegments[i-1][3] = P3;

                        glm::vec2 P4 = P3 + (P3 - P2);
                        glm::vec2 P5 = P1 + 2.0f * P4 - 2.0f * P2;
                        
                        splineSegments[i][0] = P3;
                        splineSegments[i][1] = P4;
                        splineSegments[i][2] = P5;
                    }
                    // P2 of middle segment moved (affects next segment)
                    // splineSegments[selectedSegmentIndex + 1][0] = splineSegments[selectedSegmentIndex][3];
                    // splineSegments[selectedSegmentIndex + 1][1] = 2.0f * splineSegments[selectedSegmentIndex + 1][0] - newPosition;
                } else if (selectedPointIndex == 3) {
                    // P3 of middle segment moved (affects next segment)
                    for(int i = selectedSegmentIndex+1; i<splineSegments.size(); ++i){
                        glm::vec2 P1 = splineSegments[i-1][1];
                        glm::vec2 P2 = splineSegments[i-1][2];
                        glm::vec2 P3 = splineSegments[i][0];
                        splineSegments[i-1][3] = P3;

                        glm::vec2 P4 = P3 + (P3 - P2);
                        glm::vec2 P5 = P1 + 2.0f * P4 - 2.0f * P2;
                        
                        splineSegments[i][0] = P3;
                        splineSegments[i][1] = P4;
                        splineSegments[i][2] = P5;
                    }
                }
            }
        }
    }
}

void App::dragCatmullControlPoint(){
    if(selectedPointIndex == -1)return;
    controlPoints[selectedPointIndex] = currentMousePos;
    buildBezierFromCatmullRom();
}



void App::insertControlPoint() {
    if (selectedPointIndex != -1) {
        if(inBezierMode){
            glm::vec2 newPoint = (splineSegments[selectedSegmentIndex][selectedPointIndex] +
                                splineSegments[selectedSegmentIndex][(selectedPointIndex + 1) % 4]) * 0.5f;
            splineSegments[selectedSegmentIndex].insert(splineSegments[selectedSegmentIndex].begin() + selectedPointIndex + 1, newPoint);

            // Recalculate control points to maintain C2 continuity
            if (splineSegments.size() > 1 && selectedSegmentIndex < splineSegments.size() - 1) {
                auto& currSegment = splineSegments[selectedSegmentIndex];
                auto& nextSegment = splineSegments[selectedSegmentIndex + 1];
                nextSegment[1] = 2.0f * nextSegment[0] - currSegment[2];
                nextSegment[2] = 2.0f * nextSegment[1] - nextSegment[0];
            }
        }
        else if(inCatmullRomMode){
            std::vector<glm::vec2> tempControlPoints(controlPoints.size()+1);
            for(int i = 0; i<=selectedPointIndex; i++){
                tempControlPoints[i] = controlPoints[i];
            }
            tempControlPoints[selectedPointIndex+1] = currentMousePos;
            for(int i = selectedPointIndex+1; i<controlPoints.size(); i++){
                tempControlPoints[i+1] = controlPoints[i];
            }
            controlPoints = tempControlPoints;
            buildBezierFromCatmullRom();
        }
    }
}

void App::deleteControlPoint() {
    if(inBezierMode){
        if (selectedPointIndex != -1 && selectedSegmentIndex != -1) {
            splineSegments[selectedSegmentIndex].erase(splineSegments[selectedSegmentIndex].begin() + selectedPointIndex);

            // Recalculate control points to maintain C2 continuity
            if (splineSegments.size() > 1 && selectedSegmentIndex < splineSegments.size() - 1) {
                auto& currSegment = splineSegments[selectedSegmentIndex];
                auto& nextSegment = splineSegments[selectedSegmentIndex + 1];
                nextSegment[1] = 2.0f * nextSegment[0] - currSegment[2];
                nextSegment[2] = 2.0f * nextSegment[1] - nextSegment[0];
            }
        }
    }
    else if(inCatmullRomMode){
        if(selectedPointIndex!=-1){
            std::vector<glm::vec2> tempControlPoints(controlPoints.size()-1);
            for(int i = 0; i<selectedPointIndex; i++){
                tempControlPoints[i] = controlPoints[i];
            }
            // tempControlPoints[selectedPointIndex+1] = currentMousePos;
            for(int i = selectedPointIndex+1; i<controlPoints.size(); i++){
                tempControlPoints[i-1] = controlPoints[i];
            }
            controlPoints = tempControlPoints;
            buildBezierFromCatmullRom();
        }
    }
}


void App::saveSplineToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open() && inBezierMode) {
        file << "2 2 " << getTotalControlPoints() << std::endl;
        
        // Write the first segment completely
        if (!splineSegments.empty()) {
            for (const auto& point : splineSegments[0]) {
                file << point.x << " " << point.y << std::endl;
            }
            
            // For subsequent segments, only write the last 3 points
            for (size_t i = 1; i < splineSegments.size(); ++i) {
                for (size_t j = 1; j < splineSegments[i].size(); ++j) {
                    file << splineSegments[i][j].x << " " << splineSegments[i][j].y << std::endl;
                }
            }
        }
        
        file.close();
    }
    else if (file.is_open() && inCatmullRomMode) {
        file << "2 1 " << getTotalControlPoints() << std::endl;
        
        // Write the first segment completely
        for(int i = 0; i<controlPoints.size(); i++){
            file << controlPoints[i].x << " " << controlPoints[i].y << std::endl;
        }
        
        file.close();
    }
}

int App::getTotalControlPoints() const {

    if(inBezierMode){
        if (splineSegments.empty()) return 0;
    
        // Count all points in the first segment
        int total = splineSegments[0].size();
        
        // For subsequent segments, only count the last 3 points
        for (size_t i = 1; i < splineSegments.size(); ++i) {
            total += std::max(0, static_cast<int>(splineSegments[i].size()) - 1);
        }
        
        return total;
    }
    else if(inCatmullRomMode){
        return controlPoints.size();
    }
    std::cout<<"ERROR MODE IN getTOTAL CONTROL POINTS\n";
    return -1;
}


void App::loadSplineFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        curveFinalized = false;
        int dim, continuity, totalPoints;
        file >> continuity >> dim >> totalPoints;
        
        if(dim == 2){
            if(!inBezierMode){
                std::cout<<"Converting to Bezier Mode since loading a curve with C2 satisfied";
                inBezierMode = true;
                inCatmullRomMode = false;
            }
            splineSegments.clear();
            controlPoints.clear();
            splineSegments.push_back({});
            for (int i = 0; i < totalPoints-1; ++i) {
                float x, y;
                file >> x >> y;
                splineSegments.back().push_back(glm::vec2(x,y));
                if(splineSegments.back().size() == 4){
                    splineSegments.push_back({});
                    splineSegments.back().push_back(glm::vec2(x,y));
                }
            }
            curveFinalized = true;
            float x, y;
            file >> x >> y;
            splineSegments.back().push_back(glm::vec2(x,y));
            file.close();
        }
        else if(dim == 1){
            if(!inCatmullRomMode){
                std::cout<<"Converting to inCatmullRom Mode since loading a curve with C1 satisfied";
                inBezierMode = false;
                inCatmullRomMode = true;
            }
            splineSegments.clear();
            controlPoints.clear();
            for (int i = 0; i < totalPoints; ++i) {
                float x, y;
                file >> x >> y;
                controlPoints.push_back(glm::vec2(x,y));
                std::cout<<x<<" "<<y<<std::endl;
            }
            curveFinalized=true;
            buildBezierFromCatmullRom();

        }
    }
}

std::vector<glm::vec2> App::calculateNewSegmentPoints(const std::vector<glm::vec2>& prevSegment, const glm::vec2& newPoint) {
    glm::vec2 p0 = prevSegment[3];
    glm::vec2 p1 = 2.0f * prevSegment[3] - prevSegment[2];
    glm::vec2 p2 = (2.0f * p1 - p0 + newPoint) / 2.0f;
    return {p0, p1, p2};
}

void App::buildBezierFromCatmullRom(){
    splineSegments.clear();
    if(controlPoints.size()<4)return;
    for(int i = 3; i<controlPoints.size(); i++){
        glm::vec2 Pi_m1 = controlPoints[i-3];
        glm::vec2 Pi = controlPoints[i-2];
        glm::vec2 Pi_p1 = controlPoints[i-1];
        glm::vec2 Pi_p2 = controlPoints[i];
        glm::vec2 v1 = (Pi_p1 + 6.0f*Pi - Pi_m1)/6.0f;
        glm::vec2 v2 = (Pi + 6.0f*Pi_p1 - Pi_p2)/6.0f;
        splineSegments.push_back({Pi, v1, v2, Pi_p1});
    }
}

//bezier config
// 2 2 13
// 432 484
// 454 536
// 496 536
// 523 488
// 550 440
// 562 344
// 511 341
// 460 338
// 346 428
// 340 526
// 334 624
// 436 730
// 554 627

//catmullrom Config


