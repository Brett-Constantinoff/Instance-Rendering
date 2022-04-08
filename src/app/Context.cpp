#include "Context.h"
#include <iostream>


//function that starts your application
void Context::start( void ){
    init();
    run();
    destroy();
}

//initializes the window and ImGui
void Context::init( void ){
    initContext();
    initImGui();
}

//destroys any and all resources allocated for the app
void Context::destroy( void ){
    destroyRes();
    destroyWindow();
    destroyImGui();
}

//inits GLFW and GLEW
void Context::initContext( ){
     /* INIT GLFW */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* WINDOW INIT */
    mID = glfwCreateWindow(mWidth, mHeight, mName, NULL, NULL);
    if(!mID){
        std::cerr << "Error creating OpenGL window" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(mID);
    glViewport(0, 0, mWidth, mHeight);

    /* GLEW INIT */
    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        std::cerr << "ERROR INITIALIZING GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    //renders 3D vertices properly
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}

//inits ImGui
void Context::initImGui( void ){
      /* IMGUI INIT */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; //sets up input / output

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(mID, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

//start of frame, clear buffers and tell ImGui to start a new frame
void Context::startFrame( void ){
    //set the clear colour and clear the buffers
    glClearColor(mColour.r, mColour.g, mColour.b, mColour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setup imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

//main application loop
void Context::run( void ){
    onStart();
    while(!glfwWindowShouldClose(mID)){
        startFrame();
        onUpdate();
        onRender();
        endFrame();
    }
}

//render ImGui data and swap buffer / poll events
void Context::endFrame( void ){

    //swap front and back buffers and poll IO events(buttons presses, mouse movement etc)
    glfwSwapBuffers(*getID());
    glfwPollEvents();
}

//deallocate ImGui stuff
void Context::destroyImGui( void ){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

//terminates the window
void Context::destroyWindow( void ){
    glfwDestroyWindow(mID);
    glfwTerminate();
}


//destorys any programs / buffers used for the app
void Context::destroyRes( void ){
    for(auto vao: mVaos){
        glDeleteVertexArrays(1, &vao);
    }
    for(auto vbo: mVbos){
        glDeleteBuffers(1, &vbo);
    }
    for(auto ibo: mIbos){
        glDeleteBuffers(1, &ibo);
    }
    for(auto tbo: mTbos){
        glDeleteTextures(1, &tbo);
    }
    for(auto program: mPrograms){
        glDeleteProgram(program);
    }
}

//returns the context ID
GLFWwindow** Context::getID( void ){
    return &mID;
}

//returns a projection matrix for perspective viewing
const glm::mat4* Context::getPersepctiveView( void ){
    return &mPerspective; 
}