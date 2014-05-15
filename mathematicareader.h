#ifndef MATHEMATICAREADER_H
#define MATHEMATICAREADER_H

#include "array.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace spirit = boost::spirit;

class MathematicaReader
{
    template <typename Iterator>
    struct magrammar : qi::grammar<Iterator, ascii::space_type, spirit::utree()>
    {
        magrammar() : magrammar::base_type(expression)
        {
            using qi::uint_;
            using qi::char_;
            using qi::double_;
            using qi::lit;

            expression = list >> -(lit(';'));
            listElement = double_ | list;
            list = '{' >> listElement % ',' >> '}';
        }

        qi::rule<Iterator, ascii::space_type, spirit::utree()> expression;
        qi::rule<Iterator, ascii::space_type, spirit::utree()> listElement;
        qi::rule<Iterator, ascii::space_type, spirit::utree::list_type()> list;
    };
    typedef struct {
       std::vector< std::vector<int> > coordinates;
       std::vector< double > data;
    } arrayData;
public:
    MathematicaReader();
    Array read(std::string filename);
    static void Register();
protected:
    static bool m_registered;
    void processNode(boost::spirit::utree& tree, arrayData& data, std::vector<int> coordinates);


};

#endif // MATHEMATICAREADER_H
