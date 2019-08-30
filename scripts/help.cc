#include <iostream>

int main(int argc, const char** argv) {
    
    if (argc==1) {
    	std::cout << std::endl;

    	std::string s3 = "'./benchmark <dim> <iter>' default: 'dim=3000', 'iter=10'\nRuns the parallel"
    	"implementation on the same square matrices of dimension 'dim' for `iter` times and outputs running times, "
    	"cpu usages and the their averages.\n";
    	std::string s4 = "'./help'\nExpains the commands in short.";

    	std::cout << s3 << std::endl;
    	std::cout << s4 << std::endl;
    	std::cout << std::endl;
    }
}