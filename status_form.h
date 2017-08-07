#ifndef _STATUS_FORM_H
#define _STATUS_FORM_H
#include "parser.h"
#include<vector>
#include<algorithm>
using namespace std;
struct _MatchContent
{
	_MatchContent(int l, int r) :left(l), right(r){}
	int left, right; //Æ¥ÅäµÄ×óÓÒ±ß½ç
};

class Status;
class Edge
{
public:
	Edge(Status* s, Status* e, _MatchContent m) :Start(s), End(e), MatchContent(m){}
	_MatchContent MatchContent;
	Status* Start;
	Status* End;
};

class Status
{
public:
	Status() :IsFinal(false){}
	vector<Edge*> InEdges;
	vector<Edge*> OutEdges;
	bool IsFinal;
};

void AddEdges(Status* status1, Status* status2, _MatchContent content)
{
	auto result1 = find(AllStatus.begin(), AllStatus.end(), status1);
	auto result2 = find(AllStatus.begin(), AllStatus.end(), status2);
	if (result1 == AllStatus.end())
		AllStatus.push_back(status1);
	if (result2 == AllStatus.end())
		AllStatus.push_back(status2);


}

vector<Edge*> AllEdges;
vector<Status*> AllStatus;

Edge* make_edge(Status* status1, Status* status2, _MatchContent content)
{
	return new Edge(status1, status2, content);
}

Edge* make_edge(Status* status1, Status* status2)
{
	return make_edge(status1, status2, _MatchContent(-1, -1));
}


#endif