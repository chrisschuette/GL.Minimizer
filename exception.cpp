/*
 * Exception.cpp
 *
 *  Created on: Feb 8, 2010
 *      Author: schuette
 */

#include "exception.h"
#include "utils.h"

using namespace std;
using namespace basic;
using namespace utils;

Exception::Exception(string message, const char* pFile, int nLine)
{
	m_errorMsg = message;
	m_line = nLine;
	m_fileName = pFile;
	//m_message = m_fileName + ", line " + toString(m_line) + ": " +  m_errorMsg;

}

Exception::Exception(std::string message, const char* pFunction , const char* pFile, int nLine) {
	m_errorMsg = message;
	m_line = nLine;
	m_fileName = pFile;
//	m_message = m_fileName + ", line " + toString(m_line) + ": " +  m_errorMsg;
        m_function = pFunction;
}


void Exception::setFunction(std::string function) {
    this->m_function = function;
}

std::string Exception::getFunction() const {
    return m_function;
}

void Exception::setLine(int line) {
    this->m_line = line;
}

int Exception::getLine() const {
    return m_line;
}

void Exception::setFileName(std::string fileName) {
    this->m_fileName = fileName;
}

std::string Exception::getFileName() const {
    return m_fileName;
}

void Exception::setErrorMsg(std::string errorMsg) {
    this->m_errorMsg = errorMsg;
}

std::string Exception::getErrorMsg() const {
    return m_errorMsg;
}

std::string Exception::getMessage() const {
    if(m_function != "")
        return m_fileName + ", function " + m_function + ", line " + toString(m_line) + ": " +  m_errorMsg;
    else
        return m_fileName + ", line " + toString(m_line) + ": " +  m_errorMsg;
}

