#include "dfa.h"
#include<queue>
using namespace std;
bool StateSet::operator==(const StateSet& _set)
{
	if (Set.size() != _set.Set.size())
		return false;
	auto b1 = Set.begin();
	auto b2 = _set.Set.begin();         //查看_set和 成员set是否所有的状态是否全等
	for (; b1 != Set.end(); ++b1, ++b2)
	{
		if ((*b1) != (*b2))return false;
	}
	return true;
}
void DFA::constructDFA(NFA& nfa)           
{                                   //L和D都是DFA状态集
	queue<DFA_Status*> L;  //未被处理的状态
	vector<DFA_Status*> D; //已经存在的DFA状态  
	L.push(new DFA_Status(nfa.start_status));
	D.push_back(new DFA_Status(nfa.start_status));//加入开始状态

	while (!L.empty())      
	{
		auto dfa_status = L.front();
		L.pop(); //从队列中取出一个状态

		vector<pair<_MatchContent, DFA_Status*> > content_DFAStatus_Map;  //
		for (auto nfa_status : dfa_status->status_set) //遍历DFA状态的NFA状态集合 找这些NFA状态的出边
		{
			for (auto edge : nfa_status->OutEdges)  //对于每一条出边
			{
				auto iter = find_if(content_DFAStatus_Map.begin(), content_DFAStatus_Map.end(), [&](pair<_MatchContent, DFA_Status*>& p){return p.first == edge->MatchContent; });//双层循环找  匹配一样的边
				if (iter != content_DFAStatus_Map.end()) //如果有匹配相同内容的边
					(*iter).second->add_nfa(edge->End); //直接将到达的状态加入已经存在的DFA状态中  这里iter.sencond是一个dfastate
				else
				{
					content_DFAStatus_Map.push_back(make_pair(edge->MatchContent, new DFA_Status(edge->End)));  //如果并没有这个出边  那么就把它放进去
				}
			}
		} //此时对于DFA状态的转移状态处理完毕

		for (auto p : content_DFAStatus_Map)   //对L和D的处理
		{
			auto exist = is_status_exist(p.second, D);
			if (exist >= 0) //如果该状态已经存在于D中
			{
				AllEdges.push_back(make_edge(dfa_status, p.first, D[exist])); //只需建立边即可   
				delete p.second; //释放多余状态
			}
			else
			{
				D.push_back(p.second); //将新的DFA状态加入D
				AllEdges.push_back(make_edge(dfa_status, p.first, *D.rbegin())); //建立边
				L.push(p.second);

			}
		}
	}
	AllStatus.insert(AllStatus.end(), D.begin(), D.end());
}

DFA_Edge* make_edge(DFA_Status* b, _MatchContent content, DFA_Status* e)
{
	return new DFA_Edge(b, content, e);
}
int is_status_exist(DFA_Status* dfa_status, vector<DFA_Status*>& d) //在d中搜寻是否有dfa_status的存在（注意DFA状态的相等是指其中所含有的NFA状态一致）
{
	for (auto iter = d.begin(); iter != d.end(); ++iter)
	{
		if (is_dfa_status_equal(dfa_status, *iter))
			return iter - d.begin();  //返回下标
	}
	return -1;
}
bool is_dfa_status_equal(const DFA_Status* status1,const DFA_Status* status2)
{
	if (status1->status_set.size() != status2->status_set.size())
		return false;

	auto iter1 = status1->status_set.begin();
	auto iter2 = status2->status_set.begin();
	for (; iter1 != status1->status_set.end(); ++iter1, ++iter2)
	{
		if ((*iter1) != (*iter2)) // NFA状态相同可直接通过指针判断
			return false;     //另，因为使用set容器已排好序，所以直接按顺序比较即可
	}
	return true;
}