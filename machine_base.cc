#include <iostream>
#include <sstream>
#include "machines.hh"

mode current_mode;

// Rule functions

Rule::Rule(std::vector<char> *from_states, std::string to_name, std::vector<char> *to_states, std::vector<char> *directions)
{
    _to_name = to_name;
    _from_states.reset(from_states);
    _to_states.reset(to_states);
    _directions.reset(directions);
}

bool Rule::matchRule(const std::vector<char>& from_states) const
{
    if(from_states.size() != _from_states->size())
    {
        return false;
    }

    for(int i = 0; i < from_states.size(); ++i)
    {
        if((*_from_states)[i] == '?' || from_states[i] != (*_from_states)[i])
        {
            return false;
        }
    }
    
    return true;
}

// State fnctions

State::State(std::string name) :
_rules(std::make_unique<std::vector<std::unique_ptr<Rule>>>()),
_accepting(false),
_name(name)
{}

bool State::addRule(std::vector<char> *from_states, std::string to_name,
                    std::vector<char> *to_states, std::vector<char> *directions)
{

    for(auto& rule : (*_rules))
    {
        if(rule->matchRule(*from_states))
        {
            return false;
        }
    }
    
    _rules->push_back(std::make_unique<Rule>(from_states, to_name, to_states, directions));
    return true;
}

int State::getRuleIndex(const std::vector<char>& from_states) const
{
    for(int i = 0; i < _rules->size(); ++i)
    {
        if((*_rules)[i]->matchRule(from_states))
        {
            return i;
        }
    }
    return -1;
}

// Machine functions

Machine::Machine() :
    _states(std::make_unique<std::map<std::string, std::unique_ptr<State>>>()),
    _tapes(std::make_unique<std::vector<std::string>>()),
    _indexes(std::make_unique<std::vector<int>>())
{}

void Machine::error(int line, std::string text)
{
    std::cerr << "Line " << line << ": Error: " << text << std::endl;
    exit(1);
}

std::string Machine::complementTape(std::string tape) const
{
    std::stringstream ss;
    ss<<tape;

    for(int i = tape.length(); i < _tape_length; ++i)
    {
        ss<<"_";
    }
    return ss.str();
}

void Machine::addState(int line, std::string state_name)
{
    if(_states->count(state_name) > 0)
    {
        error(line,"Re-declaration of state: '" + state_name + "'");
    }
    else
    {
        (*_states)[state_name] = std::make_unique<State>(State(state_name));
    }
}

void Machine::addAccepting(int line, std::string accepting_name)
{
    if(_states->count(accepting_name) == 0)
    {
        error(line,"Undefined state: '" + accepting_name + "'");
    }
    else
    {
        (*_states)[accepting_name]->setAccepting();
    }
}

void Machine::addInit(int line, std::string init_name)
{
    if(_states->count(init_name) == 0)
    {
        error(line,"Undefined state: '" + init_name + "'");
    }
    else
    {
        _current = init_name;
    }
}

void Machine::addTapeCount(int line, std::string tape_count)
{
    if(int count = stoi(tape_count); count <= 0)
    {
        error(line, "Tape count is not positive: '" + tape_count + "'");
    }
    else
    {
        _tape_count = count;
    }
}

void Machine::addTapeLength(int line, std::string tape_length)
{
    if(int length = stoi(tape_length); length <= 0)
    {
        error(line, "Tape length is not positive: '" + tape_length + "'");
    }
    else
    {
        _tape_length = length;
    }
}

void Machine::addTapes(int line, std::vector<std::string> *tapes)
{
    if(tapes->size() < _tape_count)
    {
        error(line, "Missing tape definitions");
    }
    else if(tapes->size() > _tape_count)
    {
        error(line, "Overabundance of tape definitions");
    }

    for(std::string tape : (*tapes))
    {
        if(tape.length() > _tape_length)
        {
            error(line, "Tape '" + tape + "' is longer than the maximum tape length of " + std::to_string(_tape_length));
        }

        if(tape != "?")
        {


            _tapes->push_back(complementTape(tape));
        }
        else
        {
            _tapes->push_back(tape);
        }
        _indexes->push_back(0);
    }
}

void Machine::addRule(int line, std::string from_name, std::vector<char> *from_state, std::string to_name,
             std::vector<char> *to_state, std::vector<char> *directions)
{
    if(_states->count(from_name) == 0)
    {
        error(line, "Undefined source state: '" + from_name + "'");
    }

    if(_states->count(to_name) == 0)
    {
        error(line, "Undefined target state: '" + to_name + "'");
    }

    if(from_state->size() < _tape_count)
    {
        error(line, "Shortage of source tape states.");
    }

    if(from_state->size() > _tape_count)
    {
        error(line, "Overabundance of source tape states.");
    }

    if(to_state->size() < _tape_count)
    {
        error(line, "Shortage of target tape states.");
    }

    if(to_state->size() > _tape_count)
    {
        error(line, "Overabundance of target tape states.");
    }

    if(directions->size() < _tape_count)
    {
        error(line, "Shortage of tape directions.");
    }

    if(directions->size() > _tape_count)
    {
        error(line, "Overabundance of tape directions.");
    }

    if((*_states)[from_name]->isAccepting())
    {
        error(line, "A rule must not lead out of an accepting state");
    }

    if(!(*_states)[from_name]->addRule(from_state, to_name, to_state, directions))
    {
        error(line, "A rule is already defined on this state for this case");
    }
}

