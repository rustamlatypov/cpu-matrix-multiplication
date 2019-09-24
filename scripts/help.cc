#include <iostream>
#include <algorithm>
#include <string>



int main(int argc, const char** argv) {
    
    (void) argv;
    if (argc==1) {
    	std::cout << std::endl;

    	std::string s1 = "  -run <dim> <iter>\n"
                         "   default: dim=1000, iter=3\n";
    	std::string s2 = "  -test <ny> <nm> <nx> <verbose>\n"
                         "   default: ny,nm,nx in {5,1500}, verbose=0\n";
        std::string s3 = "  -benchmark <dim> <iter>\n"
                         "   default: dim=3000, iter=10\n";
    	std::string s4 = "  -help\n";
    	std::string s5 = "  author: rustam latypov\n"
    					 "  email:  rlatypov1337@gmail.com\n"
                         "  doc:    github.com/rustamlatypov/cpu-matrix-multiplication";

    	std::cout << s1 << std::endl;
    	std::cout << s2 << std::endl;
    	std::cout << s3 << std::endl;
    	std::cout << s4 << std::endl;
    	std::cout << s5 << std::endl;
    	std::cout << std::endl;
    }
}