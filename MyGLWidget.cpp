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
  createBuffersBonus();
  createBuffersBubble();

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
      glm::vec3 pos(i-rows/2,j-cols/2,0);
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
        if(sq.unit < 10) paintPlayer(pos,glm::vec4(colors[sq.unit],1.0f));
        else if(sq.unit >= 10 and sq.unit < 59) paintBubble(pos,glm::vec4(colors[(sq.unit-10)/10],1.0f));
        else paintBonus(pos,glm::vec4(glm::vec3(200.0f,0.0f,0.0f),1.0f));
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
  ra = float(w)/float(h);
  projectTransform();
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
  ++rounds;
  boards = vector<vector<vector<Square>>>(rounds,vector<vector<Square>>(rows,vector<Square>(cols)));
  for(int i = 0; i < rounds; ++i){
    int r;
    cin >> r;
    /*if(r != i){
      cerr << "Incorrect input round: expected " << i << " and got " << r << endl;
      exit(1); 
    }*/
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
  float l = left;
  float r = right;
  float u = up;
  float d = down;

  if(ra > 1){
    r*=ra;
    l*=ra;
  }
  if(ra < 1){
    d/=ra;
    u/=ra;
  }
  glm::mat4 Proj = glm::ortho(l,r,d,u,znear,zfar);
  glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::iniCamera()
{
  left = -25.0;  right = 25.0;
  up = -25.0;  down = 25.0;
  znear = 0.1;  zfar = 1.1;
  OBS = glm::vec3(0.0,0.0,-1.0);
  VRP = glm::vec3(0.0,0.0,0.0);
  UP = glm::vec3(1.0,0.0,0.0);
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
  glDrawArrays(GL_TRIANGLES,0,3);
  glBindVertexArray(0);  
}

void MyGLWidget::paintBonus(glm::vec3 pos, glm::vec4 color){
  glBindVertexArray(VAOBonus);
  modelTransformPlayer(pos);
  glUniform4fv(colLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES,0,18);
  glBindVertexArray(0);    
}

void MyGLWidget::paintBubble(glm::vec3 pos, glm::vec4 color){
  glBindVertexArray(VAOBubble);
  modelTransformPlayer(pos);
  glUniform4fv(colLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES,0,108);
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

void MyGLWidget::createBuffersBonus(){
  glm::vec3 Vertices[18];
  Vertices[0] =  glm::vec3(0.4,0.2,0.0);
  Vertices[1] =  glm::vec3(0.6,0.2,0.0);
  Vertices[2] =  glm::vec3(0.6,0.4,0.0);
  Vertices[3] =  glm::vec3(0.4,0.2,0.0);
  Vertices[4] =  glm::vec3(0.6,0.4,0.0);
  Vertices[5] =  glm::vec3(0.4,0.4,0.0);
  Vertices[6] =  glm::vec3(0.2,0.4,0.0);
  Vertices[7] =  glm::vec3(0.8,0.4,0.0);
  Vertices[8] =  glm::vec3(0.8,0.6,0.0);
  Vertices[9] =  glm::vec3(0.2,0.4,0.0);
  Vertices[10] = glm::vec3(0.8,0.6,0.0);
  Vertices[11] = glm::vec3(0.2,0.6,0.0);
  Vertices[12] = glm::vec3(0.4,0.6,0.0);
  Vertices[13] = glm::vec3(0.6,0.6,0.0);
  Vertices[14] = glm::vec3(0.6,0.8,0.0);
  Vertices[15] = glm::vec3(0.4,0.6,0.0);
  Vertices[16] = glm::vec3(0.6,0.8,0.0);
  Vertices[17] = glm::vec3(0.4,0.8,0.0);

  // Creating VAO
  glGenVertexArrays(1, &VAOBonus);
  glBindVertexArray(VAOBonus);

  // Creating VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glBindVertexArray(0);
}

void MyGLWidget::createBuffersBubble(){
  glm::vec3 Vertices[108];
  Vertices[0] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[1] = glm::vec3(0.9f,0.5f,0.0f);
  Vertices[2] = glm::vec3(0.893923f,0.569459f,0.0f);
  Vertices[3] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[4] = glm::vec3(0.893923f,0.569459f,0.0f);
  Vertices[5] = glm::vec3(0.875877f,0.636808f,0.0f);
  Vertices[6] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[7] = glm::vec3(0.875877f,0.636808f,0.0f);
  Vertices[8] = glm::vec3(0.84641f,0.7f,0.0f);
  Vertices[9] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[10] = glm::vec3(0.84641f,0.7f,0.0f);
  Vertices[11] = glm::vec3(0.806418f,0.757115f,0.0f);
  Vertices[12] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[13] = glm::vec3(0.806418f,0.757115f,0.0f);
  Vertices[14] = glm::vec3(0.757115f,0.806418f,0.0f);
  Vertices[15] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[16] = glm::vec3(0.757115f,0.806418f,0.0f);
  Vertices[17] = glm::vec3(0.7f,0.84641f,0.0f);
  Vertices[18] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[19] = glm::vec3(0.7f,0.84641f,0.0f);
  Vertices[20] = glm::vec3(0.636808f,0.875877f,0.0f);
  Vertices[21] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[22] = glm::vec3(0.636808f,0.875877f,0.0f);
  Vertices[23] = glm::vec3(0.569459f,0.893923f,0.0f);
  Vertices[24] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[25] = glm::vec3(0.569459f,0.893923f,0.0f);
  Vertices[26] = glm::vec3(0.5f,0.9f,0.0f);
  Vertices[27] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[28] = glm::vec3(0.5f,0.9f,0.0f);
  Vertices[29] = glm::vec3(0.430541f,0.893923f,0.0f);
  Vertices[30] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[31] = glm::vec3(0.430541f,0.893923f,0.0f);
  Vertices[32] = glm::vec3(0.363192f,0.875877f,0.0f);
  Vertices[33] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[34] = glm::vec3(0.363192f,0.875877f,0.0f);
  Vertices[35] = glm::vec3(0.3f,0.84641f,0.0f);
  Vertices[36] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[37] = glm::vec3(0.3f,0.84641f,0.0f);
  Vertices[38] = glm::vec3(0.242885f,0.806418f,0.0f);
  Vertices[39] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[40] = glm::vec3(0.242885f,0.806418f,0.0f);
  Vertices[41] = glm::vec3(0.193582f,0.757115f,0.0f);
  Vertices[42] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[43] = glm::vec3(0.193582f,0.757115f,0.0f);
  Vertices[44] = glm::vec3(0.15359f,0.7f,0.0f);
  Vertices[45] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[46] = glm::vec3(0.15359f,0.7f,0.0f);
  Vertices[47] = glm::vec3(0.124123f,0.636808f,0.0f);
  Vertices[48] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[49] = glm::vec3(0.124123f,0.636808f,0.0f);
  Vertices[50] = glm::vec3(0.106077f,0.569459f,0.0f);
  Vertices[51] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[52] = glm::vec3(0.106077f,0.569459f,0.0f);
  Vertices[53] = glm::vec3(0.1f,0.5f,0.0f);
  Vertices[54] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[55] = glm::vec3(0.1f,0.5f,0.0f);
  Vertices[56] = glm::vec3(0.106077f,0.430541f,0.0f);
  Vertices[57] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[58] = glm::vec3(0.106077f,0.430541f,0.0f);
  Vertices[59] = glm::vec3(0.124123f,0.363192f,0.0f);
  Vertices[60] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[61] = glm::vec3(0.124123f,0.363192f,0.0f);
  Vertices[62] = glm::vec3(0.15359f,0.3f,0.0f);
  Vertices[63] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[64] = glm::vec3(0.15359f,0.3f,0.0f);
  Vertices[65] = glm::vec3(0.193582f,0.242885f,0.0f);
  Vertices[66] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[67] = glm::vec3(0.193582f,0.242885f,0.0f);
  Vertices[68] = glm::vec3(0.242885f,0.193582f,0.0f);
  Vertices[69] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[70] = glm::vec3(0.242885f,0.193582f,0.0f);
  Vertices[71] = glm::vec3(0.3f,0.15359f,0.0f);
  Vertices[72] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[73] = glm::vec3(0.3f,0.15359f,0.0f);
  Vertices[74] = glm::vec3(0.363192f,0.124123f,0.0f);
  Vertices[75] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[76] = glm::vec3(0.363192f,0.124123f,0.0f);
  Vertices[77] = glm::vec3(0.430541f,0.106077f,0.0f);
  Vertices[78] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[79] = glm::vec3(0.430541f,0.106077f,0.0f);
  Vertices[80] = glm::vec3(0.5f,0.1f,0.0f);
  Vertices[81] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[82] = glm::vec3(0.5f,0.1f,0.0f);
  Vertices[83] = glm::vec3(0.569459f,0.106077f,0.0f);
  Vertices[84] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[85] = glm::vec3(0.569459f,0.106077f,0.0f);
  Vertices[86] = glm::vec3(0.636808f,0.124123f,0.0f);
  Vertices[87] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[88] = glm::vec3(0.636808f,0.124123f,0.0f);
  Vertices[89] = glm::vec3(0.7f,0.15359f,0.0f);
  Vertices[90] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[91] = glm::vec3(0.7f,0.15359f,0.0f);
  Vertices[92] = glm::vec3(0.757115f,0.193582f,0.0f);
  Vertices[93] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[94] = glm::vec3(0.757115f,0.193582f,0.0f);
  Vertices[95] = glm::vec3(0.806418f,0.242885f,0.0f);
  Vertices[96] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[97] = glm::vec3(0.806418f,0.242885f,0.0f);
  Vertices[98] = glm::vec3(0.84641f,0.3f,0.0f);
  Vertices[99] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[100] = glm::vec3(0.84641f,0.3f,0.0f);
  Vertices[101] = glm::vec3(0.875877f,0.363192f,0.0f);
  Vertices[102] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[103] = glm::vec3(0.875877f,0.363192f,0.0f);
  Vertices[104] = glm::vec3(0.893923f,0.430541f,0.0f);
  Vertices[105] = glm::vec3(0.5f,0.5f,0.0f);
  Vertices[106] = glm::vec3(0.893923f,0.430541f,0.0f);
  Vertices[107] = glm::vec3(0.9f,0.5f,0.0f);


  // Creating VAO
  glGenVertexArrays(1, &VAOBubble);
  glBindVertexArray(VAOBubble);

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