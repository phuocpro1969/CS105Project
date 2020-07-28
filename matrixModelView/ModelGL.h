#pragma once
#ifndef MODEL_GL_H
#define MODEL_GL_H

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <string>
#include "Matrices.h"
#include "glext.h"
#include "glExtension.h"
#include "resource.h"
#include "GL/GLU.H"

class ModelGL
{
public:
    ModelGL();
    ~ModelGL();

    void init();                                    // initialize OpenGL states
    bool initShaders();                             // init shader programs
    void quit();                                    // clean up OpenGL objects
    void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
    void draw();

    void DrawWithShape();

	void DrawWithFog();

	void CloseDrawWithFog();

    void CloseDrawWithShape();

    void drawObject(int id_obj);

    void setMousePosition(int x, int y) { mouseX = x; mouseY = y; };
    void setDrawMode(int mode);
    void setWindowSize(int width, int height);
    void setViewMatrix(float x, float y, float z, float pitch, float heading, float roll);
    void setModelMatrix(float x, float y, float z, float rx, float ry, float rz);

    void setCameraX(float x) { cameraPosition[0] = x; updateViewMatrix(); }
    void setCameraY(float y) { cameraPosition[1] = y; updateViewMatrix(); }
    void setCameraZ(float z) { cameraPosition[2] = z; updateViewMatrix(); }
    void setCameraAngleX(float p) { cameraAngle[0] = p; updateViewMatrix(); }
    void setCameraAngleY(float h) { cameraAngle[1] = h; updateViewMatrix(); }
    void setCameraAngleZ(float r) { cameraAngle[2] = r; updateViewMatrix(); }
    float getCameraX() { return cameraPosition[0]; }
    float getCameraY() { return cameraPosition[1]; }
    float getCameraZ() { return cameraPosition[2]; }
    float getCameraAngleX() { return cameraAngle[0]; }
    float getCameraAngleY() { return cameraAngle[1]; }
    float getCameraAngleZ() { return cameraAngle[2]; }

    void setMove(bool b) { move = b; }
    void setIsDraw(bool b) { isDraw = b; }
    bool getMove() { return move; }
    bool getisDraw() { return isDraw; }
    float getSizeObject() { return sizeObject; }
    int getModelObject() { return object; }
    void setModelObject(int x) { object = x; }

    void setModelX(float x) { modelPosition[0] = x; updateModelMatrix(); }
    void setModelY(float y) { modelPosition[1] = y; updateModelMatrix(); }
    void setModelZ(float z) { modelPosition[2] = z; updateModelMatrix(); }
    void setModelAngleX(float a) { modelAngle[0] = a; updateModelMatrix(); }
    void setModelAngleY(float a) { modelAngle[1] = a; updateModelMatrix(); }
    void setModelAngleZ(float a) { modelAngle[2] = a; updateModelMatrix(); }
    float getModelX() { return modelPosition[0]; }
    float getModelY() { return modelPosition[1]; }
    float getModelZ() { return modelPosition[2]; }
    float getModelAngleX() { return modelAngle[0]; }
    float getModelAngleY() { return modelAngle[1]; }
    float getModelAngleZ() { return modelAngle[2]; }

    void setBoxRotationOX(bool b) { boxRotationOX = b; }
    void setBoxRotationOY(bool b) { boxRotationOY = b; }
    void setBoxRotationOZ(bool b) { boxRotationOZ = b; }

    bool boxRotationOXIsCheck() { return boxRotationOX; }
    bool boxRotationOYIsCheck() { return boxRotationOY; }
    bool boxRotationOZIsCheck() { return boxRotationOZ; }

    void setFlagFog(bool ff) { flagFog = ff; }
    bool getFlagFog() { return flagFog; }

    void setModelShape(int id) { shape = id; }
    int getModelShape() { return shape; }

    // return 16 elements of  target matrix
    const float* getViewMatrixElements() { return matrixView.get(); }
    const float* getModelMatrixElements() { return matrixModel.get(); }
    const float* getModelViewMatrixElements() { return matrixModelView.get(); }
    const float* getProjectionMatrixElements() { return matrixProjection.get(); }

	void setSizeObject(int x);

	void setSizeObject(int x, int y);

    void rotateCamera(int x, int y);
    void zoomCamera(int dist);
    void zoomCameraDelta(float delta);  // for mousewheel
    void setX1Y1SizeObject(int x, int y) { x_first = x; y_first = y; }
    bool isShaderSupported() { return glslSupported; }
    void runTexture();
protected:

private:
    // member functions
    void initLights();                              // add a white light ti scene
    void setViewport(int x, int y, int width, int height);
    void setViewportSub(int left, int bottom, int width, int height, float nearPlane, float farPlane);
    void drawGrid(float size, float step);          // draw a grid on XZ plane
    void drawAxis(float size);                      // draw 3 axis
    void drawSub1();                                // draw upper window
    void drawSub2();                                // draw bottom window
    void drawFrustum(float fovy, float aspect, float near, float far);
    Matrix4 setFrustum(float l, float r, float b, float t, float n, float f);
    Matrix4 setFrustum(float fovy, float ratio, float n, float f);
    Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n = -1, float f = 1);
    void updateModelMatrix();
    void updateViewMatrix();
    bool createShaderPrograms();
    std::string getShaderStatus(GLuint shader);     // return GLSL compile error log
    std::string getProgramStatus(GLuint program);   // return GLSL link error log
    
    // members
    int windowWidth;
    int windowHeight;
    int povWidth;           // width for point of view screen (left)
    bool windowSizeChanged;
    bool drawModeChanged;
    int drawMode;
    int mouseX;
    int mouseY;
    float cameraPosition[3];
    float cameraAngle[3];
    float modelPosition[3];
    float modelAngle[3];
    int object;
    bool boxRotationOX = false;
    bool boxRotationOY = false;
    bool boxRotationOZ = false;
    bool flagFog;
    // these are for 3rd person view
    float cameraAngleX;
    float cameraAngleY;
    float cameraDistance;
    float bgColor[4];
    int shape;
    bool move;
    bool isDraw;
    float sizeObject = 0;
    int x_first, y_first;
    int x_last, y_last;

    // 4x4 transform matrices
    Matrix4 matrixView;
    Matrix4 matrixModel;
    Matrix4 matrixModelView;
    Matrix4 matrixProjection;

    // glsl extension
    bool glslSupported;
    bool glslReady;
    GLuint progId1;             // shader program with color
    GLuint progId2;             // shader program with color + lighting
};
#endif


