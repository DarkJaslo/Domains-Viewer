#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void creaBuffersQuadrat();
    void creaBuffersJugador();
    
    void carregaShaders();

    void viewTransform();
    void projectTransform();
    void iniCamera();
    
    void modelTransformQuadrat(glm::vec3 posicio);
    void pintaQuadrat(glm::vec3 posicio, glm::vec3 color);
    void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
    
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
    GLuint VAOQuadrat,VAOjugador;

    // viewport
    GLint ample, alt;

    // escala escena
    GLfloat scl = 1.0;
    GLfloat angle = 0.0;

    //camera
    GLfloat left,right,up,down,znear,zfar;
    glm::vec3 OBS,VRP,UP; 
    
    // colors
    glm::vec3 cols[4] = {
      glm::vec3(255.0/255.0,105.0/255.0,180.0/255.0),
      glm::vec3(200.0/255.0,162.0/255.0,200.0/255.0),
      glm::vec3(255.0/255.0,215.0/255.0,0.0/255.0),
      glm::vec3(0.0/255.0,255.0/255.0,255.0/255.0)
    };
    glm::vec3 blancTauler = glm::vec3(233.0/255.0,216.0/255.0,170.0/255.0);
    glm::vec3 negreTauler = glm::vec3(63.0/255.0,56.0/255.0,50.0/255.0);
    glm::vec3 blanc = glm::vec3(1.0);
    glm::vec3 negre = glm::vec3(0.0);    

    // posicio cavall blanc
    glm::vec3 posNegre = glm::vec3(0.125, 0.125, 0.0);
    glm::vec3 posBlanc = glm::vec3(-0.125,-0.375,0.0);
};
