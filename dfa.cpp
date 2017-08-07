#include "dfa.h"
#include<queue>
using namespace std;
bool StateSet::operator==(const StateSet& _set)
{
	if (Set.size() != _set.Set.size())
		return false;
	auto b1 = Set.begin();
	auto b2 = _set.Set.begin();         //�鿴_set�� ��Աset�Ƿ����е�״̬�Ƿ�ȫ��
	for (; b1 != Set.end(); ++b1, ++b2)
	{
		if ((*b1) != (*b2))return false;
	}
	return true;
}
void DFA::constructDFA(NFA& nfa)           
{                                   //L��D����DFA״̬��
	queue<DFA_Status*> L;  //δ�������״̬
	vector<DFA_Status*> D; //�Ѿ����ڵ�DFA״̬  
	L.push(new DFA_Status(nfa.start_status));
	D.push_back(new DFA_Status(nfa.start_status));//���뿪ʼ״̬

	while (!L.empty())      
	{
		auto dfa_status = L.front();
		L.pop(); //�Ӷ�����ȡ��һ��״̬

		vector<pair<_MatchContent, DFA_Status*> > content_DFAStatus_Map;  //
		for (auto nfa_status : dfa_status->status_set) //����DFA״̬��NFA״̬���� ����ЩNFA״̬�ĳ���
		{
			for (auto edge : nfa_status->OutEdges)  //����ÿһ������
			{
				auto iter = find_if(content_DFAStatus_Map.begin(), content_DFAStatus_Map.end(), [&](pair<_MatchContent, DFA_Status*>& p){return p.first == edge->MatchContent; });//˫��ѭ����  ƥ��һ���ı�
				if (iter != content_DFAStatus_Map.end()) //�����ƥ����ͬ���ݵı�
					(*iter).second->add_nfa(edge->End); //ֱ�ӽ������״̬�����Ѿ����ڵ�DFA״̬��  ����iter.sencond��һ��dfastate
				else
				{
					content_DFAStatus_Map.push_back(make_pair(edge->MatchContent, new DFA_Status(edge->End)));  //�����û���������  ��ô�Ͱ����Ž�ȥ
				}
			}
		} //��ʱ����DFA״̬��ת��״̬�������

		for (auto p : content_DFAStatus_Map)   //��L��D�Ĵ���
		{
			auto exist = is_status_exist(p.second, D);
			if (exist >= 0) //�����״̬�Ѿ�������D��
			{
				AllEdges.push_back(make_edge(dfa_status, p.first, D[exist])); //ֻ�轨���߼���   
				delete p.second; //�ͷŶ���״̬
			}
			else
			{
				D.push_back(p.second); //���µ�DFA״̬����D
				AllEdges.push_back(make_edge(dfa_status, p.first, *D.rbegin())); //������
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
int is_status_exist(DFA_Status* dfa_status, vector<DFA_Status*>& d) //��d����Ѱ�Ƿ���dfa_status�Ĵ��ڣ�ע��DFA״̬�������ָ���������е�NFA״̬һ�£�
{
	for (auto iter = d.begin(); iter != d.end(); ++iter)
	{
		if (is_dfa_status_equal(dfa_status, *iter))
			return iter - d.begin();  //�����±�
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
		if ((*iter1) != (*iter2)) // NFA״̬��ͬ��ֱ��ͨ��ָ���ж�
			return false;     //����Ϊʹ��set�������ź�������ֱ�Ӱ�˳��Ƚϼ���
	}
	return true;
}