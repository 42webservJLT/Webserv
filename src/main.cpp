#include "main.hpp"
// [...]

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: ./webserv [config file]" << std::endl;
        return 1;
    }

    (void)argv;

    std::cout << "Hello webserv" << std::endl;
    // [...]
}
