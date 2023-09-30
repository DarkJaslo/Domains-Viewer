#include <QApplication>
#include <QSurfaceFormat>

#include "MyForm.h"
#include <iostream>

int main (int argc, char **argv) 
{
  QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
  QApplication a( argc, argv);
  QSurfaceFormat f;
  f.setVersion(3,3);
  f.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(f);

  MyForm myf;
  myf.show();

  return a.exec ();
}

