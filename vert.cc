#include <iostream>
#include <cmath>
using namespace std;

//prints Vertices array for a circle
void printVert(int ntr, double R, float orgX, float orgY, int start){
  double angle = 2*M_PI/ntr;
  double a = 0.0;
  for(int i = start; i < ntr+start; ++i){
    float x = orgX + R*cos(a);
    float y = orgY + R*sin(a);
    cout << "Vertices[" << 3*i << "] = glm::vec3(0.5f,0.5f,0.0f);" << endl;
    cout << "Vertices[" << 3*i+1 << "] = glm::vec3(" << x << "f," << y << "f,0.0f);" << endl;
    a+=angle;
    x = orgX + R*cos(a);
    y = orgY + R*sin(a);
    cout << "Vertices[" << 3*i+2 << "] = glm::vec3(" << x << "f," << y << "f,0.0f);" << endl;
  }
}

int main(){
  printVert(36,0.35,0.5,0.5,0);
}