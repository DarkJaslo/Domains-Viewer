#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QTimer>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  public slots:
    void setRound(int r);
    void autoAdvanceAnimation();
    void toggleAnimation(bool b);

  signals:
    void init();
    void roundChanged(int r);
    void maxRounds(int m);
    void animationToggle();
    void enablePl1();
    void enablePl2();
    void enablePl3();
    void enablePl4();
    void textLabel1(const QString& s);
    void textLabel2(const QString& s);
    void textLabel3(const QString& s);
    void textLabel4(const QString& s);
    void numLabel1(int n);
    void numLabel2(int n);
    void numLabel3(int n);
    void numLabel4(int n);
    
    
  protected:
    // initializeGL - Is executed once at the beggining
    virtual void initializeGL ();

    // paintGL - Is called every time the window is updated
    virtual void paintGL ();
 
    // resize - Is called when the window's size changes
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Is called when a key is pressed
    virtual void keyPressEvent (QKeyEvent *event);

  private:

    void deencodeSq(int c, int& painter, int& drawer, bool& ability);
    void enablePl(int pl);
    void readBoards();

    void createBuffersDrawing();
    void createBuffersSquare();
    void createBuffersPlayer();
    void createBuffersBonus();
    void createBuffersBubble();
    
    void loadShaders();

    void viewTransform();
    void projectTransform();
    void iniCamera();
    
    void modelTransformSquare(glm::vec3 pos);
    void modelTransformPlayer(glm::vec3 pos);
    void paintSquare(glm::vec3 pos, glm::vec4 color);
    void paintPlayer(glm::vec3 pos, glm::vec4 color);
    void paintBonus(glm::vec3 pos, glm::vec4 color);
    void paintBubble(glm::vec3 pos, glm::vec4 color);
    
    // program
    QOpenGLShaderProgram *program;
    // attribute locations
    GLuint vertexLoc;
    // uniform locations
    GLuint TGLoc;
    GLuint colLoc;
    GLuint sclLoc;
    GLuint projLoc;
    GLuint viewLoc;

    // VAOs
    GLuint VAODrawing,VAOSquare,VAOPlayer,VAOBonus,VAOBubble;

    // viewport
    GLint width, height;

    // camera
    float ra;
    GLfloat left,right,up,down,znear,zfar;
    glm::vec3 OBS,VRP,UP; 
    
    // colors
    glm::vec3 colors[4] = {
      glm::vec3(255.0f  /255.0f,  105.0f  /255.0f,  180.0f  /255.0f),   //pink
      glm::vec3(203.0f  /255.0f,  114.0f  /255.0f,  244.0f  /255.0f),   //purple
      glm::vec3(255.0f  /255.0f,  215.0f  /255.0f,    0.0f  /255.0f),   //yellow
      glm::vec3( 44.0f  /255.0f,  237.0f  /255.0f,  236.0f  /255.0f)    //cyan
    };

    glm::vec3 abilityColors[4] = {
      glm::vec3(255.0f  /255.0f, 43.0f  /255.0f,  127.0f  /255.0f),   //pink
      glm::vec3(161.0f  /255.0f,  50.0f  /255.0f,  233.0f  /255.0f),   //purple
      glm::vec3(255.0f  /255.0f,  180.0f  /255.0f,    0.0f  /255.0f),   //yellow
      glm::vec3(  0.0f  /255.0f,  180.0f  /255.0f,  180.0f  /255.0f)    //cyan
    };

    glm::vec3 playerColors[4]{
      glm::vec3(151.0f /255.0f,   0.0f /255.0f,   58.0f  /255.0f), //darker pink
      glm::vec3(97.0f /255.0f,   0.0f /255.0f,  148.0f  /255.0f), //darker purple
      //glm::vec3(147.0f /255.0f,  99.0f /255.0f,    0.0f  /255.0f), //darker yellow
      glm::vec3(188.0f /255.0f,  86.0f /255.0f,    0.0f  /255.0f), //darker yellow
      glm::vec3(  0.0f /255.0f,  122.0f /255.0f,  123.0f  /255.0f)  //darker cyan
    };
    glm::vec4 white = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    glm::vec4 black = glm::vec4(0.0f,0.0f,0.0f,1.0f);

    //control
    int nplayers;
    bool autoAdvance;
    QTimer timer;
    int speed;
    int rounds,cols,rows;
    int currentRound;
    struct Square{
      int painter;
      int drawer;
      int unit;
      bool ability;
    };
    vector<vector<vector<Square>>> boards;
    vector<vector<int>> points;
    vector<string> names;
};
