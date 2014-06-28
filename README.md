GL.Minimizer
============

GL.Minimizer is a Ginzburg-Landau free energy minimizer for magnetic systems. I developed the program to find real space mean-field configurations of chiral magnetic systems. The program is however quite versatile and developed using a component-based approach: C++ classes conforming to predefined interfaces export functionality to LUA. The user controls the program flow by supplying the C++ host application with user scripts. Adding new host functionality and exporting it to LUA is simple and regularly done by our master students.

An OpenGL viewer supplies live view capabilities allowing the user to inspect the magnetic configuration in real-time while it is being optimized.

Required libraries
------------------
The following libraries are needed to successfully compile the project
* [libconfig](http://www.hyperrealm.com/libconfig/)
* [GNU Scientific Library](http://www.gnu.org/software/gsl/)
* [Boost.Program_options](http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html)
* OpenGL
* [Lua](http://www.lua.org/)
* [Luabind](http://www.rasterbar.com/products/luabind.html)

