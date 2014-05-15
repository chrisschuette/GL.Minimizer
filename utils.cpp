
#include "utils.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

namespace dos {

double rho3D_m(double eps) {
    return 0.0746756967363403 + (8675.0 * eps * eps) / 6283008;
}

double rho3D_n(double eps) {
    return -0.10744121287128713 - (51.0 * eps) / 25856.0 - (81.0 * eps * eps) / 51712.0;
}

double rho3D(double omega) {
    double dos = rho3D_m(omega) * sqrt(36 - omega * omega);
    if(omega < -6.0)
      return 0.0;
    if(omega > 6.0)
      return 0.0;
    if (omega < -2.0)
        dos += rho3D_n(-omega) * sqrt(4.0 - (omega + 4.0) * (omega + 4.0));
    if (omega > 2.0)
        dos += rho3D_n(omega) * sqrt(4.0 - (omega - 4.0) * (omega - 4.0));
    return dos / M_PI;
}

double rho2D(double eps) {
//    return 0.14282921950711333 + (-0.051299342112440104 + 0.0003538996863908572 * eps * eps) * log(fabs(eps));
	return 0.1408652818140526 + 0.0009352207587808881*eps*eps - 0.000025063028633741483*pow(eps,4) - log(eps*eps)/(4.*M_PI*M_PI);
}

double rho2Dxx(double eps) {
		return 0.4053206358913109 - 0.04161446460559167*pow(eps,2) - \
		   0.00008077647962742948*pow(eps,4) + 9.596446188475877e-7*pow(eps,6) + \
		   0.00625*pow(eps,2)*log(pow(eps,2));
}

}

namespace math {
    double heaviside(double argument) {
      double result = 0.0;
      if(argument > 0.0)
	result = 1.0;
      return result;
  }
}

//
namespace utils {
 
std::vector<std::string> getPathHierarchy(std::string filename) {
	std::string temp = filename;
	std::vector<std::string> paths;

	while(true) {
	std::string::size_type begin = temp.find_last_of("//");
	if (begin == std::string::npos) {
		//paths.push_back(temp);
	    return paths;
	}
	else {
		temp = temp.substr(0,begin);
		paths.insert(paths.begin(),temp);
	}
	}
	return paths;
}


std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

double nF(double beta, double omega) {
	return 1.0 / ( 1.0 + exp(beta * omega) );
}

double dnF(double beta, double omega) {
	return beta / ( exp(-beta * omega) + 2.0 + exp(beta * omega) );
}



}


