#ifndef APP_H
#define APP_H

#include "Context.h"
#include "../shaders/Shader.h"
#include "../utilities/Camera.h"
#include "../libs/stb_image.h"
#include "../utilities/Utils.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

#define RANDOM (float)rand() / RAND_MAX

#define MAX_CUBES 700000
#define MIN_CUBES 0

#define MAX_RENDER_AMOUNT 25000
#define MIN_RENDER_AMOUNT 1


class App : public Context{  
    //can add additional methods, but these 4 are all you really need
    public:
        virtual void onStart();
        virtual void onUpdate();
        virtual void onRender();
        ~App();

    //define any attirbutes your app needs
    private:

        bool mEnableInstanced;
        bool mRenderWindow;

        Camera* mCamera;

        float mCurrFrame;
        float mDeltaTime;
        float mLastFrame = 0.0f;

        glm::mat4 mProjection;
        glm::mat4 mView;
        glm::mat4* mTransforms;
        glm::mat4* mNormals;

        glm::vec3* mColours;

        Shader* mInstancedShader;
        Shader* mNoInstancedShader;
        Shader* mCubeMapShader;

        uint32_t mCubes;
        uint32_t mRenderAmount;
        uint32_t mCubeMapVao;
        uint32_t mInstancedCubeVao;
        uint32_t mNoInstancedCubeVao;
        uint32_t mCubeMapTexture;
};
#endif