#ifndef _NFA_H
#define _NFA_H
#include "parser.h"
#include<algorithm>
#include<utility>
using namespace std;
struct _MatchContent  //matchcontent������int�ͱ���   ��һ������  ��һ��==��ȫ��
{
	_MatchContent(int l, int r) :left(l), right(r) {}
	int left, right; //ƥ������ұ߽�  �����ַ��� ASCII�뷶Χ
	bool operator==(const _MatchContent& m) { return left == m.left&&right == m.right; }  //����==�ж�left right�Ƿ�ȫ��
};

class Status;
class Edge                 //�� ��
{
public:
	Edge(Status* s, _MatchContent m, Status* e) :Start(s), End(e), MatchContent(m) {}
	~Edge() {} //�ͷű߲����ͷ��������յ�
	_MatchContent MatchContent;
	Status* Start;                 //��״̬
	Status* End;                     //��״̬
};


class Status            //״̬  ����Ȧ
{
public:
	Status(bool _is = false) :IsFinal(_is) {}   //Status����һ������ ��������IsFinal 
	~Status();
	vector<Edge*> InEdges;    //���
	vector<Edge*> OutEdges;      //�ձ�
	bool IsFinal=false;

};



class NFA    //NFA ͼ
{
public:
	NFA(Node* tree);  //�������һ��node tree
	~NFA();

public:
	//pair�е�ָ��ֱ�Ϊ����ͼ��start��end
	pair<Status*, Status*> gen_status(Node* node);
	pair<Status*, Status*> gen_and(Node* node);
	pair<Status*, Status*> gen_or(Node* node);
	pair<Status*, Status*> gen_char(Node* node);
	pair<Status*, Status*> gen_range(Node* node);
	pair<Status*, Status*> gen_repeat_0(Node* node);
	pair<Status*, Status*> gen_repeat_1(Node* node);  //һЩ����node* �����򷽷�

	Status* start_status;
	Status* end_status;      //��ʼ�� ���ձ�

	Edge* make_edge(Status* status1, _MatchContent content, Status* status2, bool isAdd = true);
	Edge* make_edge(Status* status1, Status* status2, bool isAdd = true);  //����make_edge �м���˸�matchcontent  isadd������Ϊtrue

	vector<Edge*> AllEdges; //�����滻Ϊmap�����Ч�ʡ�map<Status*,Edge_link>   ���еı߼���
	vector<Status*> AllStatus;           //���е�״̬��

	bool _isStatusExist(Status*s) { return !(find(AllStatus.begin(), AllStatus.end(), s) == AllStatus.end()); } //״̬�Ƿ����
	bool _isEedge(Edge* edge) { return (edge->MatchContent.left == -1); } //�Ƿ�ΪE��
	bool _isValidStatus(Status* s);

	void add_edge(Edge* edge) { AllEdges.push_back(edge); edge->Start->OutEdges.push_back(edge); edge->End->InEdges.push_back(edge); } //��edge���ڲ�����״̬������
	void add_status(Status* s) { AllStatus.push_back(s); }
	void set_edge_E(Edge* edge) { edge->MatchContent.left = edge->MatchContent.right = -1; }
	void destroy_edge(Edge* edge)
	{
		edge->Start->OutEdges.erase(find(edge->Start->OutEdges.begin(), edge->Start->OutEdges.end(), edge)); //ɾ�����ĳ���
		edge->End->InEdges.erase(find(edge->End->InEdges.begin(), edge->End->InEdges.end(), edge)); //ɾ���յ�ĳ���
		delete edge; //�ͷ��ڴ�
					 //ע�⣬��ʱ��δ��AllEdge��ɾ���˱�
	}

	void eraseE();
	void E2NFA();
	void find_end_status();
};
#endif