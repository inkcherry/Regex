#include "state_list.h"
void Matrix::construct(int n)
{
	matrix = new int*[n];
	for (int i = 0; i < n; ++i)
		matrix[i] = new int[MAX_CHAR];

}
void Matrix::destruct()
{
	for (int i = 0; i < N; ++i)
		delete[]matrix[i];
	delete[]matrix;
}

//由这个函数 得到一个数组  其中数组有三个值
State_List::State_List(DFA& dfa)   //用DFA构造State_List 表
	:matrix(dfa.AllStatus.size())     //用dfa状态数 构造状态表
{
	auto find_status = [&](DFA_Status* status){                     //返回status的下标
		auto iter = find(dfa.AllStatus.begin(), dfa.AllStatus.end(), status);
		if (iter == dfa.AllStatus.end()) return -1;
		return iter - dfa.AllStatus.begin();
	};

	start_status = find_status(dfa.AllStatus[0]);    //初始化start_status start_status是int类型 返回下标
	for (int i = 0; i < matrix.getN(); ++i)    //i从0到n
		for (int j = 0; j < MAX_CHAR; ++j)      //j是ascII字符集
			matrix.set(start_status, i, j);   //用开始状态所有的i j
	int status_no = -1;                      
	for (auto status : dfa.AllStatus)     //遍历DFA ALLStatus
	{
		++status_no;                  //一个状态的计数器  也就是matrix的下标
		if (status->IsFinal)            //如果是结束状态
		{
			for (int i = 0; i < 256; ++i)    //
				matrix.set(-1,status_no,i);
		}

		for (auto edge : dfa.AllEdges)   //嵌套循环 遍历所有边
		{
			if (is_dfa_status_equal(edge->Begin, status))  //如果边的begin状态 = status
			{
				int pos = find_status(edge->End);           //pos=边的结束状态
				for (int i = edge->MatchContent.left; i <= edge->MatchContent.right; ++i)  //查找这个边可以接受的字符集
				{
					matrix.set(pos, status_no, i);           // 然后给这个 边的结束状态   i是字符标识   pos是value 也就是如果接受字符 下一次通向的status
				}
			}
		}
	}
	for (auto iter = dfa.AllStatus.begin(); iter != dfa.AllStatus.end(); ++iter)   //遍历所有的 allstatus
	{
		if ((*iter)->IsFinal)											 //如果是一个结束状态
			end_pos.push_back(iter - dfa.AllStatus.begin());               //将他存入end_pos中去
	}

}