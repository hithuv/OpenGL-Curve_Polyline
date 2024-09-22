#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.h"
#include "shape/Pixel.h"
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

    if (app.mousePressed)
    {
        // // Note: Must calculate offset first, then update lastMouseLeftPressPos.
        // glm::dvec2 offset = app.mousePos - app.lastMouseLeftPressPos;
        app.lastMouseLeftPressPos = app.mousePos;
    }

    // Display a preview line which moves with the mouse cursor iff.
    // the most-recent mouse click is left click.
    // showPreview is controlled by mouseButtonCallback.



    

    if (app.showPreview && (app.currentMode == LINE_MODE || app.currentMode == POLYLINE_MODE))
    {

        auto pixel = dynamic_cast<Pixel *>(app.shapes.front().get());
        
        auto x0 = static_cast<int>(app.lastMouseLeftPressPos.x);
        auto y0 = static_cast<int>(app.lastMouseLeftPressPos.y);
        auto x1 = static_cast<int>(app.mousePos.x);
        auto y1 = static_cast<int>(app.mousePos.y);

        pixel->path.clear();

        if(app.currentMode == POLYLINE_MODE){
            int polyLineSize = app.polylineCorners.size();
            for(int i = 0; i<polyLineSize - 1; i++){
                // std::cout<<"{"<<app.polylineCorners[i][0].x<<" ,"<< app.polylineCorners[i][0].y<<"} ,{"<< app.polylineCorners[i][1].x<<" ,"<< app.polylineCorners[i][1].y<<" }"<<std::endl;
                bresenhamLine(pixel->path, app.polylineCorners[i][0].x, app.polylineCorners[i][0].y, app.polylineCorners[i][1].x, app.polylineCorners[i][1].y);
            }
        }
        
        bresenhamLine(pixel->path, x0, y0, x1, y1);
        pixel->dirty = true;
    }

    else if (app.currentMode == POLYLINE_MODE && !app.showPreview && app.polylineCorners.size())
    {
        auto pixel = dynamic_cast<Pixel *>(app.shapes.front().get());
        


        auto x0 = static_cast<int>(app.lastMouseLeftPressPos.x);
        auto y0 = static_cast<int>(app.lastMouseLeftPressPos.y);
        auto x1 = static_cast<int>(app.mousePos.x);
        auto y1 = static_cast<int>(app.mousePos.y);

        pixel->path.clear();
        int polyLineSize = app.polylineCorners.size();
        for(int i = 0; i<polyLineSize; i++){
            // std::cout<<"{"<<app.polylineCorners[i][0].x<<" ,"<< app.polylineCorners[i][0].y<<"} ,{"<< app.polylineCorners[i][1].x<<" ,"<< app.polylineCorners[i][1].y<<" }"<<std::endl;
            bresenhamLine(pixel->path, app.polylineCorners[i][0].x, app.polylineCorners[i][0].y, app.polylineCorners[i][1].x, app.polylineCorners[i][1].y);
        }
        if(app.cPressed){
            // std::cout<<"{"<<app.polylineCorners[0][0].x<<" ,"<< app.polylineCorners[0][0].y<<"} ,{"<< app.polylineCorners.back()[1].x<<" ,"<< app.polylineCorners.back()[1].y<<" }"<<std::endl;
            bresenhamLine(pixel->path, app.polylineCorners[0][0].x, app.polylineCorners[0][0].y, app.polylineCorners.back()[1].x, app.polylineCorners.back()[1].y);
        }
        // std::cout<<"---"<<std::endl;
        
        // bresenhamLine(pixel->path, x0, y0, x1, y1);
        pixel->dirty = true;
        app.polylineCorners.clear();
    }
    else if(app.showPreview && app.currentMode == ELLIPSE_MODE){
        auto pixel = dynamic_cast<Pixel *>(app.shapes.front().get());
        
        auto x0 = static_cast<int>(app.lastMouseLeftPressPos.x);
        auto y0 = static_cast<int>(app.lastMouseLeftPressPos.y);
        auto x1 = static_cast<int>(app.mousePos.x);
        auto y1 = static_cast<int>(app.mousePos.y);

        pixel->path.clear();

        drawEllipse(pixel->path, x0, y0, x1, y1);

        pixel->dirty = true;
    }
    else if(app.showPreview && app.currentMode == CIRCLE_MODE){
        auto pixel = dynamic_cast<Pixel *>(app.shapes.front().get());
        
        auto x0 = static_cast<int>(app.lastMouseLeftPressPos.x);
        auto y0 = static_cast<int>(app.lastMouseLeftPressPos.y);
        auto x1 = static_cast<int>(app.mousePos.x);
        auto y1 = static_cast<int>(app.mousePos.y);
        int r2 = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
        int r = sqrt(r2);

        pixel->path.clear();

        drawEllipse(pixel->path, x0, y0, x0+r, y0+r);

        pixel->dirty = true;
    }

}


void App::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void App::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        app.animationEnabled = !app.animationEnabled;
    }

    if(key == GLFW_KEY_1){
        app.polylineCorners.clear();
        app.showPreview = false;
        app.currentMode = LINE_MODE;
    }
    if(key == GLFW_KEY_3){
        // app.polylineCorners.clear();
        app.showPreview = false;
        app.currentMode = POLYLINE_MODE;
    }
    if(key == GLFW_KEY_4){
        app.polylineCorners.clear();
        app.showPreview = false;
        app.currentMode = ELLIPSE_MODE;
    }

    if (key == GLFW_KEY_C){
        if (action == GLFW_PRESS) {
            app.cPressed = true;
        }
        else if (action == GLFW_RELEASE) {
            app.cPressed = false;
        }
    }

    if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT){
        if (action == GLFW_PRESS) {
            app.shiftPressed = true;
        }
        else if (action == GLFW_RELEASE) {
            app.shiftPressed = false;
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
            if(app.currentMode == ELLIPSE_MODE && app.shiftPressed == true){
                app.currentMode = CIRCLE_MODE;
            }
            if(app.currentMode == CIRCLE_MODE && app.shiftPressed == false){
                app.currentMode = ELLIPSE_MODE;
            }

            app.mousePressed = false;
            app.showPreview = true;
            if(app.currentMode == POLYLINE_MODE){
                if(app.polylineCorners.size() != 0)app.polylineCorners.back().push_back(app.mousePos);
                app.polylineCorners.push_back({app.mousePos});
            }
            
            #ifdef DEBUG_MOUSE_POS
            std::cout << "[ " << app.mousePos.x << ' ' << app.mousePos.y << " ]\n";
            #endif
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        
            
        // app.polylineCorners.push_back({app.mousePos});
        if (action == GLFW_RELEASE)
        {
            if(app.currentMode == POLYLINE_MODE)if(app.polylineCorners.size() != 0)app.polylineCorners.back().push_back(app.mousePos);
            app.showPreview = false;
            
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


void App::bresenhamLine(std::vector<Pixel::Vertex> & path, int x0, int y0, int x1, int y1)
{
    // int x1_temp = x1, y1_temp = y1;
    // if(x1 < x0){
    //     x1 = 2*x0 - x1;
    // }
    // if(y1 < y0){
    //     y1 = 2*y0 - y1;
    // }

    
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int p = 2 * dy - dx;
    int twoDy = 2 * dy;
    int twoDyMinusDx = 2 * (dy - dx);

    int x = x0;
    int y = y0;

    //handle 1<m<infi
    if(dy>dx){
        if(y1<y0){
            bresenhamLine(path, x1, y1, x0, y0);
        }
        p = 2*dx-dy;
        int twoDx = 2*dx;
        int twoDxMinusDy = 2*(dx - dy);

        path.emplace_back(x, y, 1.0f, 1.0f, 1.0f);

        while (y<y1)
        {
            ++y;

            if (p < 0)
            {
                p += twoDx;
            }
            else
            {
                ++x;
                p += twoDxMinusDy;
            }
            if(x1>=x0){
                path.emplace_back(x, y, 1.0f, 1.0f, 1.0f);
            }
            else if(x1<=x0){
                path.emplace_back(2*x0 - x, y, 1.0f, 1.0f, 1.0f);
            }
        }

        return;

    }

    else{
        if(x1<x0){
            bresenhamLine(path, x1, y1, x0, y0);
            return;
        }

        

        path.emplace_back(x, y, 1.0f, 1.0f, 1.0f);


        while (x < x1)
        {
            ++x;

            if (p < 0)
            {
                p += twoDy;
            }
            else
            {
                ++y;
                p += twoDyMinusDx;
            }
            if(y1<=y0 && x1>x0){
                path.emplace_back(x, y0 - (y-y0), 1.0f, 1.0f, 1.0f);
            }
            else if(y1>=y0 && x1>x0){
                path.emplace_back(x, y, 1.0f, 1.0f, 1.0f);
            }
        }
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glLineWidth(1.0f);
    glPointSize(1.0f);

    // Initialize shaders and objects-to-render;
    pPixelShader = std::make_unique<Shader>("src/shader/pixel.vert.glsl",
                                            "src/shader/pixel.frag.glsl");

    shapes.emplace_back(std::make_unique<Pixel>(pPixelShader.get()));
}


void App::render()
{
    // Update all shader uniforms.
    pPixelShader->use();
    pPixelShader->setFloat("windowWidth", kWindowWidth);
    pPixelShader->setFloat("windowHeight", kWindowHeight);

    // Render all shapes.
    for (auto & s : shapes)
    {
        s->render();
    }
}

//Srihith
void App::drawEllipse(std::vector<Pixel::Vertex> & path, int xc, int yc, int rx_actual, int ry_actual){

    if(rx_actual < xc){
        rx_actual = xc + xc - rx_actual;
    }
    if(ry_actual < yc){
        ry_actual = yc + yc - ry_actual;
    }

    int rx = rx_actual - xc;
    int ry = ry_actual - yc;
    int x = 0;
    int y = ry;

    double p1 = ry*ry - rx*rx*ry + 0.25*rx*rx;
    path.emplace_back(xc+x, yc+y, 1.0f, 1.0f, 1.0f);
    path.emplace_back(xc-x, yc+y, 1.0f, 1.0f, 1.0f);
    path.emplace_back(xc+x, yc-y, 1.0f, 1.0f, 1.0f);
    path.emplace_back(xc-x, yc-y, 1.0f, 1.0f, 1.0f);


    while(2*rx*rx*y >= 2*ry*ry*(++x)){
        if(p1< 0){
            p1 += 2*ry*ry*x + ry*ry;
        }
        else{
            y -= 1; 
            p1+= 2*ry*ry*x - 2*rx*rx*y + ry*ry;
        }
        path.emplace_back(xc+x, yc+y, 1.0f, 1.0f, 1.0f);
        path.emplace_back(xc-x, yc+y, 1.0f, 1.0f, 1.0f);
        path.emplace_back(xc+x, yc-y, 1.0f, 1.0f, 1.0f);
        path.emplace_back(xc-x, yc-y, 1.0f, 1.0f, 1.0f);
    }

    x = rx;
    y = 0;

    double p2 = rx*rx - ry*ry*rx + 0.25*ry*ry;
    path.emplace_back(xc+x, yc+y, 1.0f, 1.0f, 1.0f);
    path.emplace_back(xc-x, yc+y, 1.0f, 1.0f, 1.0f);
    path.emplace_back(xc+x, yc-y, 1.0f, 1.0f, 1.0f);
    path.emplace_back(xc-x, yc-y, 1.0f, 1.0f, 1.0f);
    while(2*rx*rx*(++y) < 2*ry*ry*(x)){
        if(p2< 0){
            p2 += 2*rx*rx*y + rx*rx;
        }
        else{
            x -= 1; 
            p2+= 2*rx*rx*y - 2*ry*ry*x + rx*rx;
        }
        path.emplace_back(xc+x, yc+y, 1.0f, 1.0f, 1.0f);
        path.emplace_back(xc-x, yc+y, 1.0f, 1.0f, 1.0f);
        path.emplace_back(xc+x, yc-y, 1.0f, 1.0f, 1.0f);
        path.emplace_back(xc-x, yc-y, 1.0f, 1.0f, 1.0f);
    }


}
