#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/glu.h>
#include <FL/glut.h> 
#include <cmath>
#include <iostream>
#include <vector>
#include"SMFloader.h"
  
#define GL_PI 3.14159265359f  
SMFLoader loader_duck;

class MyOpenGLWindow : public Fl_Gl_Window {
public:
    MyOpenGLWindow(int x, int y, int w, int h, const char* label)
        : Fl_Gl_Window(x, y, w, h, label), xRotAngle(0.0f), yRotAngle(0.0f), verCoordinate(0.0f), distance(1.0f), Car_num(8), wheel_rot(0.0f)
        , Rotate_Speed(1.0f), flag(false) {
        spot_red = 1.0, spot_green = 1.0, spot_blue = 1.0;
        light_x = 0.0f, light_z = 0.0f;
        do_init = false;
        is_ambient_light = true;
        window = 1;  
        view[0] = 0.5;
        view[1] = -1.8;
        view[2] = 0;
        duck_color[0]=1.0;
        duck_color[1]=1.0;
        duck_color[2]=0.0;
    }
    void smf_loader(SMFLoader loader);                              //SMFloader
    void duck(float Rotate);             //畫鴨子
    void set_duck_color(float r, float g, float b);
    
    void Set_lightcolor(float r, float g, float b);                 //設置spotlight顏色    
    void Switch_ambient_light();                                    //環境燈開關   
    void ambient_light(bool on_off);                                //環境燈   
    void matrial_lightcolor(float red, float green, float blue);    //材質顏色    
    void spot_light(GLfloat x, GLfloat y, GLfloat z);               //聚光燈


    /*--------------------------------------------------畫圖的程式碼------------------------------------------------------------------*/
    GLvoid DrawCircleArea(float  cx, float  cy, float  cz, float  r, int  num_segments);    
    void  mySolidCylinder(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);  //有蓋子的圓柱    
    void DrawWheel(float x, float y, float z);                                                                          //畫輪子   
    void drawV(float Length, float Width, float x, float y, float z, float R);                                          //單邊的倒V支架   
    void draw_WheelPillar(float R, float G, float B);                                                                   //摩天輪支架
    void draw_CarPillar(float R, float G, float B, float Rotate);                                                       //繪製車廂
    void drawflat();                                                                                                    //繪製地板
    void DrawUpper(float rotationAngle);                                                                                //繪製上半部
    void DrawAll();
    /*--------------------------------------------------畫圖的程式碼------------------------------------------------------------------*/
           
    void HorRot(float Angle);               //調整水平旋轉角度   
    void VerRot(float Angle);               //調整垂直旋轉角度    
    void VerHeight(float Height);           //調整垂直高度   
    void SetDistance(float dis);            //調整遠近   
    void SetCar_num(bool plus_minus);       //調整車廂數量
    void Start_Stop();                      //摩天輪轉動開始或停止  
    void SetSpeed(GLfloat speed);           //摩天輪轉動速度   
    void rot();                             //初始化摩天輪隨著時間旋轉   
    void set_window(int window_num);        //設置要顯示哪個畫面     
    void init();                            //初始化
    void draw() override;

private:
    GLfloat xRotAngle;
    GLfloat yRotAngle;
    GLfloat verCoordinate;  
    GLfloat distance;
    GLfloat wheel_rot;
    GLfloat Rotate_Speed;
    bool flag;
    bool do_init;
    bool is_ambient_light;
    int Car_num;
    float spot_red, spot_green, spot_blue;
    float light_x, light_z;
    int window;         
    float view[3];    //相機的位置 
    float duck_color[3];
};

int main(int argc, char* argv[]) {

    loader_duck.load("rubber_duck.smf");

    Fl_Window* window = new Fl_Window(900, 600, "FLTK with OpenGL");
    MyOpenGLWindow* glWindow = new MyOpenGLWindow(10, 10, 600, 600, "OpenGL View");

    //---------------------------------------------------------小鴨視角的視窗
    Fl_Box* buttonLabe2 = new Fl_Box(650, 300, 80, 20, " Duck's View: ");
    MyOpenGLWindow* glWindow2 = new MyOpenGLWindow(650,320, 230, 230, "OpenGL View");
    glWindow2->set_window(2);
    glWindow2->Switch_ambient_light();

    //---------------------------------------------------------左右旋轉
    Fl_Value_Slider* HorSlider = new Fl_Value_Slider(750, 30, 140, 30, "HORIZONTAL VIEW");
    HorSlider->type(FL_HORIZONTAL);
    HorSlider->align(FL_ALIGN_LEFT);
    HorSlider->bounds(0, 250);
    HorSlider->value(0);
    HorSlider->callback([](Fl_Widget* widget, void* userData) {
        Fl_Value_Slider* slider = static_cast<Fl_Value_Slider*>(widget);
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->HorRot(static_cast<float>(slider->value()));
        }, glWindow);


    //---------------------------------------------------------垂直高度
    Fl_Value_Slider* VerSlider = new Fl_Value_Slider(750, 65, 140, 30, "VERTICAL VIEW");
    VerSlider->type(FL_HORIZONTAL);
    VerSlider->align(FL_ALIGN_LEFT);
    VerSlider->bounds(-1.0, 1.0);
    VerSlider->value(0);
    VerSlider->callback([](Fl_Widget* widget, void* userData) {
        Fl_Value_Slider* slider = static_cast<Fl_Value_Slider*>(widget);
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->VerHeight(static_cast<float>(slider->value()));
        }, glWindow);

    //---------------------------------------------------------上下旋轉
    Fl_Value_Slider* RotSlider = new Fl_Value_Slider(750, 100, 140, 30, "ROTATE VIEW");
    RotSlider->type(FL_HORIZONTAL);
    RotSlider->align(FL_ALIGN_LEFT);
    RotSlider->bounds(10, 45);
    RotSlider->value(10);
    RotSlider->callback([](Fl_Widget* widget, void* userData) {
        Fl_Value_Slider* slider = static_cast<Fl_Value_Slider*>(widget);
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->VerRot(static_cast<float>(slider->value()));
        }, glWindow);

    //---------------------------------------------------------距離遠近
    Fl_Value_Slider* DisSlider = new Fl_Value_Slider(750, 135, 140, 30, "DISTANCE VIEW");
    DisSlider->type(FL_HORIZONTAL);
    DisSlider->align(FL_ALIGN_LEFT);
    DisSlider->bounds(0.5, 2.0);
    DisSlider->value(1.0);
    DisSlider->callback([](Fl_Widget* widget, void* userData) {
        Fl_Value_Slider* slider = static_cast<Fl_Value_Slider*>(widget);
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->SetDistance(static_cast<float>(slider->value()));
        }, glWindow);

    //---------------------------------------------------------車廂數量增減
    Fl_Box* buttonLabel = new Fl_Box(750, 180, 40, 20, "Number of cart\n Max:12 Min:1");
    buttonLabel->align(FL_ALIGN_LEFT); // 設置文本向左對齊
    Fl_Button* button1 = new Fl_Button(760, 175, 40, 35, " - ");
    button1->callback([](Fl_Widget* widget, void* userData) {
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->SetCar_num(false);
        }, glWindow);
    Fl_Button* button2 = new Fl_Button(840, 175, 40, 35, " + ");
    button2->callback([](Fl_Widget* widget, void* userData) {
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->SetCar_num(true);
        }, glWindow);



    // 創建一個包含兩個OpenGL視窗的容器
    std::vector<MyOpenGLWindow*> windows = { glWindow, glWindow2 };
    //---------------------------------------------------------摩天輪轉動速度
    Fl_Value_Slider* SpeedSlider = new Fl_Value_Slider(750, 220, 140, 30, "ROTATE SPEED");
    SpeedSlider->type(FL_HORIZONTAL);
    SpeedSlider->align(FL_ALIGN_LEFT);
    SpeedSlider->bounds(0.5, 2.5);
    SpeedSlider->value(1.0);
    SpeedSlider->callback([](Fl_Widget* widget, void* userData) {
        Fl_Value_Slider* slider = static_cast<Fl_Value_Slider*>(widget);
        std::vector<MyOpenGLWindow*>& windowList = *static_cast<std::vector<MyOpenGLWindow*>*>(userData);
        for (MyOpenGLWindow* glWindow : windowList) {
            glWindow->SetSpeed(static_cast<float>(slider->value()));
        }
        }, &windows);

    //---------------------------------------------------------摩天輪開始/停止
    Fl_Button* button3 = new Fl_Button(650, 260, 80, 35, " Start/Stop ");
    button3->color(FL_BLUE); 
    button3->callback([](Fl_Widget* widget, void* userData) {
        std::vector<MyOpenGLWindow*>& windowList = *static_cast<std::vector<MyOpenGLWindow*>*>(userData);
        for (MyOpenGLWindow* glWindow : windowList) {
            glWindow->Start_Stop();
        }
        Fl_Button* button = static_cast<Fl_Button*>(widget);
        if (button->color() == FL_BLUE)button->color(FL_RED);
        else button->color(FL_BLUE);
        }, &windows);


    //---------------------------------------------------------退出
    Fl_Button* button4 = new Fl_Button(815, 560, 80, 35, " Quit ");
    button4->callback([](Fl_Widget* widget, void*) {
        exit(0);
        });

    //---------------------------------------------------------環境光開關
    Fl_Button* button5 = new Fl_Button(730, 260, 80, 35, " Switch ");
    button5->callback([](Fl_Widget* widget, void* userData) {
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        glWindow->Switch_ambient_light();
        }, glWindow);


    //---------------------------------------------------------投射光顏色
    Fl_Menu_Button* lightcolor_menu = new Fl_Menu_Button(810, 260, 80, 35, " Light Color ");
    lightcolor_menu->callback([](Fl_Widget* widget, void* userData) {
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        Fl_Menu_Button* lightcolor_menu = dynamic_cast<Fl_Menu_Button*>(widget);
        if (lightcolor_menu) {
            const char* selected_item = lightcolor_menu->text();
            if (strcmp(selected_item, "White") == 0) {
                glWindow->Set_lightcolor(1.0f, 1.0f, 1.0f);
            }
            else if (strcmp(selected_item, "Red") == 0) {
                glWindow->Set_lightcolor(1.0f, 0.0f, 0.0f);
            }
            else if (strcmp(selected_item, "Green") == 0) {
                glWindow->Set_lightcolor(0.0f, 1.0f, 0.0f);
            }
            else if (strcmp(selected_item, "Blue") == 0) {
                glWindow->Set_lightcolor(0.0f, 0.0f, 1.0f);
            }
            else if (strcmp(selected_item, "Yellow") == 0) {
                glWindow->Set_lightcolor(1.0f, 1.0f, 0.0f);
            }
            else if (strcmp(selected_item, "Purple") == 0) {
                glWindow->Set_lightcolor(0.4f, 0.0f, 0.6f);
            }
            else if (strcmp(selected_item, "Cyan") == 0) {
                glWindow->Set_lightcolor(0.0f, 1.0f, 1.0f);
            }
        }
        }, glWindow);
    lightcolor_menu->add("White");
    lightcolor_menu->add("Red");
    lightcolor_menu->add("Green");
    lightcolor_menu->add("Blue");
    lightcolor_menu->add("Yellow");
    lightcolor_menu->add("Purple");
    lightcolor_menu->add("Cyan");
    lightcolor_menu->value(0);

    //---------------------------------------------------------鴨子顏色
    Fl_Menu_Button* duckcolor_menu = new Fl_Menu_Button(730, 560, 80, 35, " Duck Color ");
    duckcolor_menu->callback([](Fl_Widget* widget, void* userData) {
        MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
        Fl_Menu_Button* duckcolor_menu = dynamic_cast<Fl_Menu_Button*>(widget);
        if (duckcolor_menu) {
            const char* selected_item = duckcolor_menu->text();
            if (strcmp(selected_item, "Red") == 0) {
                glWindow->set_duck_color(1.0f, 0.0f, 0.0f);
            }
            else if (strcmp(selected_item, "Green") == 0) {
                glWindow->set_duck_color(0.0f, 1.0f, 0.0f);
            }
            else if (strcmp(selected_item, "Blue") == 0) {
                glWindow->set_duck_color(0.0f, 0.0f, 1.0f);
            }
            else if (strcmp(selected_item, "Yellow") == 0) {
                glWindow->set_duck_color(1.0f, 1.0f, 0.0f);
            }
        }
        }, glWindow);
    duckcolor_menu->add("Red");
    duckcolor_menu->add("Green");
    duckcolor_menu->add("Blue");
    duckcolor_menu->add("Yellow");
    duckcolor_menu->value(0);


    window->end();
    window->resizable(glWindow);
    window->show(argc, argv);
    return Fl::run();
}

//----------------------------------------------SMFloader
void MyOpenGLWindow::smf_loader(SMFLoader loader) {
    float* vertex = NULL;
    float* color = NULL;

    for (int i = 0; i < loader.getFaceSize(); i++)	//每一迴圈畫一個三角面
    {
        // 得到一個面上的3個點  總共9個數值
        // { x1,y1,z1,x2,y2,z2,x3,y3,z3 }
        vertex = loader.getFace(i);
        // 得到一個面上的每個點的顏色 總共9個數值
        // { r1,g1,b1,r2,g2,b2,r3,g3,b3 }
        glBegin(GL_TRIANGLES);
        //glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(vertex[0], vertex[1], vertex[2]);
        glVertex3f(vertex[3], vertex[4], vertex[5]);
        glVertex3f(vertex[6], vertex[7], vertex[8]);

        glEnd();

        delete vertex, color;
    }
}

//----------------------------------------------讀取鴨子
void MyOpenGLWindow::duck(float Rotate) {
    glPushMatrix();
        glTranslated(0.3f, -3.5f, 0.0f);
        glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
        glTranslated(6.0f, 0.0f, 0.0f);
        glRotatef(-Rotate, 0.0f, 0.0f, 1.0f);
        glRotatef(90.0f, 0.0, 1.0, 0.0);
        glScaled(0.0065, 0.0065, 0.0065);
        matrial_lightcolor(duck_color[0], duck_color[1], duck_color[2]);
        smf_loader(loader_duck);
    glPopMatrix();
}

//----------------------------------------------設定鴨子的顏色
void MyOpenGLWindow::set_duck_color(float r, float g, float b) {
    duck_color[0] = r;
    duck_color[1] = g;
    duck_color[2] = b;
    redraw();
}

//----------------------------------------------設置spotlight顏色
void MyOpenGLWindow::Set_lightcolor(float r, float g, float b) {
    spot_red = r;
    spot_green = g;
    spot_blue = b;
    do_init = true;
    redraw();
}
//----------------------------------------------環境燈開關
void MyOpenGLWindow::Switch_ambient_light() {
    is_ambient_light = !is_ambient_light;
    do_init = true;
    redraw();
}

//----------------------------------------------環境燈
void MyOpenGLWindow::ambient_light(bool on_off) {
    float ambient = on_off ? 0.3f : 0.0f;
    GLfloat ambient_light_position[] = { 200.0f, 600.0f,200.0f, 1.0f };
    GLfloat ambient_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat ambient_light_diffuse[] = { ambient, ambient, ambient, 1.0f };
    GLfloat ambient_light_specular[] = { ambient, ambient, ambient, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, ambient_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ambient_light_specular);
    glEnable(GL_LIGHT0);
}

//----------------------------------------------材質顏色
void MyOpenGLWindow::matrial_lightcolor(float red, float green, float blue) {
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { red, green, blue, 0.0f };
    GLfloat specular[] = { red * 2, green * 2, blue * 2, 1.0f };
    GLfloat emission[] = { red * 0.1, green * 0.1, blue * 0.1, 1.0f };
    GLfloat shininess = 1.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

//----------------------------------------------聚光燈
void MyOpenGLWindow::spot_light(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat Spot_position[] = { x, y, z, 0.1f };    //聚光燈的位置
    GLfloat spotlight[] = { spot_red, spot_green, spot_blue, 1.0 };
    GLfloat Spot_Direction[] = { light_x , -1.0f, light_z };    //聚光燈的方向

    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Spot_Direction);
    glLightfv(GL_LIGHT1, GL_POSITION, Spot_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, spotlight);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 0.3f);
    glEnable(GL_LIGHT1);
}


/*--------------------------------------------------畫圖的程式碼------------------------------------------------------------------*/
GLvoid MyOpenGLWindow::DrawCircleArea(float  cx, float  cy, float  cz, float  r, int  num_segments)
{
    GLfloat vertex[4];

    const  GLfloat delta_angle = 2.0 * GL_PI / num_segments;
    glBegin(GL_TRIANGLE_FAN);

    vertex[0] = cx;
    vertex[1] = cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);

    //draw the vertex on the contour of the circle   
    for (int i = 0; i < num_segments; i++)
    {
        vertex[0] = std::cos(delta_angle * i) * r + cx;
        vertex[1] = std::sin(delta_angle * i) * r + cy;
        vertex[2] = cz;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
    }

    vertex[0] = 1.0 * r + cx;
    vertex[1] = 0.0 * r + cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    glEnd();
}
//有蓋子的圓柱
void  MyOpenGLWindow::mySolidCylinder(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
{
    glColor3f(84.0 / 255, 0.0, 125.0 / 255.0);
    gluCylinder(quad, base, top, height, slices, stacks);
    //top   
    DrawCircleArea(0.0, 0.0, height, top, slices);
    //base   
    DrawCircleArea(0.0, 0.0, 0.0, base, slices);
}

//畫輪子
void MyOpenGLWindow::DrawWheel(float x, float y, float z) {
    glPushMatrix();
    matrial_lightcolor(1.0f, 0.6f, 0.21f);
    glTranslatef(x, y, z);

    glutSolidTorus(0.15f, 6.0f, 40, 40);
    glutSolidTorus(0.15f, 5.2f, 40, 40);
    glutSolidTorus(0.15f, 2.0f, 40, 40);



    //畫輪子中間的支架
    for (int i = 0; i < 12; i++) {
        glPushMatrix();
        glRotatef(i * 30.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        gluCylinder(gluNewQuadric(), 0.15, 0.15, 6.0, 30, 30);
        glEnd();
        glPopMatrix();
    }

    matrial_lightcolor(0.0f, 0.0f, 1.0f);
    glutSolidTorus(0.15f, 6.5f, 40, 40);
    glPopMatrix();
}

//單邊的倒V支架
void MyOpenGLWindow::drawV(float Length, float Width, float x, float y, float z, float R) {

    glPushMatrix();
    glTranslated(x, y, z);
    glRotatef(R, 0.0f, 0.0f, 1.0f);
    glRotatef(285.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(gluNewQuadric(), Width, Width, Length, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(x, y, z);
    glRotatef(R, 0.0f, 0.0f, 1.0f);
    glRotatef(255.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(gluNewQuadric(), Width, Width, Length, 30, 30);
    glPopMatrix();

}

//摩天輪支架
void MyOpenGLWindow::draw_WheelPillar(float R, float G, float B) {
    glPushMatrix();
    matrial_lightcolor(R, G, B);
    drawV(13.0f, 0.25f, 0.0f, 0.0f, 2.0f, 0.0f);
    drawV(13.0f, 0.25f, 0.0f, 0.0f, -2.0f, 0.0f);
    glPopMatrix();
}

void MyOpenGLWindow::draw_CarPillar(float R, float G, float B, float Rotate) {
    glPushMatrix();
    matrial_lightcolor(R, G, B);
    glRotatef(Rotate, 0.0f, 0.0f, 1.0f);

    //車廂支架中間的軸
    glPushMatrix();
    glTranslatef(6.0f, 0.0f, -1.5f);
    gluCylinder(gluNewQuadric(), 0.15, 0.15, 3.0, 30, 30);
    glPopMatrix();

    drawV(4.0f, 0.15f, 6.0f, 0.0f, -1.0f, -Rotate);
    drawV(4.0f, 0.15f, 6.0f, 0.0f, 1.0f, -Rotate);
    glPopMatrix();

    //車廂的板子
    glPushMatrix();
    glTranslated(0.0f, -3.86f, 0.0f);
    glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
    glTranslated(6.0f, 0.0f, 0.0f);
    glRotatef(-Rotate, 0.0f, 0.0f, 1.0f);
    glScaled(2.2f, 0.5f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void MyOpenGLWindow::drawflat() {
    glPushMatrix();
    matrial_lightcolor(1.0f, 1.0f, 1.0f);
    glTranslated(0.0f, -12.5f, 0.0f);
    glScaled(20.0f, 0.8f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

//繪製上半部
void MyOpenGLWindow::DrawUpper(float rotationAngle) {

    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); // 繞Z軸旋轉物體
    DrawWheel(0.0f, 0.0f, 1.5f);
    DrawWheel(0.0f, 0.0f, -1.5f);
    glPopMatrix();

    for (int i = 0; i < Car_num; i++) {
        draw_CarPillar(1.0f, 1.0f, 1.0f, i * 360.0f / Car_num + rotationAngle);
    }
    glPushMatrix();
    //摩天輪轉軸
    matrial_lightcolor(1.0f, 0.6f, 0.21f);
    glTranslatef(0.0f, 0.0f, -2.5f);
    gluCylinder(gluNewQuadric(), 0.3, 0.3, 5.0, 30, 30);

    //有蓋子的轉軸
    glTranslatef(0.0f, 0.0f, -0.5f);
    mySolidCylinder(gluNewQuadric(), 0.7, 0.7, 0.5, 30, 30);
    gluCylinder(gluNewQuadric(), 0.7, 0.7, 0.5, 30, 30);
    glTranslatef(0.0f, 0.0f, 5.5f);
    mySolidCylinder(gluNewQuadric(), 0.7, 0.7, 0.5, 30, 30);
    glPopMatrix();
}

void MyOpenGLWindow::DrawAll() {
    glScaled(0.6f, 0.6f, 0.6f);
    DrawUpper(wheel_rot);
    //支架
    draw_WheelPillar(0.13f, 0.0f, 0.93f);
    drawflat();
    duck(wheel_rot);
}

/*--------------------------------------------------畫圖的程式碼------------------------------------------------------------------*/


//----------------------------------------------調整水平旋轉角度
void MyOpenGLWindow::HorRot(float Angle) {
    yRotAngle = Angle;
    redraw();
}

//----------------------------------------------調整垂直旋轉角度
void MyOpenGLWindow::VerRot(float Angle) {
    xRotAngle = -Angle;
    redraw();
}

//----------------------------------------------調整垂直高度
void MyOpenGLWindow::VerHeight(float Height) {
    verCoordinate = Height;
    redraw();
}

//----------------------------------------------調整遠近
void MyOpenGLWindow::SetDistance(float dis) {
    distance = dis;
    redraw();
}

//----------------------------------------------調整車廂數量
void MyOpenGLWindow::SetCar_num(bool plus_minus) {
    Car_num = (plus_minus) ? Car_num + 1 : Car_num - 1;
    if (Car_num > 12)Car_num = 12;
    if (Car_num < 1)Car_num = 1;
    redraw();
}

//----------------------------------------------摩天輪轉動開始或停止
void MyOpenGLWindow::Start_Stop() {
    flag = !flag;
    if (flag) {
        Fl::add_timeout(0.03, [](void* userData) {
            MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
            glWindow->rot();
            }, this);
    }
}

//----------------------------------------------摩天輪轉動速度
void MyOpenGLWindow::SetSpeed(GLfloat speed) {
    Rotate_Speed = speed;
}


//----------------------------------------------初始化摩天輪隨著時間旋轉
void MyOpenGLWindow::rot() {
    wheel_rot += Rotate_Speed;
    if (wheel_rot > 360.0f)wheel_rot = 0.0f;
    redraw();
    if (flag) {
        Fl::add_timeout(0.03, [](void* userData) {
            MyOpenGLWindow* glWindow = static_cast<MyOpenGLWindow*>(userData);
            glWindow->rot();
            }, this);
    }
}

//----------------------------------------------設置要顯示哪個畫面
void MyOpenGLWindow::set_window(int window_num) {
    window = window_num;
    redraw();
}

//----------------------------------------------初始化
void MyOpenGLWindow::init() {
    ambient_light(is_ambient_light);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    spot_light(0.0f, 60.0f, 0.0f);
}

//----------------------------------------------Draw
void MyOpenGLWindow::draw() {
    if (!valid() || do_init) {
        valid(1);
        glLoadIdentity();
        glViewport(0, 0, w(), h());
        init();
        do_init = false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (window == 2) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1, 0.001, 20);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        view[0] = 0.47 * cos(wheel_rot * 2 * GL_PI / 365);
        view[1] = 0.47 * sin(wheel_rot * 2 * GL_PI / 365);
        gluLookAt(view[0], view[1], view[2],
            view[0] + 1, view[1], view[2],
            0, 1, 0);
    }
    glPushMatrix();
        glTranslated(0.0f, verCoordinate, 0.0f);
        glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
        glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);
        glScaled(distance, distance, distance);
        glPushMatrix();
            glScaled(0.1, 0.1, 0.1);
            DrawAll();
        glPopMatrix();
    glPopMatrix();
    glFlush();
}

