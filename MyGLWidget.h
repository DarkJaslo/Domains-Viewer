#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
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

    void readBoards();

    void createBuffersDrawing();
    void createBuffersSquare();
    void createBuffersPlayer();
    
    void loadShaders();

    void viewTransform();
    void projectTransform();
    void iniCamera();
    
    void modelTransformSquare(glm::vec3 pos);
    void modelTransformPlayer(glm::vec3 pos);
    void paintSquare(glm::vec3 pos, glm::vec4 color);
    void paintPlayer(glm::vec3 pos, glm::vec4 color);
    
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
    GLuint VAODrawing,VAOSquare,VAOPlayer;

    // viewport
    GLint width, height;

    // camera
    GLfloat left,right,up,down,znear,zfar;
    glm::vec3 OBS,VRP,UP; 
    
    // colors
    glm::vec3 colors[4] = {
      glm::vec3(255.0/255.0,105.0/255.0,180.0/255.0),
      glm::vec3(200.0/255.0,162.0/255.0,200.0/255.0),
      glm::vec3(255.0/255.0,215.0/255.0,0.0/255.0),
      glm::vec3(0.0/255.0,255.0/255.0,255.0/255.0)
    };
    glm::vec4 white = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    glm::vec4 black = glm::vec4(0.0f,0.0f,0.0f,1.0f);

    //control
    int rounds,cols,rows;
    int currentRound;
    struct Square{
      int painter;
      int drawer;
      int unit;
    };
    vector<vector<vector<Square>>> boards;
};
