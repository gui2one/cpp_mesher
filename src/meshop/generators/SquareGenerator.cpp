#include "SquareGenerator.h"
namespace msh {
SquareGenerator::SquareGenerator()
{

}

SquareGenerator::~SquareGenerator()
{

}

void SquareGenerator::update()
{
    std::cout << "Generating a Square ..." << std::endl;
    if(getInput(0)){
        std::cout << "my 1st input is : " <<getInput(0)->getName() << std::endl;
         
    }
    
}
}
