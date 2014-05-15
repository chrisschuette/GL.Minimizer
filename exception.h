/*
 * Exception.h
 *
 *  Created on: Feb 8, 2010
 *      Author: schuette
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include<exception>
#include <stdexcept>

#define ERROR(a) throw basic::Exception(a, __FUNCTION__ ,__FILE__, __LINE__);



namespace basic {
    
    /*! \brief The Exception class should be able to hold all the relevant details about any type of error, which might be
 * 			encountered throughout the program flow.
 * This is a work in progress. Exception needs much more attention and is not widely used so far throughout the project.
 */
class Exception : public std::exception {
public:
	Exception(std::string message, const char* pFile, int nLine);
	Exception(std::string message, const char* pFunction , const char* pFile, int nLine);
	~Exception() throw() { };
	virtual const char* what() const throw()
	{
	return getMessage().c_str();
	}
        void setFunction(std::string function);
        std::string getFunction() const;
        void setLine(int line);
        int getLine() const;
        void setFileName(std::string fileName);
        std::string getFileName() const;
        void setErrorMsg(std::string errorMsg);
        std::string getErrorMsg() const;
        std::string getMessage() const;
protected:
	std::string m_errorMsg;
	std::string m_fileName;
	int m_line;
        std::string m_function; 
};
}
#endif /* EXCEPTION_H_ */
