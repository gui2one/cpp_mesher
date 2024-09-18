#ifndef IMESHOPERATOR_H
#define IMESHOPERATOR_H

#pragma once

class IMeshOperator
{
public:
    
    IMeshOperator();
    ~IMeshOperator();

    virtual void update() = 0;

    // deleting copy and move constructors and assignment operators
    // this is to prevent copying of mesh operators, as this is not a trivial task
    // and we don't want to accidentally copy mesh operators that are still in use
    // by other parts of the program
    IMeshOperator(const IMeshOperator&) = delete; // copy constructor
    IMeshOperator(IMeshOperator&&) = delete; // move constructor
    IMeshOperator& operator=(const IMeshOperator&) = delete; // copy assignment
    IMeshOperator& operator=(IMeshOperator&&) = delete; // move assignment

    inline const char * getName() const{
        return mName;
    }

    inline void setName(const char * name){
        mName = name;
    }


private:


public :
    const char * mName = "default";
};

#endif