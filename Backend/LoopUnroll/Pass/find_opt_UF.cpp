#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<map>
using namespace std;



int main()
{
	int a[] = { 1, 0, 2, 1 };
	int n = sizeof(a) / sizeof(a[0]);

	std::string delimiter = " = ";
	// std::vector<std::int,std::int> uf_data;

	std::string file_name = "t.txt";
	std::ifstream myfile; myfile.open(file_name);
	std::vector<int>uf;
	std::vector<int>perf_time;
	std::string myline;
	if ( myfile.is_open() ) {
		while ( myfile ) {
			std::getline (myfile, myline);
			std::cout<<myline<< '\n';
			auto ff=  myline.find(delimiter);
			auto token = myline.substr(0,ff);
			uf.push_back(atoi(token.c_str()));
			// std::cout<<"delim length"<<ff+delimiter.length()<<"\n";
			auto token2 = myline.substr(ff+delimiter.length()+5,myline.length());//change 5 to 4 for real-> sys
			auto temp = token2.substr(0,token2.find('.'));//mins
			auto temp2 = token2.substr(token2.find('.')+1, temp.length());//sec
			auto temp3 = temp.substr(0,temp.find('m')) + temp.substr(temp.find('m')+1, temp.length());
			int sec = atoi(temp2.c_str());
			sec = sec + 60 * atoi(temp3.c_str());
			std::cout<<"after split "<<temp<<"\n";
			std::cout<<"after split again  "<<temp2<<"\n";
			std::cout<<"finally "<<sec<<"\n";
			perf_time.push_back(sec);
			std::cout<<atoi(token.c_str())<< '\n';
			std::cout<<token2<<"\n";
		}
		myfile.close();
	}
	else {
		std::cout << "Couldn't open file\n";
	}
	int num = perf_time.size();
	std::vector<std::vector<int>> data(2, std::vector<int>(num)); 
	for (int i = 0; i < num ; i++){
		cout<<perf_time[i]<<" ";
		// cout<<uf[i]<<"\n";
	}
	std::cout<<"\n got here \n";
	auto min =std::min(perf_time.begin(),perf_time.end());
	auto opt_UF = uf[std::distance(perf_time.begin(), min)];
	auto c = *min.base();
	std::cout<<"min value %d "<<*min.base()<<"\n";
	std::cout << "index of smallest element: " <<
			std::distance(perf_time.begin(), std::min(perf_time.begin(), perf_time.end()));
	std::cout<<"UF: "<<opt_UF<<"\n";
	return 0;
}
