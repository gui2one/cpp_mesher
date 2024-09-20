#include "IMeshOperator.h"
namespace msh {
IMeshOperator::IMeshOperator() :
    mName("default"), 
    mUUID("00000000-0000-0000-0000-000000000000"), 
    mMeshCache(Mesh())
{
    mUUID = gen_uuid();
}

IMeshOperator::~IMeshOperator()
{

}
}