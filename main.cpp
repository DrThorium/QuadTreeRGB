#include "QuadTree.h"

int main() {
    std::string strg_;
    int tol;
    std::cout << "Filename ? ... " << std::endl;
    std::cin >> strg_;
    std::cout << std::endl;
    std::cout << "Tolerance?" << std::endl;
    std::cin >> tol;
    QuadTree QT_obj;
    QT_obj.compress(strg_, tol);
    return 0;
}
