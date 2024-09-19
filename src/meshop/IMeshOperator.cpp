#include "IMeshOperator.h"

IMeshOperator::IMeshOperator() :mName("default"), mUUID("00000000-0000-0000-0000-000000000000")
{
    mUUID = gen_uuid();
}

IMeshOperator::~IMeshOperator()
{

}