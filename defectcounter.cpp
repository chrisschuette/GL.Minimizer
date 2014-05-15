#include <fstream>

#include "defectcounter.h"
#include "luainterpreter.h"
#include <cmath>
#include <assert.h>
#include "exception.h"

using namespace std;


bool DefectCounter::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(DefectCounter::Register);
void DefectCounter::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
        SpinCuboid::Register();

    if(!luaInterpreter.isClassRegistered("DefectCounter")) {
        luabind::module(luaInterpreter.getL())
                [
                luabind::class_< DefectCounter >("DefectCounter")
                .def(luabind::constructor<>())
                .def("getFluxField", &DefectCounter::getFluxField)
                .def("saoAXY", &DefectCounter::saoAXY)
                .def("saoAYZ", &DefectCounter::saoAYZ)
                .def("saoAZX", &DefectCounter::saoAZX)
                .def("saoCube", (double(DefectCounter::*)(const SpinCuboid& , int , int , int ) )&DefectCounter::saoCube)
                .def("getDefects", (luabind::object (DefectCounter::*)(const SpinCuboid&)) &DefectCounter::getDefects)
                ];
        luaInterpreter.setClassRegistered("DefectCounter");
    }
#endif
}

DefectCounter::DefectCounter() : Named("DefectCounter")
{

}

DefectCounter::~DefectCounter() {
}


double DefectCounter::angle (int a1, int a2, int a3, int b1, int b2, int b3, int c1,
                             int c2, int c3, const SpinCuboid& spins )
{
    double s0[3];
    double s1[3];
    double s2[3];

    double norm02 = 0.0;
    double norm12 = 0.0;
    double norm22 = 0.0;

    for(int pos=0; pos<3; pos++)
    {
        s0[pos] = spins.get4(a1,a2,a3,pos);
        s1[pos] = spins.get4(b1,b2,b3,pos);
        s2[pos] = spins.get4(c1,c2,c3,pos);
        norm02 += s0[pos]*s0[pos];
        norm12 += s1[pos]*s1[pos];
        norm22 += s2[pos]*s2[pos];
    }

    for(int pos=0; pos<3; pos++) {
        s0[pos] /= sqrt(norm02);
        s1[pos] /= sqrt(norm12);
        s2[pos] /= sqrt(norm22);
    }

    double spat_produkt = s0[0]*s1[1]*s2[2] + s0[2]*s1[0]*s2[1] +
            s0[1]*s1[2]*s2[0] - s0[2]*s1[1]*s2[0] - s0[0]*s1[2]*s2[1] -
            s0[1]*s1[0]*s2[2];
    double denominator = 1.0 + (s0[0]*s1[0] + s0[1]*s1[1] + s0[2]*s1[2]) +
            (s0[0]*s2[0] + s0[1]*s2[1] + s0[2]*s2[2]) + (s2[0]*s1[0] + s2[1]*s1[1] + s2[2]*s1[2]);
    double angle = 2.0 * atan2( spat_produkt , denominator );

    return angle;
}

double DefectCounter::charge(int i, int j, int k, const SpinCuboid& spins)
{
    //Define mid of cube
    double cube_mid[3];
    cube_mid[0] = double(i)+0.5;
    cube_mid[1] = double(j)+0.5;
    cube_mid[2] = double(k)+0.5;

    //Define unit vectors
    double e[3][3];
    e[0][0] = 1.0;	e[1][0] = 0.0;	e[2][0] = 0.0;
    e[0][1] = 0.0;	e[1][1] = 1.0;	e[2][1] = 0.0;
    e[0][2] = 0.0;	e[1][2] = 0.0;	e[2][2] = 1.0;

    //Berechne Ladung, a und b definieren die Flaeche e_a x e_b
    double charge = 0.0;
    for(int a=0; a<3; a++)
    {
        for(int b=0; b<3; b++)
        {
            if(a!=b)
            {
                double surf[3]; //Oberflaechenvektor
                surf[0] = e[a][1]*e[b][2] - e[a][2]*e[b][1];
                surf[1] = e[a][2]*e[b][0] - e[a][0]*e[b][2];
                surf[2] = e[a][0]*e[b][1] - e[a][1]*e[b][0];

                int coord[4][3];
                coord[0][0] = int(cube_mid[0] + 0.5*surf[0] -0.5*e[a][0] -
                        0.5*e[b][0]+0.1);
                coord[0][1] = int(cube_mid[1] + 0.5*surf[1] -0.5*e[a][1] -
                        0.5*e[b][1]+0.1);
                coord[0][2] = int(cube_mid[2] + 0.5*surf[2] -0.5*e[a][2] -
                        0.5*e[b][2]+0.1);

                coord[1][0] = int(cube_mid[0] + 0.5*surf[0] +0.5*e[a][0] -
                        0.5*e[b][0]+0.1);
                coord[1][1] = int(cube_mid[1] + 0.5*surf[1] +0.5*e[a][1] -
                        0.5*e[b][1]+0.1);
                coord[1][2] = int(cube_mid[2] + 0.5*surf[2] +0.5*e[a][2] -
                        0.5*e[b][2]+0.1);

                coord[2][0] = int(cube_mid[0] + 0.5*surf[0] +0.5*e[a][0] +
                        0.5*e[b][0]+0.1);
                coord[2][1] = int(cube_mid[1] + 0.5*surf[1] +0.5*e[a][1] +
                        0.5*e[b][1]+0.1);
                coord[2][2] = int(cube_mid[2] + 0.5*surf[2] +0.5*e[a][2] +
                        0.5*e[b][2]+0.1);

                coord[3][0] = int(cube_mid[0] + 0.5*surf[0] -0.5*e[a][0] +
                        0.5*e[b][0]+0.1);
                coord[3][1] = int(cube_mid[1] + 0.5*surf[1] -0.5*e[a][1] +
                        0.5*e[b][1]+0.1);
                coord[3][2] = int(cube_mid[2] + 0.5*surf[2] -0.5*e[a][2] +
                        0.5*e[b][2]+0.1);

                //Pruefen, dass kein 0-Spin dabei ist
                for(int q=0; q<4; q++)
                {
                    if(
                            scalarproduct(coord[q][0],coord[q][1],coord[q][2],coord[q][0],coord[q][1],coord[q][2],spins)
                            < 0.001 )
                    {
                        return 0.0;
                    }
                }


                charge +=
                        angle(coord[0][0],coord[0][1],coord[0][2],coord[1][0],coord[1][1],coord[1][2],coord[3][0],coord[3][1],coord[3][2],spins)
                        +
                        angle(coord[1][0],coord[1][1],coord[1][2],coord[2][0],coord[2][1],coord[2][2],coord[3][0],coord[3][1],coord[3][2],spins);

            }
        }
    }

    return charge/(4.0*M_PI);
}

#ifndef LUADISABLED
luabind::object DefectCounter::getDefects(const SpinCuboid& spins) {
    luabind::object defects = luabind::newtable( scripting::LuaInterpreter::getInstance().getL() );

    double ch;
    int defectCounter = 0;

    for(int x = 0; x < spins.getDimension(0)-1; x++)
        for(int y = 0; y < spins.getDimension(1)-1; y++)
            for(int z = 0; z < spins.getDimension(2)-1; z++) {

                //ch = saoCube(spins,x,y,z) / (4.0 * M_PI);
                ch = charge(x,y,z,spins);
                if(abs(ch) > 0.001) {
                    defectCounter++;
                    luabind::object defect = luabind::newtable( scripting::LuaInterpreter::getInstance().getL() );
                    luabind::object defectPosition = luabind::newtable( scripting::LuaInterpreter::getInstance().getL() );
                    defectPosition["x"] = x;
                    defectPosition["y"] = y;
                    defectPosition["z"] = z;
                    defect["position"] = defectPosition;
                    defect["charge"] = ch;
                    defects[defectCounter] = defect;

                }
            }
    return defects;
}
#endif

double DefectCounter::scalarproduct(int a1, int a2, int a3, int b1, int b2, int b3, const SpinCuboid& spins) {
    double s0[3];
    double s1[3];
    double norm02 = 0.0;
    double norm12 = 0.0;
    for(int pos=0; pos<3; pos++)
    {
        s0[pos] = spins.get4(a1,a2,a3,pos);
        s1[pos] = spins.get4(b1,b2,b3,pos);
        norm02 += s0[pos]*s0[pos];
        norm12 += s1[pos]*s1[pos];
    }
    return (s0[0]*s1[0] + s0[1]*s1[1] + s0[2]*s1[2])/(sqrt(norm02 * norm12));
}

double DefectCounter::sa(const SpinCuboid& spins, int * base, int coord1, int d1, int coord2, int d2) {
    double n1[3];
    n1[0] = spins.get4(base[0], base[1], base[2], 0);
    n1[1] = spins.get4(base[0], base[1], base[2], 1);
    n1[2] = spins.get4(base[0], base[1], base[2], 2);
    double norm = std::sqrt(n1[0]*n1[0]+n1[1]*n1[1]+n1[2]*n1[2]);
    n1[0] /= norm;
    n1[1] /= norm;
    n1[2] /= norm;

    base[coord1] = base[coord1] + d1;

    double n2[3];
    n2[0] = spins.get4(base[0], base[1], base[2], 0);
    n2[1] = spins.get4(base[0], base[1], base[2], 1);
    n2[2] = spins.get4(base[0], base[1], base[2], 2);
    norm = std::sqrt(n2[0]*n2[0]+n2[1]*n2[1]+n2[2]*n2[2]);
    n2[0] /= norm;
    n2[1] /= norm;
    n2[2] /= norm;

    base[coord1] = base[coord1] - d1;
    base[coord2] = base[coord2] + d2;

    double n3[3];
    n3[0] = spins.get4(base[0], base[1], base[2], 0);
    n3[1] = spins.get4(base[0], base[1], base[2], 1);
    n3[2] = spins.get4(base[0], base[1], base[2], 2);
    norm = std::sqrt(n3[0]*n3[0]+n3[1]*n3[1]+n3[2]*n3[2]);
    n3[0] /= norm;
    n3[1] /= norm;
    n3[2] /= norm;

    base[coord2] = base[coord2] - d2;

    double zaehler = n1[0] * ( n2[1]*n3[2] - n2[2]*n3[1] )
            + n1[1] * ( n2[2]*n3[0] - n2[0]*n3[2] )
            + n1[2] * ( n2[0]*n3[1] - n2[1]*n3[0] );
    double nenner = 1.0 + (n1[0]*n2[0] + n1[1]*n2[1] + n1[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n1[0]*n3[0] + n1[1]*n3[1] + n1[2]*n3[2]);
    assert(nenner != 0);
    return 2.0 * atan2(zaehler,nenner);
}

double DefectCounter::saoAXY(const SpinCuboid& spins, int x, int y, int z) {
    {

        int i = x;
        int j = y;
        int k = z;
        //Define mid of cube
        double cube_mid[3];
        cube_mid[0] = double(i)+0.5;
        cube_mid[1] = double(j)+0.5;
        cube_mid[2] = double(k)+0.5;

        //Define unit vectors
        double e[3][3];
        e[0][0] = 1.0;	e[1][0] = 0.0;	e[2][0] = 0.0;
        e[0][1] = 0.0;	e[1][1] = 1.0;	e[2][1] = 0.0;
        e[0][2] = 0.0;	e[1][2] = 0.0;	e[2][2] = 1.0;

        //Berechne Ladung, a und b definieren die Flaeche e_a x e_b
        double charge = 0.0;
        int a = 1;
        int b = 0;
        if(a!=b)
        {
            double surf[3]; //Oberflaechenvektor
            surf[0] = e[a][1]*e[b][2] - e[a][2]*e[b][1];
            surf[1] = e[a][2]*e[b][0] - e[a][0]*e[b][2];
            surf[2] = e[a][0]*e[b][1] - e[a][1]*e[b][0];

            int coord[4][3];
            coord[0][0] = int(cube_mid[0] + 0.5*surf[0] -0.5*e[a][0] -
                    0.5*e[b][0]+0.1);
            coord[0][1] = int(cube_mid[1] + 0.5*surf[1] -0.5*e[a][1] -
                    0.5*e[b][1]+0.1);
            coord[0][2] = int(cube_mid[2] + 0.5*surf[2] -0.5*e[a][2] -
                    0.5*e[b][2]+0.1);

            coord[1][0] = int(cube_mid[0] + 0.5*surf[0] +0.5*e[a][0] -
                    0.5*e[b][0]+0.1);
            coord[1][1] = int(cube_mid[1] + 0.5*surf[1] +0.5*e[a][1] -
                    0.5*e[b][1]+0.1);
            coord[1][2] = int(cube_mid[2] + 0.5*surf[2] +0.5*e[a][2] -
                    0.5*e[b][2]+0.1);

            coord[2][0] = int(cube_mid[0] + 0.5*surf[0] +0.5*e[a][0] +
                    0.5*e[b][0]+0.1);
            coord[2][1] = int(cube_mid[1] + 0.5*surf[1] +0.5*e[a][1] +
                    0.5*e[b][1]+0.1);
            coord[2][2] = int(cube_mid[2] + 0.5*surf[2] +0.5*e[a][2] +
                    0.5*e[b][2]+0.1);

            coord[3][0] = int(cube_mid[0] + 0.5*surf[0] -0.5*e[a][0] +
                    0.5*e[b][0]+0.1);
            coord[3][1] = int(cube_mid[1] + 0.5*surf[1] -0.5*e[a][1] +
                    0.5*e[b][1]+0.1);
            coord[3][2] = int(cube_mid[2] + 0.5*surf[2] -0.5*e[a][2] +
                    0.5*e[b][2]+0.1);

            //Pruefen, dass kein 0-Spin dabei ist
            for(int q=0; q<4; q++)
            {
                if(
                        scalarproduct(coord[q][0],coord[q][1],coord[q][2],coord[q][0],coord[q][1],coord[q][2],spins)
                        < 0.001 )
                {
                    return 0.0;
                }
            }


            charge +=
                    angle(coord[0][0],coord[0][1],coord[0][2],coord[1][0],coord[1][1],coord[1][2],coord[3][0],coord[3][1],coord[3][2],spins)
                    +
                    angle(coord[1][0],coord[1][1],coord[1][2],coord[2][0],coord[2][1],coord[2][2],coord[3][0],coord[3][1],coord[3][2],spins);

        }


        return charge;
    }

    /*
    double sa = 0;
    double n1[3];
    n1[0] = spins.get4(x, y, z, 0);
    n1[1] = spins.get4(x, y, z, 1);
    n1[2] = spins.get4(x, y, z, 2);
    double norm = std::sqrt(n1[0]*n1[0]+n1[1]*n1[1]+n1[2]*n1[2]);
    n1[0] /= norm;
    n1[1] /= norm;
    n1[2] /= norm;

    double n2[3];
    n2[0] = spins.get4(x+1, y, z, 0);
    n2[1] = spins.get4(x+1, y, z, 1);
    n2[2] = spins.get4(x+1, y, z, 2);
    norm = std::sqrt(n2[0]*n2[0]+n2[1]*n2[1]+n2[2]*n2[2]);
    n2[0] /= norm;
    n2[1] /= norm;
    n2[2] /= norm;

    double n3[3];
    n3[0] = spins.get4(x, y+1, z, 0);
    n3[1] = spins.get4(x, y+1, z, 1);
    n3[2] = spins.get4(x, y+1, z, 2);
    norm = std::sqrt(n3[0]*n3[0]+n3[1]*n3[1]+n3[2]*n3[2]);
    n3[0] /= norm;
    n3[1] /= norm;
    n3[2] /= norm;

    double n4[3];
    n4[0] = spins.get4(x+1, y+1, z, 0);
    n4[1] = spins.get4(x+1, y+1, z, 1);
    n4[2] = spins.get4(x+1, y+1, z, 2);
    norm = std::sqrt(n4[0]*n4[0]+n4[1]*n4[1]+n4[2]*n4[2]);
    n4[0] /= norm;
    n4[1] /= norm;
    n4[2] /= norm;

    double zaehler = n1[0] * ( n2[1]*n3[2] - n2[2]*n3[1] )
            + n1[1] * ( n2[2]*n3[0] - n2[0]*n3[2] )
            + n1[2] * ( n2[0]*n3[1] - n2[1]*n3[0] );
    double nenner = 1.0 + (n1[0]*n2[0] + n1[1]*n2[1] + n1[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n1[0]*n3[0] + n1[1]*n3[1] + n1[2]*n3[2]);
    assert(nenner != 0);
    sa += 2.0 * atan2(zaehler, nenner);

    zaehler = n2[0] * (n4[1] * n3[2] - n4[2]*n3[1])
            + n2[1] * (n4[2] * n3[0] - n4[0]*n3[2])
            + n2[2] * (n4[0] * n3[1] - n4[1]*n3[0]);
    nenner = 1.0 + (n4[0]*n2[0] + n4[1]*n2[1] + n4[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n4[0]*n3[0] + n4[1]*n3[1] + n4[2]*n3[2]);
    sa += 2.0 * atan2(zaehler,nenner);
    return sa;*/
}

double DefectCounter::saoAYZ(const SpinCuboid& spins, int x, int y, int z) {

    double sa = 0;
    double n1[3];
    n1[0] = spins.get4(x, y, z, 0);
    n1[1] = spins.get4(x, y, z, 1);
    n1[2] = spins.get4(x, y, z, 2);
    double norm = std::sqrt(n1[0]*n1[0]+n1[1]*n1[1]+n1[2]*n1[2]);
    n1[0] /= norm;
    n1[1] /= norm;
    n1[2] /= norm;

    double n2[3];
    n2[0] = spins.get4(x, y+1, z, 0);
    n2[1] = spins.get4(x, y+1, z, 1);
    n2[2] = spins.get4(x, y+1, z, 2);
    norm = std::sqrt(n2[0]*n2[0]+n2[1]*n2[1]+n2[2]*n2[2]);
    n2[0] /= norm;
    n2[1] /= norm;
    n2[2] /= norm;

    double n3[3];
    n3[0] = spins.get4(x, y, z+1, 0);
    n3[1] = spins.get4(x, y, z+1, 1);
    n3[2] = spins.get4(x, y, z+1, 2);
    norm = std::sqrt(n3[0]*n3[0]+n3[1]*n3[1]+n3[2]*n3[2]);
    n3[0] /= norm;
    n3[1] /= norm;
    n3[2] /= norm;

    double n4[3];
    n4[0] = spins.get4(x, y+1, z+1, 0);
    n4[1] = spins.get4(x, y+1, z+1, 1);
    n4[2] = spins.get4(x, y+1, z+1, 2);
    norm = std::sqrt(n4[0]*n4[0]+n4[1]*n4[1]+n4[2]*n4[2]);
    n4[0] /= norm;
    n4[1] /= norm;
    n4[2] /= norm;

    double zaehler = n1[0] * ( n2[1]*n3[2] - n2[2]*n3[1] )
            + n1[1] * ( n2[2]*n3[0] - n2[0]*n3[2] )
            + n1[2] * ( n2[0]*n3[1] - n2[1]*n3[0] );
    double nenner = 1.0 + (n1[0]*n2[0] + n1[1]*n2[1] + n1[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n1[0]*n3[0] + n1[1]*n3[1] + n1[2]*n3[2]);
    assert(nenner != 0);
    sa += 2.0 * atan2(zaehler, nenner);

    zaehler = n4[0] * ( n2[1]*n3[2] - n2[2]*n3[1] )
            + n4[1] * ( n2[2]*n3[0] - n2[0]*n3[2] )
            + n4[2] * ( n2[0]*n3[1] - n2[1]*n3[0] );
    nenner = 1.0 + (n4[0]*n2[0] + n4[1]*n2[1] + n4[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n4[0]*n3[0] + n4[1]*n3[1] + n4[2]*n3[2]);
    assert(nenner != 0);
    sa -= 2.0 * atan2(zaehler,nenner);
    return sa;
}

double DefectCounter::saoAZX(const SpinCuboid& spins, int x, int y, int z) {
    double sa = 0;
    double n1[3];
    n1[0] = spins.get4(x, y, z, 0);
    n1[1] = spins.get4(x, y, z, 1);
    n1[2] = spins.get4(x, y, z, 2);
    double norm = std::sqrt(n1[0]*n1[0]+n1[1]*n1[1]+n1[2]*n1[2]);
    n1[0] /= norm;
    n1[1] /= norm;
    n1[2] /= norm;

    double n2[3];
    n2[0] = spins.get4(x, y, z+1, 0);
    n2[1] = spins.get4(x, y, z+1, 1);
    n2[2] = spins.get4(x, y, z+1, 2);
    norm = std::sqrt(n2[0]*n2[0]+n2[1]*n2[1]+n2[2]*n2[2]);
    n2[0] /= norm;
    n2[1] /= norm;
    n2[2] /= norm;

    double n3[3];
    n3[0] = spins.get4(x+1, y, z, 0);
    n3[1] = spins.get4(x+1, y, z, 1);
    n3[2] = spins.get4(x+1, y, z, 2);
    norm = std::sqrt(n3[0]*n3[0]+n3[1]*n3[1]+n3[2]*n3[2]);
    n3[0] /= norm;
    n3[1] /= norm;
    n3[2] /= norm;

    double n4[3];
    n4[0] = spins.get4(x+1, y, z+1, 0);
    n4[1] = spins.get4(x+1, y, z+1, 1);
    n4[2] = spins.get4(x+1, y, z+1, 2);
    norm = std::sqrt(n4[0]*n4[0]+n4[1]*n4[1]+n4[2]*n4[2]);
    n4[0] /= norm;
    n4[1] /= norm;
    n4[2] /= norm;

    double zaehler = n1[0] * ( n2[1]*n3[2] - n2[2]*n3[1] )
            + n1[1] * ( n2[2]*n3[0] - n2[0]*n3[2] )
            + n1[2] * ( n2[0]*n3[1] - n2[1]*n3[0] );
    double nenner = 1.0 + (n1[0]*n2[0] + n1[1]*n2[1] + n1[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n1[0]*n3[0] + n1[1]*n3[1] + n1[2]*n3[2]);
    assert(nenner != 0);
    sa += 2.0 * atan2(zaehler, nenner);

    zaehler = n4[0] * ( n2[1]*n3[2] - n2[2]*n3[1] )
            + n4[1] * ( n2[2]*n3[0] - n2[0]*n3[2] )
            + n4[2] * ( n2[0]*n3[1] - n2[1]*n3[0] );
    nenner = 1.0 + (n4[0]*n2[0] + n4[1]*n2[1] + n4[2]*n2[2])
            + (n3[0]*n2[0] + n3[1]*n2[1] + n3[2]*n2[2])
            + (n4[0]*n3[0] + n4[1]*n3[1] + n4[2]*n3[2]);
    assert(nenner != 0);
    sa -= 2.0 * atan2(zaehler,nenner);
    return sa;
}


double DefectCounter::saoA(const SpinCuboid& spins, int * base, int coord1, int coord2) {
    double solidAngle = sa(spins, base, coord1, 1, coord2, 1);
    std::cout << "delta+ : " << solidAngle << std::endl;
    base[coord1]++;
    base[coord2]++;
    double temp = sa(spins, base, coord1, -1, coord2, -1);
    std::cout << "delta- : " << temp << std::endl;
    base[coord1]--;
    base[coord2]--;
    return temp + solidAngle;
}

double DefectCounter::saoCube(const SpinCuboid& spins, int x, int y, int z) {
    return saoAXY(spins, x, y, z+1)
            + saoAZX(spins, x, y+1, z)
            + saoAYZ(spins, x+1, y, z)
            - saoAZX(spins, x, y, z)
            - saoAXY(spins, x, y, z)
            - saoAYZ(spins, x, y, z);
}


double DefectCounter::saoCube(const SpinCuboid& spins, int * base) {
    return saoAXY(spins, base[0], base[1], base[2]+1)
            + saoAZX(spins, base[0], base[1]+1, base[2])
            + saoAYZ(spins, base[0]+1, base[1], base[2])
            - saoAZX(spins, base[0], base[1], base[2])
            - saoAXY(spins, base[0], base[1], base[2])
            - saoAYZ(spins, base[0], base[1], base[2]);
}

SpinCuboid* DefectCounter::getFluxField(const SpinCuboid &spins) {
    SpinCuboid* fluxField = static_cast<SpinCuboid*>(spins.clone());
    for(int z = 0; z < fluxField->getDimension(2); z++) {
        double totalZ = 0;
        for(int x = 0; x < fluxField->getDimension(0); x++)
            for(int y = 0; y < fluxField->getDimension(1); y++) {
                double fx = 0;//saoAYZ(spins, x,y,z);
                double fy = 0;//saoAZX(spins, x,y,z);
                double fz = saoAXY(spins, x,y,z) * fluxField->getDimension(0) * fluxField->getDimension(1) / (10 * 4 * M_PI);
                fluxField->set4(x,y,z,0,fx);
                fluxField->set4(x,y,z,1,fy);
                fluxField->set4(x,y,z,2,fz);
                totalZ += saoAXY(spins, x,y,z);
            }
        std::cout << "z = " << z << ": " << totalZ / (4 * M_PI) << std::endl;
    }
    return fluxField;
}

