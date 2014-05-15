#include "minimizer.h"
#include "luainterpreter.h"
#include "functional.h"
#include "exception.h"
#include <sstream>

bool Minimizer::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Minimizer::Register);
void Minimizer::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
        SpinCuboid::Register();

    if(!luaInterpreter.isClassRegistered("Functional"))
        Functional::Register();

    if(!luaInterpreter.isClassRegistered("Configurable"))
        Configurable::Register();

    if(!luaInterpreter.isClassRegistered("Minimizer")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Minimizer, luabind::bases< Configurable > >("Minimizer")
                .def(luabind::constructor<SpinCuboid&,Functional&>())
                .def("init", &Minimizer::init)
                .def("iterate", &Minimizer::iterate)
                .def("getIteration", &Minimizer::getIteration)
                .def("getFunctionalValue", &Minimizer::getFunctionalValue)
                .def("getCurrentSpins", &Minimizer::getCurrentSpins)
                .def("modifyCurrentSpins", &Minimizer::modifyCurrentSpins)
                .def("setMaxIterations", &Minimizer::setMaxIterations)
                .def("translateStatus", &Minimizer::translateStatus)
                .def("setThreshold", &Minimizer::setThreshold)
        ];
        luaInterpreter.setClassRegistered("Minimizer");
    }
#endif
}

Minimizer::Minimizer(SpinCuboid &start, Functional &functional)
    : Configurable("Minimizer"), m_start(start), m_functional(functional)
    , m_algorithm(gsl_multimin_fdfminimizer_conjugate_fr)
    , m_minimizer(0)
    , m_LSAccuracy(1e-4)
    , m_LSParameter(0.01)
    , m_iteration(0)
    , m_maxIterations(100)
    , m_threshold(1e-3)
{
    m_current = static_cast<SpinCuboid*>(m_start.clone());
    m_currentGSLVector = m_current->getData();
}

Minimizer::~Minimizer() {
    if(m_minimizer)
        gsl_multimin_fdfminimizer_free(m_minimizer);
    if(m_current) {
        m_current->setData(m_currentGSLVector);
        delete m_current;
    }
}

std::string Minimizer::toString() const {
    std::stringstream ss;
    ss << "==== CONFIGURATION FOR MINIMIZER ====" << std::endl;
    ss << " initial LS parameter = " << m_LSParameter << std::endl;
    ss << " LS accuracy = " << m_LSAccuracy << std::endl;
    ss << " max. iterations = " << m_maxIterations << std::endl;
    ss << " threshold = " << m_threshold << std::endl;
    ss << "!=== CONFIGURATION FOR MINIMIZER ====";
    return ss.str();
}


int Minimizer::iterate() {
    m_iteration++;
    int status = gsl_multimin_fdfminimizer_iterate(m_minimizer);
    if (status)
        return status;
    if (m_iteration > m_maxIterations)
        return GSL_EMAXITER;
    status = gsl_multimin_test_gradient (m_minimizer->gradient, m_threshold);
    return status;
}

double Minimizer::getFunctionalValue() const {
    if(m_minimizer)
       return m_minimizer->f;
    return 0;
}

const SpinCuboid &Minimizer::getCurrentSpins() const {
    m_current->setData(m_minimizer->x);
    return *m_current;
}

SpinCuboid &Minimizer::modifyCurrentSpins() {
    m_current->setData(m_minimizer->x);
    return *m_current;
}

void Minimizer::configure(const basic::Configuration& configuration) {

    std::cout << "==== CONFIGURATION FOR MINIMIZER ====" << std::endl;

    try {
    libconfig::Setting& algorithmSetting = configuration.lookup(MAKEPATH("algorithm"));
    std::string algorithm = algorithmSetting;
    std::cout << " algorithm: " << algorithm << std::endl;
    if(algorithm == "Fletcher-Reeves") {
        m_algorithm = gsl_multimin_fdfminimizer_conjugate_fr;
    }
    } catch (...) {
        std::cout << " algorithm = Fletcher-Reeves" << std::endl;
    }

    try {
        libconfig::Setting& lspSetting = configuration.lookup(MAKEPATH("initialLSP"));
        m_LSParameter = lspSetting;
    } catch (...) {
        //ok
    }
    std::cout << " initial LS parameter = " << m_LSParameter << std::endl;
    try {
        libconfig::Setting& lsaccuracySetting = configuration.lookup(MAKEPATH("LSAccuracy"));
        m_LSAccuracy = lsaccuracySetting;
    } catch (...) {
        //ok
    }
    std::cout << " LS accuracy = " << m_LSAccuracy << std::endl;

    try {
        libconfig::Setting& maxIterSetting = configuration.lookup(MAKEPATH("iterations"));
        m_maxIterations = maxIterSetting;
    } catch (...) {
        //ok
    }
    std::cout << " max. iterations = " << m_maxIterations << std::endl;
    try {
        libconfig::Setting& thresholdSetting = configuration.lookup(MAKEPATH("threshold"));
        m_threshold = thresholdSetting;
    } catch (...) {
        //ok
    }
    std::cout << " threshold = " << m_threshold << std::endl;
    std::cout << "!=== CONFIGURATION FOR MINIMIZER ====" << std::endl;
}

std::string Minimizer::translateStatus(int status) {
    std::string message(gsl_strerror(status));
    return message;
}


void Minimizer::init() {
    int solverSize = m_start.getSize();

    m_iteration = 0;
    //if(m_minimizer != 0) {
    //    gsl_multimin_fdfminimizer_free (m_minimizer);
    //    m_minimizer = 0;
    //}
    if( (m_algorithm != 0) && (m_start.getSize() > 0) ) {
        m_minimizer = gsl_multimin_fdfminimizer_alloc (m_algorithm, solverSize);
    }
    else {
        ERROR("algorithm not set or vector length = 0.")
    }


    m_func.n = solverSize;
    m_func.f = Minimizer::m_f;
    m_func.df = Minimizer::m_df;
    m_func.fdf = Minimizer::m_fdf;
    m_func.params = this;

    std::cout << "Solver size: " << solverSize << std::endl;
    for( int d = 0; d < m_start.getDimensions(); d++) {
        std::cout << "dimension " << d << ": " << m_start.getDimension(d) << std::endl;
    }

    gsl_multimin_fdfminimizer_set (m_minimizer, &m_func, m_start.getData(), m_LSParameter, m_LSAccuracy);
}

double Minimizer::m_f (const gsl_vector * x, void * params) {
    Minimizer* m = static_cast< Minimizer* >(params);
    return m->m_functional.f(x, m->m_start);
}

void Minimizer::m_df (const gsl_vector * x, void * params, gsl_vector * g) {
    Minimizer* m = static_cast< Minimizer* >(params);
    m->m_functional.df(x, m->m_start, g);
}

void Minimizer::m_fdf (const gsl_vector * x, void * params, double * f, gsl_vector * g) {
    Minimizer* m = static_cast< Minimizer* >(params);
    *f = m->m_functional.fdf(x, m->m_start, g);
}

