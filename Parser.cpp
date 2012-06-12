#include "Parser.h"
#include "DemoModules.h"

Parser::Parser()
{
    terminals.append(new Three); //placeholder for dynamic class loading

    operators.append(new Addition);
}

CAS::AbstractArithmetic *Parser::parse(std::string input)
{
    int ebenencount;
    bool geloescht;
    do {
        if (input.empty()) throw "Ungueltiger (Teil-)Term";
        geloescht = false;
        if (input.at(0) == '(' && input.at(input.length() - 1) == ')') {
            ebenencount = 0;
            for (std::string::iterator i = input.begin() + 1; i != input.end() - 1; ++i) {
                if (*i == '(') ebenencount++;
                else if (*i == ')' && --ebenencount == -1) break;
            }
            if (ebenencount != -1) {
                input.erase(0, 1);
                input.erase(input.length() - 1, 1);
                geloescht = true;
            }
        }
    } while (geloescht);
    CAS::AbstractArithmetic *result;
    for (CAS::SmartList<AbstractTerminal*>::const_iterator it = terminals.begin(); it != terminals.end(); ++it) {
        result = (*it)->parse(input);
        if (result) return result;
    }
    ebenencount = 0;
    unsigned int op_pos = 0;
    CAS::SmartList<AbstractOperator*>::iterator best_op_match = operators.end();

    for (std::string::reverse_iterator i = input.rbegin(); i != input.rend(); ++i) {
        if (*i == '(' || *i == '[' || *i == '{')  ebenencount--;
        else if (*i == ')' || *i == ']' || *i == '}') ebenencount++;
        else if (ebenencount == 0) {
            for (CAS::SmartList<AbstractOperator*>::iterator it_op = operators.begin(); it_op != operators.end(); ++it_op) {
                if ((*it_op)->matches(std::string(1, *i))) {
                    if ((*it_op)->priority() == 0) return (*it_op)->parse(parse(input.substr(0, input.rend() - i - 1)), parse(input.substr(input.rend() - i, i - input.rbegin())));
                    else if (best_op_match == operators.end() || (*it_op)->priority() < (*best_op_match)->priority()){
                        best_op_match == it_op;
                        op_pos = input.rend() - i - 1;
                    }
                }
            }
        }
    }
    if (best_op_match != operators.end())
        return (*best_op_match)->parse(parse(input.substr(0, op_pos)), parse(input.substr(op_pos + 1, input.length() - op_pos - 1)));

    unsigned int pos_parenthesis = input.find_first_of('(');
    std::string identifier = input.substr(0, pos_parenthesis);
    for (CAS::SmartList<AbstractFunction*>::iterator it_func = functions.begin(); it_func != functions.end(); ++it_func)
        if ((*it_func)->matches(identifier)) return (*it_func)->parse(parse(input.substr(pos_parenthesis + 1, input.length() - pos_parenthesis - 1)));
    throw "derp";
}
