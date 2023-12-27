#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Button.H>
#include <GL/glut.h>
#include <vector>
#include "math.h"
#include <FL/Fl_Image.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Box.h>
#include <functional>
#include <iostream>

using namespace std;
#define PI acos(-1)

int screenWidth = 512;
int screenHeight = 512;
Fl_Box* index;
Fl_BMP_Image* bmp1;
Fl_BMP_Image* bmp2;
bool currentImage = true;

class MyGlWindow : public Fl_Gl_Window {
public:
    MyGlWindow(int x, int y, int w, int h, const char* label = 0) : Fl_Gl_Window(x, y, w, h, label) {
        Rcolor = 1.0f;
        Gcolor = 0.0f;
        Bcolor = 0.0f;
        LWidth = 1.0f;
    }

    // 座標轉換
    float coordinateX(int x);
    float coordinateY(int y);

    //用枚舉的方式定義類型
    enum class ShapeType { Line, Circle, Square, clear, Eraser };
    enum class ModeType { Draw, Select, Eraser, Scale };

    void draw_Shape(ShapeType type, float x1, float y1, float x2, float y2);


    void draw() override;
    int handle(int event) override;

    struct Shape;
    bool isPointInsideShape(Shape s, float x, float y); //判斷滑鼠是否點擊在圖形上

    //設置各種模式
    void setTotalMode(ModeType mode);
    void setDrawMode(ShapeType shape);
    void setShapeColor(float Red, float Green, float Blue);
    void setLineWdith(float wdith);

    void ClearAll();    //清除所有
    void pervious();    //上一步

private:
    // 座標結構
    struct Point {
        float x;
        float y;
    };

    // 圖形結構
    struct Shape {
        ShapeType Stype; //圖形類型
        vector<struct Point> points;  //圖形座標
        float colorRed;
        float colorGreen;
        float colorBlue;
        float lineWidth;
        bool isMovable;
    };
    vector<Shape> shapes;   // shapes這個容器存放Shape結構的數據
    ModeType now_mode;      //當前的模式
    ShapeType now_shape;    //當前所畫圖案的類型

    float mouse1X;
    float mouse1Y;
    float mouse2X;
    float mouse2Y;

    float Rcolor;
    float Gcolor;
    float Bcolor;

    float LWidth;
};

//--------------------------------------轉換為標準坐標系
float MyGlWindow::coordinateX(int x) {
    return (float)x * 2 / screenWidth - 1;
}
float MyGlWindow::coordinateY(int y) {
    return -(float)y * 2 / screenHeight + 1;
}
//--------------------------------------根據種類畫出圖形
void MyGlWindow::draw_Shape(ShapeType type, float x1, float y1, float x2, float y2) {
    if (type == ShapeType::Line || type == ShapeType::Circle || type == ShapeType::Square) {
        glBegin(GL_LINE_LOOP);
        if (type == ShapeType::Line) {
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
        else if (type == ShapeType::Circle) {
            float R = sqrt((pow(x2 - x1, 2) + pow(y2 - y1, 2)));
            for (int i = 0; i < 3600; i++) {
                glVertex2f(R * cos(2 * PI * i / 3600) + x1, R * sin(2 * PI * i / 3600) + y1);
            }
        }
        else if (type == ShapeType::Square) {
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
        }
        glEnd();
    }
    if (type == ShapeType::Eraser) {
        glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
    }
}

//--------------------------------------作畫
void MyGlWindow::draw() {
    if (!valid()) {
        valid(1);
        glLoadIdentity();
        glViewport(0, 0, w(), h());
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //將shapes中的圖形畫到版面上
    for (const auto& shape : shapes) {
        glLineWidth(shape.lineWidth);
        glColor3f(shape.colorRed, shape.colorGreen, shape.colorBlue);
        draw_Shape(shape.Stype, shape.points[0].x, shape.points[0].y, shape.points[1].x, shape.points[1].y);

    }

    // 繪製OpenGL圖形
    if (now_mode == ModeType::Draw) {
        glLineWidth(LWidth);
        glColor3f(Rcolor, Gcolor, Bcolor);
        draw_Shape(now_shape, mouse1X, mouse1Y, mouse2X, mouse2Y);
    }

}

//--------------------------------------處理滑鼠事件
int MyGlWindow::handle(int event) {
    switch (event) {
    case FL_PUSH:  // 滑鼠按下事件
        if (Fl::event_button() == FL_LEFT_MOUSE) {
            mouse1X = coordinateX(Fl::event_x());
            mouse1Y = coordinateY(Fl::event_y());
            if (now_mode == ModeType::Select || now_mode == ModeType::Scale) {
                for (auto& shape : shapes) {
                    shape.isMovable = (isPointInsideShape(shape, mouse1X, mouse1Y)) ? true : false;
                }
            }
        }
        return 1;
    case FL_DRAG:
        if (Fl::event_button() == FL_LEFT_MOUSE) {
            mouse2X = coordinateX(Fl::event_x());
            mouse2Y = coordinateY(Fl::event_y());
        }
        if (now_mode == ModeType::Select) {
            for (auto& shape : shapes) {
                if (shape.isMovable) {
                    float deltaX = mouse2X - mouse1X;
                    float deltaY = mouse2Y - mouse1Y;
                    for (auto& point : shape.points) {
                        point.x += deltaX;
                        point.y += deltaY;
                    }
                    mouse1X = mouse2X;
                    mouse1Y = mouse2Y;
                }
            }
        }
        if (now_mode == ModeType::Eraser) {
            if (sqrt(pow(mouse1X - mouse2X, 2) + pow(mouse1Y - mouse2Y, 2))) {
                mouse1X = mouse2X;
                mouse1Y = mouse2Y;
                Shape squareShape = { ShapeType::Eraser, {{mouse1X - 0.05f, mouse1Y + 0.05f}, {mouse1X + 0.05f, mouse2Y - 0.05f}} ,1.0f,1.0f,1.0f,LWidth };
                shapes.push_back(squareShape);
            }

        }
        redraw();
        return 1;
    case FL_RELEASE:
        if (Fl::event_button() == FL_LEFT_MOUSE) {
            mouse2X = coordinateX(Fl::event_x());
            mouse2Y = coordinateY(Fl::event_y());

            // 將當前選擇的圖案添加到列表中
            if (now_mode == ModeType::Draw) {
                if (now_shape == ShapeType::Line) {
                    Shape lineShape = { ShapeType::Line, {{mouse1X, mouse1Y}, {mouse2X, mouse2Y}},Rcolor,Gcolor,Bcolor,LWidth };
                    shapes.push_back(lineShape);
                }
                else if (now_shape == ShapeType::Circle) {
                    Shape circleShape = { ShapeType::Circle, {{mouse1X, mouse1Y}, {mouse2X, mouse2Y}} ,Rcolor,Gcolor,Bcolor,LWidth };
                    shapes.push_back(circleShape);
                }
                else if (now_shape == ShapeType::Square) {
                    Shape squareShape = { ShapeType::Square, {{mouse1X, mouse1Y}, {mouse2X, mouse2Y}} ,Rcolor,Gcolor,Bcolor,LWidth };
                    shapes.push_back(squareShape);
                }
            }

        }
        redraw();
        return 1;
    case FL_MOUSEWHEEL:
        if (now_mode == ModeType::Scale) {
            float scaleFactor = 1.0f;
            if (Fl::event_dy() > 0) {
                // 向前滾動，增加縮放因子
                scaleFactor = 1.1f;
            }
            else if (Fl::event_dy() < 0) {
                // 向後滾動，減小縮放因子
                scaleFactor = 0.9f;
            }
            for (auto& shape : shapes) {
                if (shape.isMovable) {
                    if (shape.Stype == ShapeType::Circle) {
                        shape.points[1].x = (shape.points[1].x - shape.points[0].x) * scaleFactor + shape.points[0].x;
                        shape.points[1].y = (shape.points[1].y - shape.points[0].y) * scaleFactor + shape.points[0].y;
                    }
                    if (shape.Stype == ShapeType::Square || shape.Stype == ShapeType::Line) {
                        float centerX = (shape.points[0].x + shape.points[1].x) / 2;    // 計算圖形的中心點
                        float centerY = (shape.points[0].y + shape.points[1].y) / 2;
                        shape.points[1].x = (shape.points[1].x - centerX) * scaleFactor + centerX;
                        shape.points[1].y = (shape.points[1].y - centerY) * scaleFactor + centerY;
                        shape.points[0].x = (shape.points[0].x - centerX) * scaleFactor + centerX;
                        shape.points[0].y = (shape.points[0].y - centerY) * scaleFactor + centerY;
                    }
                }
            }
        }
        redraw();
        return 1;
    default:
        return Fl_Gl_Window::handle(event);
    }
}

//--------------------------------------判斷游標是否在圖形內部
bool MyGlWindow::isPointInsideShape(Shape s, float x, float y) {
    if (s.Stype == ShapeType::Line) {
        bool ABAPcross = ((x - s.points[0].x) * (s.points[1].x - s.points[0].x) + (y - s.points[0].y) * (s.points[1].y - s.points[0].y)) > 0;
        bool BABPcross = ((x - s.points[1].x) * (s.points[0].x - s.points[1].x) + (y - s.points[1].y) * (s.points[0].y - s.points[1].y)) > 0;
        if (ABAPcross && BABPcross) {
            float AC = ((x - s.points[0].x) * (s.points[1].x - s.points[0].x) + (y - s.points[0].y) * (s.points[1].y - s.points[0].y))
                / (sqrt(pow(s.points[1].x - s.points[0].x, 2) + pow(s.points[1].y - s.points[0].y, 2)));
            float AP_2 = ((x - s.points[0].x) * (x - s.points[0].x) + (y - s.points[0].y) * (y - s.points[0].y));
            float h = sqrt(AP_2 - AC * AC);
            return 0.03 > h;
        }
    }
    if (s.Stype == ShapeType::Circle) {
        float R = sqrt((pow(s.points[1].x - s.points[0].x, 2) + pow(s.points[1].y - s.points[0].y, 2)));
        return R > sqrt((pow(x - s.points[0].x, 2) + pow(y - s.points[0].y, 2)));
    }
    if (s.Stype == ShapeType::Square) {
        bool ab = (s.points[1].x - s.points[0].x) * (x - s.points[0].x) > 0;
        bool ad = (s.points[1].y - s.points[0].y) * (y - s.points[0].y) > 0;
        bool cb = (s.points[0].y - s.points[1].y) * (y - s.points[1].y) > 0;
        bool cd = (s.points[0].x - s.points[1].x) * (x - s.points[1].x) > 0;
        return ab && ad && cb && cd;
    }
    return false;
}

//--------------------------------------設定各種模式
void MyGlWindow::setTotalMode(ModeType mode) {
    now_mode = mode;
}
void MyGlWindow::setDrawMode(ShapeType shape) {
    now_shape = shape;
}
void MyGlWindow::setShapeColor(float Red, float Green, float Blue) {
    Rcolor = Red;
    Gcolor = Green;
    Bcolor = Blue;
}
void MyGlWindow::setLineWdith(float wdith) {
    LWidth = wdith;
}

//--------------------------------------清除所有
void MyGlWindow::ClearAll() {
    now_shape = ShapeType::clear;
    shapes.clear();
    redraw();
}

//--------------------------------------清除最後的圖形
void MyGlWindow::pervious() {
    now_shape = ShapeType::clear;
    if (shapes.size() > 0)shapes.pop_back();
    redraw();
}

//--------------------------------------切換圖片
void changeImageCallback(Fl_Widget* widget, void* data) {
    currentImage = !currentImage;
    Fl_BMP_Image* newImage = currentImage ? bmp1 : bmp2;
    index->image(newImage);
    index->redraw();
}

Fl_Button* createButton(int x, int y, int width, int height, const char* label, Fl_Widget* widget, std::function<void()> callback) {
    Fl_Button* button = new Fl_Button(x, y, width, height, label);
    button->callback([](Fl_Widget* widget, void* userData) {
        std::function<void()>* func = static_cast<std::function<void()>*>(userData);
        (*func)();
        }, new std::function<void()>(callback));
    return button;
}


int main() {
    Fl_Window* window = new Fl_Window(screenWidth * 2, screenHeight + 100, "OpenGL HW1");  //創建了一個FLTK視窗
    bmp1 = new Fl_BMP_Image("../picture/parrot.bmp");
    bmp2 = new Fl_BMP_Image("../picture/pepper.bmp");
    if (bmp1->fail() || bmp2->fail()) {
        printf("ERROR!!, BMP image load failed!");
        exit(1);
    }
    index = new Fl_Box(screenWidth, 0, 512, 512);
    index->image(bmp1);

    MyGlWindow* glWindow = new MyGlWindow(0, 0, screenWidth, screenHeight);  //創建了自定義的OpenGL視窗
    window->resizable(glWindow);

    glWindow->box(FL_FLAT_BOX);

    //--------------------------------------------------------變換圖片
    Fl_Button* button = new Fl_Button(910, screenHeight + 60, 80, 30, "Change");
    button->callback(changeImageCallback);

    //--------------------------------------------------------設定要畫什麼圖形
    Fl_Button* button1 = createButton(110, screenHeight + 10, 80, 30, "Line", glWindow, [glWindow]() {
        glWindow->setDrawMode(MyGlWindow::ShapeType::Line);
        });
    Fl_Button* button2 = createButton(210, screenHeight + 10, 80, 30, "Circle", glWindow, [glWindow]() {
        glWindow->setDrawMode(MyGlWindow::ShapeType::Circle);
        });
    Fl_Button* button3 = createButton(310, screenHeight + 10, 80, 30, "Rectangle", glWindow, [glWindow]() {
        glWindow->setDrawMode(MyGlWindow::ShapeType::Square);
        });

    //--------------------------------------------------------設定顏色
    Fl_Button* button4 = createButton(10, screenHeight + 60, 80, 30, "Red", glWindow, [glWindow]() {
        glWindow->setShapeColor(1.0f, 0.0f, 0.0f);
        });
    Fl_Button* button5 = createButton(110, screenHeight + 60, 80, 30, "Green", glWindow, [glWindow]() {
        glWindow->setShapeColor(0.0f, 1.0f, 0.0f);
        });
    Fl_Button* button6 = createButton(210, screenHeight + 60, 80, 30, "Blue", glWindow, [glWindow]() {
        glWindow->setShapeColor(0.0f, 0.0f, 1.0f);
        });
    Fl_Button* button18 = createButton(310, screenHeight + 60, 80, 30, "Yellow", glWindow, [glWindow]() {
        glWindow->setShapeColor(1.0f, 0.8f, 0.0f);
        });
    Fl_Button* button7 = createButton(410, screenHeight + 60, 80, 30, "Black", glWindow, [glWindow]() {
        glWindow->setShapeColor(0.0f, 0.0f, 0.0f);
        });


    //--------------------------------------------------------模式選擇
    Fl_Button* button8 = createButton(10, screenHeight + 10, 80, 30, "Draw", glWindow, [glWindow]() {
        glWindow->setTotalMode(MyGlWindow::ModeType::Draw);
        });


    Fl_Button* button9 = createButton(410, screenHeight + 10, 80, 30, "Select", glWindow, [glWindow]() {
        glWindow->setTotalMode(MyGlWindow::ModeType::Select);
        });
    Fl_Button* button10 = createButton(810, screenHeight + 10, 80, 30, "Clear", glWindow, [glWindow]() {
        glWindow->ClearAll();
        });
    Fl_Button* button11 = createButton(710, screenHeight + 10, 80, 30, "Eraser", glWindow, [glWindow]() {
        glWindow->setTotalMode(MyGlWindow::ModeType::Eraser);
        });
    Fl_Button* button12 = createButton(510, screenHeight + 10, 80, 30, "Scale", glWindow, [glWindow]() {
        glWindow->setTotalMode(MyGlWindow::ModeType::Scale);
        });

    //--------------------------------------------------------線段粗細
    Fl_Button* button13 = createButton(510, screenHeight + 60, 80, 30, "LineWidth1", glWindow, [glWindow]() {
        glWindow->setLineWdith(1.0f);
        });
    Fl_Button* button14 = createButton(610, screenHeight + 60, 80, 30, "LineWidth2", glWindow, [glWindow]() {
        glWindow->setLineWdith(2.0f);
        });
    Fl_Button* button15 = createButton(710, screenHeight + 60, 80, 30, "LineWidth3", glWindow, [glWindow]() {
        glWindow->setLineWdith(3.0f);
        });
    Fl_Button* button19 = createButton(810, screenHeight + 60, 80, 30, "LineWidth4", glWindow, [glWindow]() {
        glWindow->setLineWdith(4.0f);
        });

    //--------------------------------------------------------上一步
    Fl_Button* button16 = createButton(610, screenHeight + 10, 80, 30, "Previous", glWindow, [glWindow]() {
        glWindow->pervious();
        });

    //--------------------------------------------------------退出
    Fl_Button* button17 = createButton(910, screenHeight + 10, 80, 30, "Exit", glWindow, [glWindow]() {
        exit(0);
        });
    window->end();
    window->show();

    return Fl::run();
}