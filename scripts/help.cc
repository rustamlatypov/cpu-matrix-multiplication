#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, const char** argv) {
    
    if (argc==1) {
    	std::cout << std::endl;

    	std::string s1 = "\n";
    	std::string s2 = "\n";
    	std::string s3 = "\n";
    	std::string s4 = "   -`./help`\n   Expains the commands in short.";
    	std::replace(s4.begin(), s4.end(), "`", "'")

    	std::cout << s1 << std::endl;
    	std::cout << s2 << std::endl;
    	std::cout << s3 << std::endl;
    	std::cout << s4 << std::endl;
    	std::cout << std::endl;
    }
}