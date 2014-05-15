/*
 * diag.h
 *
 *  Created on: Jan 21, 2010
 *      Author: chris
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>
#include <sstream>
#include <complex>

namespace math {
 double heaviside(double arg);
}

namespace dos {
double rho3D_m(double eps);
double rho3D_n(double eps);

double rho3D(double omega);
double rho2D(double eps);

double rho2Dxx(double eps);
}

namespace utils {

template <class T>
void swap( T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}

template <class T>
std::string toString (T a) {
	std::stringstream ss;//create a stringstream
	ss << a;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

std::string getPath(std::string filename);
std::vector<std::string> getPathHierarchy(std::string filename);

std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);
double nF(double beta, double omega);
double dnF(double beta, double omega);
}
#endif /* UTILS_H_ */
