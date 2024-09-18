#ifndef IMESHOPERATOR_H
#define IMESHOPERATOR_H

#pragma once

class IMeshOperator
{
public:
    IMeshOperator();
    ~IMeshOperator();

    virtual void update() = 0;

private:

};

#endif