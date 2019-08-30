#include <iostream>

int main(int argc, const char** argv) {
    
    if (argc==1) {
    	std::string s1 = "'./run <dim> <iter>'' default: 'dim=1000', 'iter=3'\n
Runs both implementations on the same matrices for 'iter' times and outputs the average running times, cpu usages, the speedup obtained and the error term.";
    	std::string s2 = "`./test <ny> <nm> <nx> <verbose>` default: `ny,nm,nx in {5,1500}`, `verbose=0` <br/>
Runs both implementations on different matrices and different combinations of `ny`, `nm` and `nx`. Dimension wise the multiplication is `(ny*nm)(nm*nx)`. Each combination is run **10** times and the error term is accumulated and outputed. Each test passes if the accumulated error is below **1e-3**. Output can be enabled using `verbose=1`.";
    	std::string s3 = "`./benchmark <dim> <iter>` default: `dim=3000`, `iter=10` <br/>
Runs the parallel implementation on the same square matrices of dimension `dim` for `iter` times and outputs running times, cpu usages and the their averages.
";
    	std::string s4 = "`./help` <br/>
Expains the commands in short.";
		printf("s1\n");
		printf("s2\n");
		printf("s3\n");
		printf("s4\n");
    }
}