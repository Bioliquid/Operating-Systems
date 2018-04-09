#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

static char memory[30000];

void go_crazy(const char *argv) {
	vector<char> acc;
	char ch;
	ifstream infile(argv);
	while(infile)  {
		infile.get(ch);
		if (ch != '>' && ch != '<' && ch != '+' && ch != '-' && ch != '.' && ch != ',' && ch != '[' && ch != ']' && ch != '\n') {
			cerr << "Read() Error: Unexpected character '" << ch << "'" << endl;
			return;
		}
		acc.push_back(ch);
	}
	infile.close();
	unsigned int j = 0;
	int brc = 0;
	for(size_t i = 0; i < acc.size(); ++i) {
		if(acc[i] == '>') ++j;
		if(acc[i] == '<') --j;
		if(acc[i] == '+') ++memory[j];
		if(acc[i] == '-') --memory[j];
		if(acc[i] == '.') cout << memory[j];
		if(acc[i] == ',') cin >> memory[j];
		if(acc[i] == '[') {
			if (memory[j]) continue;
			++brc;
			while(brc) {
			    ++i;
			    if (acc[i] == '[') ++brc;
			    if (acc[i] == ']') --brc;
			}
		}
		else if(acc[i] == ']') {
			if(!memory[j]) continue;
			if(acc[i] == ']') brc++;
			while(brc) {
			    --i;
			    if(acc[i] == '[') brc--;
			    if(acc[i] == ']') brc++;
			}
			--i;
		}
	}
	cout << endl;
}