#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, const char** argv) {
    
    (void) argv;
    if (argc==1) {
    	std::cout << std::endl;

    	std::string s1 = "\n";
    	std::string s2 = "\n";
    	std::string s3 = "\n";
    	std::string s4 = "   './help'\n   Expains the commands in short.\n";

    	std::string s5 = "   author: Rustam Latypov\n"
    					 "email:  rlatypov1337@gmail.com";

    	std::cout << s1 << std::endl;
    	std::cout << s2 << std::endl;
    	std::cout << s3 << std::endl;
    	std::cout << s4 << std::endl;
    	std::cout << s5 << std::endl;
    	std::cout << std::endl;
    }
}