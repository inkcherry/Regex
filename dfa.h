#ifndef _DFA_H
#define _DFA_H
#include "nfa.h"
#include <vector>
#include<queue>
#include<set>
using namespace std;


class DFA_Status
{
public:
	DFA_Status():IsFinal(false){}
	DFA_Status(Status* status){ add_nfa(status); } //以单个NFA状态构造DFA状态
	set<Status*> status_set; //对应NFA的状态集合
	void add_nfa(Status* status) { status_set.insert(status); if (status->IsFinal)IsFinal = true; } //加入NFA状态
	bool IsFinal=false;
};
class DFA_Edge
{
public:
	DFA_Edge(DFA_Status* b, _MatchContent content, DFA_Status* e) :Begin(b), MatchContent(content), End(e){}
	_MatchContent MatchContent;
	DFA_Status* Begin;
	DFA_Status* End;               //Stateus是DFA对应的states
};
class StateSet
{
public:
	StateSet(){}
	StateSet(const StateSet& _set){ Set.insert(Set.end(), _set.Set.begin(), _set.Set.end()); } //把set 的元素插入到Set末尾中去
	~StateSet(){}
public:
	vector<Status*> Set;
	void add_state(Status* state){ Set.push_back(state); }
	bool operator==(const StateSet& _set);
	
};

class DFA             //DFA图
{
public:
	DFA(NFA& nfa){ constructDFA(nfa); }   //通过nfa构造dfa
	~DFA(){}
	void _test(){ cout << AllEdges.size() <<endl<< AllStatus.size(); }  //输出边个数
	void constructDFA(NFA& nfa);            //dfa的构造
	vector<DFA_Edge*> AllEdges;
	vector<DFA_Status*> AllStatus;
	 
};

DFA_Edge* make_edge(DFA_Status* b, _MatchContent content, DFA_Status* e);    //连接两个dfa状态
int is_status_exist(DFA_Status* dfa_status, vector<DFA_Status*>& d); //在d中搜寻是否有dfa_status的存在（注意DFA状态的相等是指其中所含有的NFA状态一致）
bool is_dfa_status_equal(const DFA_Status* status1,const  DFA_Status* status2);


#endif