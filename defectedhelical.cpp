#include "defectedhelical.h"
#include "luainterpreter.h"

using namespace Eigen;

bool DefectedHelical::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(DefectedHelical::Register);
void DefectedHelical::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Phase"))
       Phase::Register();

    if(!luaInterpreter.isClassRegistered("DefectedHelical")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< DefectedHelical, luabind::bases<Phase> >("DefectedHelical")
                .def(luabind::constructor<>())
                .def("setWidth", &DefectedHelical::setWidth)
        ];
        luaInterpreter.setClassRegistered("DefectedHelical");
    }
#endif
}

DefectedHelical::DefectedHelical() : Phase(), m_width(3.0)
{
}

//Printable
std::string DefectedHelical::toString() const {
    return std::string();
}

void DefectedHelical::render(SpinCuboid &array) const {
    int Nx = array.getDimension(0);
    int Ny = array.getDimension(1);
    int Nz = array.getDimension(2);

    int centerX = Nx/2;
    int centerY = Ny/2;
    int centerZ = Nz/2;

    Vector3d Q(m_qx,m_qy,m_qz);

    for( int x = 0; x < Nx; x++ ) {
            for( int y = 0; y < Ny; y++) {
                for( int z = 0; z < Nz; z++) {
                    Vector3d s = defectConical(m_width, x-centerX,y-centerY,z-centerZ,Q);
                    array.set4(x,y,z,0,s[0]);
                    array.set4(x,y,z,1,s[1]);
                    array.set4(x,y,z,2,s[2]);
                }
            }
        }
}

Vector3d DefectedHelical::defectConical0(double width, int x, int y, Vector3d Q) const {
    Vector3d r = Vector3d(x,y,0);
    double perp = Q.cross(r).dot(Vector3d::UnitZ()) / Q.norm();
  //  if(perp > 1.0) {
 //       perp -= 1.0;
        Vector3d v = n( M_PI/2.0 - sigmoid(perp, width), -sign(perp) * (Q.dot(r) - M_PI ));
        return AngleAxisd(atan2(Q[1],Q[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0+sigmoid(perp,width),  Vector3d::UnitY() ) * v;
   // } else if(perp < -1.0) {
       // perp += 1.0;
     //   Vector3d v = n( M_PI/2.0 - sigmoid(perp, width), -sign(perp) * (Q.dot(r) - M_PI ));
     //   return AngleAxisd(atan2(Q[1],Q[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0+sigmoid(perp,width),  Vector3d::UnitY() ) * v;
   // } else {
   //     return -Vector3d::UnitZ();
  //  }
    //return Vector3d(0,0,sign(perp));
}

Vector3d DefectedHelical::helical(int x, int y, int z, Vector3d Q) const {
    Vector3d r = Vector3d(x,y,z);
    Vector3d v = n( M_PI/2.0, Q.dot(r) - M_PI );
    Vector3d s = AngleAxisd(atan2(Q[1],Q[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0,  Vector3d::UnitY() ) * v;
    return s;
}

Vector3d DefectedHelical::defectConical(double width, int x, int y,int z, Vector3d Q) const {
    Vector3d r = Vector3d(x,y,0);
    if( (Q.dot(r) > M_PI) || (Q.dot(r) < -M_PI))
    //if(abs(Q.dot(r)) > M_PI)
        return helical(x,y,z,Q);
    else
     //   return Vector3d(0,0,1);
        return defectConical0(width, x, y, Q);
}

double DefectedHelical::sigmoid(double arg, double width, double center) const {
    return atan((arg - center) / width) + M_PI/2.0;
}

double DefectedHelical::sign(double arg) const {
    if(arg < 0.0)
        return -1.0;
    return 1.0;
}
