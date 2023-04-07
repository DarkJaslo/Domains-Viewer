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
  initializeOpenGLFunctions();
  //glEnable(GL_DEPTH_TEST); seems to cause trouble

  //needed to actually be able to use alpha
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor (0.0, 0.0, 0.0, 1.0); // background color (black)

  loadShaders();
  createBuffersSquare();
  createBuffersPlayer();
  createBuffersDrawing();

  iniCamera();

  readBoards();
  currentRound = 0;
}

void MyGLWidget::paintGL ()
{
//  glViewport (0, 0, width, height);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Erase frame-buffer

  for(int i = 0; i < rows; ++i){
    for(int j = 0; j < cols; ++j){
      Square sq = boards[currentRound][i][j];
      glm::vec3 pos(i,j,0);
      if(sq.painter == -1){
        paintSquare(pos,white);
      }
      else{
        paintSquare(pos,glm::vec4(colors[sq.painter],0.8f));
      }

      if(sq.drawer != -1){
        paintSquare(pos,glm::vec4(colors[sq.drawer],0.4f));
      }

      if(sq.unit != -1){
        paintPlayer(pos,glm::vec4(colors[sq.unit],1.0f));
      }
      
    }
  }
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Specific MAC case. Not removing it since it could be useful.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  width = vp[2];
  height = vp[3];
#else
  width = w;
  height = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Left:{
      if(currentRound > 0) --currentRound;
      break;
    }
    case Qt::Key_Right:{
      if(currentRound < rounds-1) ++currentRound;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::readBoards()
{
  cin >> rounds >> rows >> cols;
  boards = vector<vector<vector<Square>>>(rounds,vector<vector<Square>>(rows,vector<Square>(cols)));
  for(int i = 0; i < rounds; ++i){
    int r;
    cin >> r;
    if(r != i){
      cerr << "Incorrect input round: expected " << i << " and got " << r << endl;
      exit(1); 
    }
    for(int j = 0; j < rows; ++j){
      for(int k = 0; k < cols; ++k){
        cin >> boards[i][j][k].painter >> boards[i][j][k].drawer >> boards[i][j][k].unit;
      }
    }
  }
}

void MyGLWidget::viewTransform()
{
  glm::mat4 View = glm::lookAt(OBS, VRP, UP);
  glUniformMatrix4fv(viewLoc,1,GL_FALSE,&View[0][0]);
}

void MyGLWidget::projectTransform()
{
  glm::mat4 Proj = glm::ortho(left,right,up,down,znear,zfar);
  glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::iniCamera()
{
  left = 0.0;  right = 50.0;
  up = 0.0;  down = 50.0;
  znear = 0.1;  zfar = 1.1;
  //OBS = glm::vec3(25.0,25.0,1.0);
  OBS = glm::vec3(50.0,0.0,1.0);
  //VRP = glm::vec3(25.0,25.0,0.0);
  VRP = glm::vec3(50.0,0.0,0.0);
  UP = glm::vec3(-1.0,0.0,0.0);
  viewTransform();
  projectTransform();
}

void MyGLWidget::modelTransformSquare(glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,pos);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPlayer(glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,pos);
  glUniformMatrix4fv(TGLoc,1,GL_FALSE,&TG[0][0]);
}

void MyGLWidget::paintSquare(glm::vec3 pos, glm::vec4 color)
{
  glBindVertexArray(VAOSquare);
  modelTransformSquare(pos);
  glUniform4fv(colLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES,0,6);
  glBindVertexArray(0);
}

void MyGLWidget::paintPlayer(glm::vec3 pos, glm::vec4 color)
{
  glBindVertexArray(VAOPlayer);
  modelTransformPlayer(pos);
  glUniform4fv(colLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES,0,6);
  glBindVertexArray(0);  
}

void MyGLWidget::createBuffersDrawing()
{
  glm::vec3 Vertices[6];
  Vertices[0] = glm::vec3(0.05, 0.05, 0);
  Vertices[1] = glm::vec3(0.95, 0.05, 0);
  Vertices[2] = glm::vec3(0.95, 0.95, 0);
  Vertices[3] = glm::vec3(0.95, 0.95, 0);
  Vertices[4] = glm::vec3(0.05, 0.95, 0);
  Vertices[5] = glm::vec3(0.05, 0.05, 0);

  // Creating VAO
  glGenVertexArrays(1, &VAODrawing);
  glBindVertexArray(VAODrawing);

  // Creating VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glBindVertexArray(0);
}

void MyGLWidget::createBuffersSquare()
{
  glm::vec3 Vertices[6];
  Vertices[0] = glm::vec3(0.05, 0.05, 0);
  Vertices[1] = glm::vec3(0.95, 0.05, 0);
  Vertices[2] = glm::vec3(0.95, 0.95, 0);
  Vertices[3] = glm::vec3(0.95, 0.95, 0);
  Vertices[4] = glm::vec3(0.05, 0.95, 0);
  Vertices[5] = glm::vec3(0.05, 0.05, 0);
  
  // Creating VAO
  glGenVertexArrays(1, &VAOSquare);
  glBindVertexArray(VAOSquare);

  // Creating VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glBindVertexArray(0);
}

void MyGLWidget::createBuffersPlayer(){
  glm::vec3 Vertices[3];
  Vertices[0] = glm::vec3(0.1,0.1,0.0);
  Vertices[1] = glm::vec3(0.9,0.1,0.0);
  Vertices[2] = glm::vec3(0.5,0.9,0.0);

  // Creating VAO
  glGenVertexArrays(1, &VAOPlayer);
  glBindVertexArray(VAOPlayer);

  // Creating VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glBindVertexArray(0);
}

void MyGLWidget::loadShaders()
{
  // Creating fragment and vertex shaders
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Loading and compiling of the shaders
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creation of the program
  program = new QOpenGLShaderProgram(this);
  // Adding the shaders
  program->addShader(&fs);
  program->addShader(&vs);
  // Linking the program
  program->link();
  // Specify we want to use this program
  program->bind();

  // Obtaining attibute identifier
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  
  // Obtaining uniform identifiers
  TGLoc = glGetUniformLocation(program->programId(), "TG");
  colLoc = glGetUniformLocation(program->programId(), "col");
  sclLoc = glGetUniformLocation(program->programId(), "scale");
  viewLoc = glGetUniformLocation(program->programId(), "View");
  projLoc = glGetUniformLocation(program->programId(), "Proj");
  glUniform1f(sclLoc, 1.0f);
}