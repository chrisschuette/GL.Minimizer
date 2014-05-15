/* 
 * File:   Configuration.h
 * Author: chris
 *
 * Created on January 2, 2012, 2:32 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include "named.h"
#include <libconfig.h++>
#include <string>
#include <list>
#include <memory>
#include <vector>
#include <utility>

namespace basic {
/*! \brief
 *
 */
class Configuration: public Named {
public:
    //Singleton
    static Configuration& getInstance();
    friend class std::auto_ptr<Configuration>;
    void readFile(std::string configFile);
    
	double getDouble(std::string path);
	bool getBool(std::string path);
	int getInteger(std::string path);
	std::string getString(std::string path);
    
    void addSetting(std::string path, std::string name, std::string value);
    void editSetting(std::string path, std::string name, double value);

    inline libconfig::Setting& lookup(const std::string path) const
   { return m_configuration.lookup(path);  }

    virtual std::string getName() const {
        return "Configuration";
    }

    static void Register();
private:
    static bool s_registered;

    //private constructor and destructor
    Configuration();
    virtual ~Configuration();
    
    static std::auto_ptr<Configuration> m_instancePtr;
     
    //config object
    libconfig::Config m_configuration;

};
}
#endif	/* CONFIGURATION_H */

