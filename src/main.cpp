#include "main.hpp"
// [...]

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: ./webser [config file]" << std::endl;
        return 1;
    }

    std::cout << "Hello webserv" << std::endl;
    // [...]
}
