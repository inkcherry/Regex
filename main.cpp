#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include <algorithm>
#include "nfa.h"
#include "dfa.h"
#include "state_list.h"
#include "re.h"
using namespace std;

int main()
{
	
	Re m("[0-9][0-9]*(\\.|f)");
	/*cout << g.match_all("se192.168.21.13re");*/
	cout << m.match_all("se234.234.11.234fskj23");

	cin.get();
	
	return 0;
}