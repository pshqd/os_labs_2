#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input> <output> <key>\n";
        return 1;
    }
    std::cout << "Args OK: " << argv[1] << " -> " << argv[2]
              << " key=" << argv[3][0] << "\n";
    return 0;
}
