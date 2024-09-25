#include "IMeshOperator.h"
namespace msh {
IMeshOperator::IMeshOperator() : INode(), mMeshCache(Mesh())
{
}

INode::INode():
    mName("default"), 
    mUUID("00000000-0000-0000-0000-000000000000")
{
    mUUID = gen_uuid();
}

}