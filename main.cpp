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
	//Parser a("a|b"); //(ab.+)*|a*
	/*Parser a("[0-9]+\\.");
	NFA b(a.GenTree());
	DFA c(b);
	State_List d(c);*/
	/*	d._test();*/
	Re g("[0-9]*\\.");
	cout << g.match_all("ffc192.168.21¡£13aasdf");
	cin.get();
	
	return 0;
}