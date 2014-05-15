#ifndef LUATHREAD_H
#define LUATHREAD_H

#include <QThread>
#include <vector>
#include <string>
#include "luainterpreter.h"
#include "viewer.h"

class LuaThread : public QThread
{
    Q_OBJECT
public:
    std::string getName() const { return "LuaThread"; }
    explicit LuaThread(scripting::LuaInterpreter& luaInterpreter, Viewer& viewer, QObject *parent = 0);
    void run();
    void setArguments(std::vector<std::string> args) { m_arguments = args; }
    void execute(std::vector<std::string> args);
signals:
    
public slots:
    
private:
    scripting::LuaInterpreter& m_interpreter;
    std::vector<std::string> m_arguments;
    Viewer& m_viewer;
};

#endif // LUATHREAD_H
