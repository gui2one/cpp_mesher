#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
int main() {
    glm::vec3 pos = glm::vec3(); 
    std::cout << "Hello" << std::endl;
    std::cout << glm::to_string(pos) << std::endl;
    return 0;
}