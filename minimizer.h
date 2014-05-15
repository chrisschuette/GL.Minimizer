#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "spincuboid.h"
#include "configurable.h"
#include <gsl/gsl_multimin.h>

class Functional;
class Minimizer: public Configurable
{
public:
    Minimizer(SpinCuboid& start, Functional& functional);
    virtual ~Minimizer();

    //Named
    virtual std::string getName() const { return "Minimizer"; }

    //Printable
    virtual std::string toString() const;

    std::string translateStatus(int status);
    int iterate();
    double getFunctionalValue() const;
    const SpinCuboid& getCurrentSpins() const;
    SpinCuboid & modifyCurrentSpins();
    void setThreshold(double threshold) { m_threshold = threshold; }

    int getIteration() const { return m_iteration; }
    virtual void configure(const basic::Configuration& configuration);
    void init();
    static void Register();

    //getters and setters
    void setMaxIterations(int maxIter) { m_maxIterations = maxIter; }
protected:

    //optimization functions
    static double m_f (const gsl_vector * x, void * params);
    static void m_df (const gsl_vector * x, void * params, gsl_vector * g);
    static void m_fdf (const gsl_vector * x, void * params, double * f, gsl_vector * g);

    static bool m_registered;
    SpinCuboid* m_current;
    gsl_vector* m_currentGSLVector;

    SpinCuboid& m_start;
    Functional& m_functional;

    // configuration parameterrs
    const gsl_multimin_fdfminimizer_type *m_algorithm;
    gsl_multimin_fdfminimizer *m_minimizer;
    double m_LSAccuracy;
    double m_LSParameter;
    int m_iteration;
    int m_maxIterations;
    double m_threshold;


    gsl_multimin_function_fdf m_func;

};

#endif // MINIMIZER_H
