#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<map>
using namespace std;
#include<algorithm>


int main()
{
		std::string delimiter = " = ";
	// std::vector<std::int,std::int> uf_data;

	std::string file_name = "testing.txt";
	std::ifstream myfile; myfile.open(file_name);
	std::vector<int>uf;
	std::vector<float>speedup;
	std::string myline;
	if ( myfile.is_open() ) {
		while ( myfile ) {
			std::getline (myfile, myline);
			cout<<"my line"<<myline<<endl;
			if(myline.length()>1){
				auto ff=  myline.find(delimiter);
				auto token = myline.substr(0,ff);
				uf.push_back(atoi(token.c_str()));
				auto token2 = myline.substr(ff+delimiter.length(),myline.length());
				float sec = stof(token2.c_str());
				speedup.push_back(sec);
			}
			
		}
		myfile.close();
	}
	else {
		std::cout << "Couldn't open file\n";
	}
	
	
	auto min =std::max_element(speedup.begin(),speedup.end());
	auto opt_UF = uf[std::distance(speedup.begin(), min)];
	auto c = *min.base();
	std::cout<<"optimal UF: "<<opt_UF<<"\n";
	return 0;
}
