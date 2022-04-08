#include "App.h"

//runs once on startup
void App::onStart(){
    /*
    init app attributes
    */
    mRenderWindow = false;
    mEnableInstanced = false;

    mCubes = 0;
    mRenderAmount = 1;
    mCamera = new Camera(glm::vec3(0.0f, 0.0f ,5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    mInstancedShader = new Shader("src/shaders/glslShaders/instancedShader.txt");
    mPrograms.push_back(mInstancedShader->getId()); //add to programs, for deallocation later

    mNoInstancedShader = new Shader("src/shaders/glslShaders/noInstancedShader.txt");
    mPrograms.push_back(mNoInstancedShader->getId());

    mCubeMapShader = new Shader("src/shaders/glslShaders/skyBoxShader.txt");
    mPrograms.push_back(mCubeMapShader->getId());

    /*
    Skybox geometry
    */
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    mCubeMapTexture = Utils::loadCubeMap();
    mTbos.push_back(mCubeMapTexture);

    mCubeMapVao = Utils::genVertexArray();
    mVaos.push_back(mCubeMapVao);

    uint32_t cubeMapVbo = Utils::genBuffer();
    mVbos.push_back(cubeMapVbo);

    glBindVertexArray(mCubeMapVao);
    glBindBuffer(GL_ARRAY_BUFFER, cubeMapVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glUseProgram(mCubeMapShader->getId());
    mCubeMapShader->setInt("skybox", 0);

    /*
    Cube geometry
    */
    float cubeVertexData[] = {
       //positions          //normals
       0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 
       1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 
       1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 
       0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,

       
       1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   
       1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,  
       1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 
       1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 

       
       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
       0.0f, 1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 
       1.0f, 1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 
       1.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,

      
       0.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f, 
       0.0f, 1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 

       
       0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
       1.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,  
       0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,  
       1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,  

       
       0.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f, 
       0.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f, 
       1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f, 
       1.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f, 
    };
    uint32_t indices[] = {
        0,  1,  2,      0,  2,  3,
        4,  5,  6,      4,  6,  7,   
        8,  9,  10,     8,  10, 11,  
        12, 13, 14,     12, 14, 15, 
        16, 17, 18,     17, 18, 19,  
        20, 21, 22,     21, 22, 23
    };
    /*
    Setup instanced cubes
    */
    mInstancedCubeVao = Utils::genVertexArray();
    mVaos.push_back(mInstancedCubeVao);

    uint32_t cubeVbo = Utils::genBuffer();
    mVbos.push_back(cubeVbo);

    uint32_t cubeIbo = Utils::genBuffer();
    mIbos.push_back(cubeIbo);

    glBindVertexArray(mInstancedCubeVao);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), &cubeVertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //cube transformations and colours
    mTransforms = new glm::mat4[MAX_CUBES];
    mColours = new glm::vec3[MAX_CUBES];
    for (int i = 0; i < MAX_CUBES; i++){
        glm::vec3 colour = {RANDOM, RANDOM, RANDOM};
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), {i * (RANDOM - 0.5f), i * (RANDOM - 0.5f), i * (RANDOM - 0.5f)}) *
                          glm::rotate(glm::mat4(1.0f), RANDOM * i * glm::radians(45.0f), {1.0f, 1.0f, 1.0f}) * 
                          glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f});
        glm::mat4 normal = glm::transpose(transform);
        normal = glm::inverse(normal);
     
        mColours[i] = colour;
        mTransforms[i] = transform;
    }
    uint32_t colourBuffer = Utils::genBuffer();
    mVbos.push_back(colourBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * sizeof(glm::vec3), &mColours[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    //tell opengl this attribute is per instance not per vertex
    glVertexAttribDivisor(2, 1);

    uint32_t transformBuffer = Utils::genBuffer();
    mVbos.push_back(transformBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, transformBuffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_CUBES * sizeof(glm::mat4), &mTransforms[0], GL_STATIC_DRAW);

    //1st row of matrix
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

    //2nd row of matrix
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

    //3rd row of matrix
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

    //4th row of matrix
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    //all these are per instance
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);

    /*
    Setup non-instanced rendered cubes
    */
    mNoInstancedCubeVao = Utils::genVertexArray();
    mVaos.push_back(mNoInstancedCubeVao);

    uint32_t nonInstancedPosVbo = Utils::genBuffer();
    mVbos.push_back(nonInstancedPosVbo);

    uint32_t nonInstancedIbo = Utils::genBuffer();
    mIbos.push_back(nonInstancedIbo);

    glBindVertexArray(mNoInstancedCubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, nonInstancedPosVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nonInstancedIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

//runs everyframe
void App::onUpdate(){
    //setup time
    mCurrFrame = glfwGetTime();
    mDeltaTime = mCurrFrame - mLastFrame;
    mLastFrame = mCurrFrame;

    //camera movement
    mCamera->move(this->getID(), mDeltaTime);

    //view and projection matrices
    mProjection = *this->getPersepctiveView();

    //configure imgui with what you want to draw
    ImGui::Begin("Main Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("%s: %.3f %.3f %.3f", "Camera Position", (*mCamera->getPosition()).x, (*mCamera->getPosition()).y, (*mCamera->getPosition()).z);
    ImGui::Checkbox("Render Details", &mRenderWindow);
    ImGui::End();

    if(mRenderWindow){
        ImGui::Begin("Render Window");
        ImGui::Checkbox("Enable Instanced Rendering", &mEnableInstanced);
        ImGui::SliderInt("Render amount", (int*)&mRenderAmount, MIN_RENDER_AMOUNT, MAX_RENDER_AMOUNT);
        if(ImGui::Button("Press to render cubes")){
            if(mCubes + mRenderAmount <= MAX_CUBES){
                mCubes += mRenderAmount;
            }
        }

        if(ImGui::Button("Press to de-render cubes")){
            //static case to normal ints so we can cheat buffer overflow
            if((static_cast<int>(mCubes) - static_cast<int>(mRenderAmount) >= MIN_CUBES)){
                mCubes -= mRenderAmount;
            }
        }
        ImGui::Text("Cubes rendered: %d", mCubes);
        ImGui::End();
    }
}

//render calls go here
void App::onRender(){
   
    //render cube using instancing
    if(mEnableInstanced){
        glUseProgram(mInstancedShader->getId());

        mInstancedShader->setMat4("uProjection", mProjection);
        mView = *mCamera->getView();
        mInstancedShader->setMat4("uView", mView);

        glBindVertexArray(mInstancedCubeVao);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, mCubes);
        glBindVertexArray(0);
    }
    //render cubes normally
    else{
        glUseProgram(mNoInstancedShader->getId());

        mNoInstancedShader->setMat4("uProjection", mProjection);
        mView = *mCamera->getView();
        mNoInstancedShader->setMat4("uView", mView);

        for(int i = 0; i < mCubes; i++){
            mNoInstancedShader->setVec3("uColour", mColours[i]);
            mNoInstancedShader->setMat4("uTransform", mTransforms[i]);
            glBindVertexArray(mNoInstancedCubeVao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    //render skybox
    glDepthFunc(GL_LEQUAL);

    glUseProgram(mCubeMapShader->getId());

    //remove translation from view so skybox renders properly
    glm::mat4 view = glm::mat4(glm::mat3(*mCamera->getView()));
    mCubeMapShader->setMat4("uProjection", mProjection);
    mCubeMapShader->setMat4("uView", view);

    glBindVertexArray(mCubeMapVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    //render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


//any memory allocated for the app needs to be deleted
App::~App(){
    delete mNoInstancedShader;
    delete mInstancedShader;
    delete mCubeMapShader;
    delete mCamera;
    delete[] mTransforms;
    delete[] mColours;
}