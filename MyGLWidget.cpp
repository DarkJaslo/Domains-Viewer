#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.0, 0.0, 0.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersQuadrat();

  iniCamera();
}

void MyGLWidget::viewTransform(){
  glm::mat4 View = glm::lookAt(OBS, VRP, UP);
  glUniformMatrix4fv(viewLoc,1,GL_FALSE,&View[0][0]);
}

void MyGLWidget::projectTransform(){
  glm::mat4 Proj = glm::ortho(left,right,up,down,znear,zfar);
  glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::iniCamera(){
  left = 0.0;
  right = 50.0;
  up = 0.0;
  down = 50.0;
  znear = 0.1;
  zfar = 1.1;
  //OBS = glm::vec3(25.0,25.0,1.0);
  OBS = glm::vec3(50.0,0.0,1.0);
  //VRP = glm::vec3(25.0,25.0,0.0);
  VRP = glm::vec3(50.0,0.0,0.0);
  UP = glm::vec3(-1.0,0.0,0.0);
  viewTransform();
  projectTransform();
}

void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::pintaQuadrat(glm::vec3 posicio, glm::vec3 color){
  glBindVertexArray(VAOQuadrat);
  modelTransformQuadrat(posicio);
  glUniform3fv(colLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES,0,6);
  glBindVertexArray(0);
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
  glm::vec3 col;

  for(int i = 0; i < 50; ++i){
    for(int j = 0; j < 50; ++j){
      pintaQuadrat(glm::vec3(i*1.0,j*1.0,0.0),cols[j%4]);
      drawFilledCircle(i,j,0.4);
    }
  }
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  /*switch (event->key()) {
    case Qt::Key_S:
      if (scl < 2.0) {
        scl += 0.1;
        glUniform1f(sclLoc, scl);
      }
    	break;
    case Qt::Key_D:
      if (scl > 0.1) {
        scl -= 0.1;
        glUniform1f(sclLoc, scl);
      }
    	break;
    case Qt::Key_E:
      angle += (1.f/360.f)*3.1415f*2.f;
    	break;
    case Qt::Key_R:
      angle -= (1.f/360.f)*3.1415f*2.f;
    	break;		
    case Qt::Key_C:
      if (posBlanc != posNegre) posBlanc = posNegre;
      else posBlanc = glm::vec3(-0.125,-0.375,0.0);
    	break;
    default: event->ignore(); break;
  }*/
  update();
}

void MyGLWidget::creaBuffersQuadrat ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  /*Vertices[0] = glm::vec3(0, 0, 0);
  Vertices[1] = glm::vec3(1, 0, 0);
  Vertices[2] = glm::vec3(1, 1, 0);
  Vertices[3] = glm::vec3(1, 1, 0);
  Vertices[4] = glm::vec3(0, 1, 0);
  Vertices[5] = glm::vec3(0, 0, 0);*/

  Vertices[0] = glm::vec3(0.05, 0.05, 0);
  Vertices[1] = glm::vec3(0.95, 0.05, 0);
  Vertices[2] = glm::vec3(0.95, 0.95, 0);
  Vertices[3] = glm::vec3(0.95, 0.95, 0);
  Vertices[4] = glm::vec3(0.05, 0.95, 0);
  Vertices[5] = glm::vec3(0.05, 0.05, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOQuadrat);
  glBindVertexArray(VAOQuadrat);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersJugador(){
  glm::vec3 Vertices[3];
  Vertices[0] = glm::vec3(0.1,0.1,0.0);
  Vertices[1] = glm::vec3(0.9,0.1,0.0);
  Vertices[2] = glm::vec3(0.5,0.9,0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOjugador);
  glBindVertexArray(VAOjugador);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG");
  colLoc = glGetUniformLocation(program->programId(), "col");
  sclLoc = glGetUniformLocation(program->programId(), "scale");
  viewLoc = glGetUniformLocation(program->programId(), "View");
  projLoc = glGetUniformLocation(program->programId(), "Proj");
  glUniform1f(sclLoc, scl);
}

void MyGLWidget::drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * M_PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		    x + (radius * cos(i *  twicePi / triangleAmount)), 
			  y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}
