#include "nfa.h"
#include <queue>
#include<algorithm>
using namespace std;
NFA::NFA(Node* Tree)   //nfa树
{
	auto status=gen_status(Tree);
	start_status = status.first;
	end_status = status.second;

	E2NFA();
	start_status->IsFinal = false;  //整体的开始状态不能作为结束状态，防止接收任意字符使其匹配结束
	 //find_end_status();
}
NFA::~NFA()
{

}
pair<Status*, Status*> NFA::gen_status(Node* node)       //NFA 状态分析函数
{
	pair<Status*, Status*> status;                      //一个status的状态集

		switch (node->type)                           //对node进行选择
		{
		case Node::CHAR:
			status = gen_char(node);
			break;
		case Node::AND:
			status = gen_and(node);
			break;
		case Node::OR:
			status = gen_or(node);
			break;
		case Node::RANGE:
			status = gen_range(node);
			break;
		case Node::REPEAT_0:
			status = gen_repeat_0(node);
			break;
		case Node::REPEAT_1:
			status = gen_repeat_1(node);
			break;
		default:
			break;
		}

	return status;
}
pair<Status*, Status*> NFA::gen_range(Node* node)    //范围
{
	Range_Node* range_node = static_cast<Range_Node*>(node);   //声明一个range_node行 node
	Status* s_start = new Status;
	Status* s_end = new Status(true);
	Edge* edge = make_edge(s_start, _MatchContent(range_node->i,range_node->j), s_end);  //给这个node 赋值

	return make_pair(s_start, s_end);         //连接上这两个状态
}
pair<Status*, Status*> NFA::gen_repeat_0(Node* node)   //*型重复
{
	Repeat_Node* repeat_node = static_cast<Repeat_Node*>(node);     //声明一个node型 用基类的指针node 初始化repeat_node
	Status* s_start = new Status(true);
	Status* s_end = s_start;                                      //这两部表示 起始状态和终止状态重复
	Status* _s_start = nullptr;
	Status* _s_end = nullptr;
	pair<Status*, Status*> s_child = gen_status(repeat_node->node);   //repeat_node中有一个node重复变量 上边被初始化了
	_s_start = s_child.first;
	_s_end = s_child.second;
	make_edge(s_start, _s_start);
	make_edge(_s_end, s_end);

	return make_pair(s_start, s_end);

}
pair<Status*, Status*> NFA::gen_repeat_1(Node* node)   //+型重复
{
	Repeat_Node* repeat_node = static_cast<Repeat_Node*>(node);   //声明一个重复对象
	Status* s_start = nullptr;
	Status* s_end = nullptr;
	pair<Status*, Status*> s_child = gen_status(repeat_node->node);    //重复内容
	make_edge(s_child.second, s_child.first);
	s_start = s_child.first;
	s_end = s_child.second;
	s_end->IsFinal = true;                              //作为一次以上的判断标志
	return make_pair(s_start, s_end);
}
pair<Status*, Status*> NFA::gen_and(Node* node)             //处理and
{
	And_Node* and_node = static_cast<And_Node*>(node);
	Status* s_start = nullptr;
	Status* s_end = nullptr;
	for (auto s : *(and_node->pool))   // 遍历这个  pool
	{
		auto tmp = gen_status(s);     //读出每个node
		if (!s_end)                 //如果没有结束
		{
			s_start=tmp.first;
			s_end = tmp.second;
			continue;
		}
		s_end->IsFinal = false;
		make_edge(s_end,tmp.first);  //就把他们连接起来
		s_end = tmp.second;
	}
	s_end->IsFinal= true;               //然后给最后一个为true
	return make_pair(s_start, s_end);
}
pair<Status*, Status*> NFA::gen_or(Node* node)    //处理或
{
	Or_Node* or_node = static_cast<Or_Node*>(node);    //申明一个ornode 对象
	Status* s_start = new Status;
	Status* s_end = new Status(true);
	for (auto s : *(or_node)->pool)
	{
		auto tmp = gen_status(s);
		make_edge(s_start, tmp.first);   //构建开始的那个E边
	    make_edge(tmp.second, s_end);     //结束的那个E边
		tmp.second->IsFinal = true;   //这个地方  去E边时 连接的是or结束边的start状态 make_edge重置为false了
	}

	return make_pair(s_start, s_end);
}
pair<Status*, Status*> NFA::gen_char(Node* node)     //char型
{
	Char_Node* char_node = static_cast<Char_Node*>(node);
	Status* s_start = new Status;
	Status* s_end = new Status(true);
	Edge* edge = make_edge(s_start, _MatchContent(char_node->c, char_node->c), s_end);  //这里传三个参数 其中_MatchContent的left 和right一样
	return make_pair(s_start,s_end);
}

Edge* NFA::make_edge(Status* status1, _MatchContent content, Status* status2,bool isAdd)
{
	if (isAdd&&status1->IsFinal)
		status1->IsFinal = false;                                           //设置的新边
	auto edge = new Edge(status1, content, status2);

	if (isAdd)                                   //如果是进行合并   如果这两个状态没有存在集合中 那么加入到集合中
	{
		if (!_isStatusExist(status1))
			add_status(status1);
		if (!_isStatusExist(status2))
			add_status(status2);
		add_edge(edge);
	}
	return edge;                                       //返回edge
}

Edge* NFA::make_edge(Status* status1, Status* status2,bool isAdd)    //如果是添加的E边 就自动过
{
	return make_edge(status1, _MatchContent(-1, -1), status2,isAdd);
}


void NFA::E2NFA() //NFA转化为DFA （这里是将E边删除）
{
	vector<Status*> valid_status;
	valid_status.push_back(start_status); //start为有效状态
	for (auto status : AllStatus)
	{
		bool exist_valid = false;
		for (auto edge:status->InEdges)
		{
			if (!_isEedge(edge))           //如果不是e边
			{
				exist_valid = true;      //那么把这个exist_valid=true
				break;
			}
		}
		if (exist_valid)                //然后把他放到valid_status集合中去
			valid_status.push_back(status);
	} //获得有效状态 这样获得所有有效边的状态
	vector<Edge*> edges_add;
	vector<Edge*> E_edges; //需要被设置为E的边
	for (auto status : valid_status) //寻找有效状态的E-closure,并将其延伸的有效边复制到有效状态上
	{
		vector<Status*> closure_status;
		queue<Status*> uncomplete_status;
		uncomplete_status.push(status);
		while (!uncomplete_status.empty())
		{
			Status* head = uncomplete_status.front();     //弹出队首元素 head
			uncomplete_status.pop();
			for (auto edge : head->OutEdges)           //遍历head
			{
				if (_isEedge(edge)) //以该状态为起始的E边
				{
					uncomplete_status.push(edge->End);    //加到队列中 继续进行遍历
					closure_status.push_back(edge->End); //加到闭包中
				}
			}
		} //已获得当前status的所有E闭包
		vector<Edge*> valid_edges; //所有由本身及E闭包延伸出的非E边
		for (auto edge:status->OutEdges)
			if (!_isEedge(edge))
				valid_edges.push_back(edge);
		for (auto E_status : closure_status)
		{
			for (auto edge : E_status->OutEdges)
			{
				if (!_isEedge(edge))
					valid_edges.push_back(edge);
			}
		}                              //所有E闭包眼神的非E边

		for (auto edge : valid_edges)  //将有效边复制到有效状态上
		{
			edges_add.push_back (make_edge(status, edge->MatchContent, edge->End, false)); //需要复制的边
			E_edges.push_back(edge);
		}

	}
	for_each(E_edges.begin(), E_edges.end(), [&](Edge* edge){set_edge_E(edge); }); //将所有被复制的边设置为E

	eraseE(); //删除所有E边以及只通过E边到达的状态

	for_each(edges_add.begin(), edges_add.end(), [&](Edge* edge){add_edge(edge); }); //加入新创建的边

	AllStatus.erase(remove_if(AllStatus.begin(), AllStatus.end(), [&](Status* s){return !_isValidStatus(s);}),AllStatus.end()); //删除所有无效状态

}
bool NFA::_isValidStatus(Status* s)
{
	if (s == start_status)
		return true; //start_status 保留
	if (s->InEdges.empty())
		return false;
	bool is_valid = false;
	for (auto edge : (s)->InEdges)
	{
		if (!_isEedge(edge))
		{
			is_valid = true;
			break;
		}
	}
	return is_valid;
}
void NFA::eraseE() //通过遍历的方式删除E边
{
	//for_each(AllEdges.begin(), AllEdges.end(), [&](Edge* edge){if (_isEedge(edge))destroy_edge(edge); }); //destroy每条E边
	AllEdges.erase(remove_if(AllEdges.begin(), AllEdges.end(), [&](Edge* edge){if (_isEedge(edge)) { destroy_edge(edge); return true; } return false; }), AllEdges.end()); //从AllEdges删除所有E边

}
void NFA::find_end_status()
{
	for_each(AllStatus.begin(), AllStatus.end(),[&](Status* status){
		if (status->OutEdges.size() == 0)//入边为0
			status->IsFinal = true;
	});
}
