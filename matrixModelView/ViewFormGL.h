#ifndef VIEW_FORM_GL_H
#define VIEW_FORM_GL_H

#include <windows.h>
#include "Controls.h"
#include "ModelGL.h"
#include "resource.h"
namespace Win
{
    class ViewFormGL
    {
    public:
        ViewFormGL(ModelGL* model);
        ~ViewFormGL();

        void check_Box(int id);

        void uncheck_Box(int id);

        void clearObjectRotation();

        bool boxRotationOXIsCheck();

        bool boxRotationOYIsCheck();

        bool boxRotationOZIsCheck();

        void uncheckObject();

        void setViewObject(int id_obj);

        void setViewShape(int id_shape);

        void setViewFog();

        void initControls(HWND handle);         // init all controls
        void updateTrackbars(HWND handle, int position);
        void setViewMatrix(float x, float y, float z, float p, float h, float r);
        void setModelMatrix(float x, float y, float z, float rx, float ry, float rz);
        void updateMatrices();
    protected:

    private:
        ModelGL* model;
        HWND parentHandle;
        
        // controls for view section (camera)
        Win::Button   buttonResetView;
        Win::TextBox  textViewPosX;
        Win::TextBox  textViewPosY;
        Win::TextBox  textViewPosZ;
        Win::TextBox  textViewRotX;
        Win::TextBox  textViewRotY;
        Win::TextBox  textViewRotZ;
        Win::Trackbar sliderViewPosX;
        Win::Trackbar sliderViewPosY;
        Win::Trackbar sliderViewPosZ;
        Win::Trackbar sliderViewRotX;
        Win::Trackbar sliderViewRotY;
        Win::Trackbar sliderViewRotZ;
        Win::TextBox  textViewGL;

        // controls for model section
        Win::Button   buttonResetModel;
        Win::TextBox  textModelPosX;
        Win::TextBox  textModelPosY;
        Win::TextBox  textModelPosZ;
        Win::TextBox  textModelRotX;
        Win::TextBox  textModelRotY;
        Win::TextBox  textModelRotZ;
        Win::Trackbar sliderModelPosX;
        Win::Trackbar sliderModelPosY;
        Win::Trackbar sliderModelPosZ;
        Win::Trackbar sliderModelRotX;
        Win::Trackbar sliderModelRotY;
        Win::Trackbar sliderModelRotZ;
        Win::TextBox  textModelGL;

        Win::Button buttonAbout;

        Win::RadioButton Radio_Object_Teapot;
        Win::RadioButton Radio_Object_Cube;
        Win::RadioButton Radio_Object_Torus;
        Win::RadioButton Radio_Object_Sphere;
        Win::RadioButton Radio_OBject_Cylinder;
        Win::RadioButton Radio_Object_Wheel;
        Win::RadioButton Radio_Object_Cone;
        Win::RadioButton Radio_Texture;

        Win::CheckBox CheckBox_Object_X;
        Win::CheckBox CheckBox_Object_Y;
        Win::CheckBox CheckBox_Object_Z;

        Win::RadioButton radio_Shape_Point;
        Win::RadioButton radio_Shape_Line;
        Win::RadioButton radio_Shape_Face;
        Win::RadioButton radio_Fog;
        // matrices
        Win::TextBox   mv[16];          // view matrix
        Win::TextBox   mm[16];          // model matrix
        Win::TextBox   mmv[16];         // modelview matrix
    };
}

#endif
