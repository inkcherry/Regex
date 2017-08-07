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

//��������� �õ�һ������  ��������������ֵ
State_List::State_List(DFA& dfa)   //��DFA����State_List ��
	:matrix(dfa.AllStatus.size())     //��dfa״̬�� ����״̬��
{
	auto find_status = [&](DFA_Status* status){                     //����status���±�
		auto iter = find(dfa.AllStatus.begin(), dfa.AllStatus.end(), status);
		if (iter == dfa.AllStatus.end()) return -1;
		return iter - dfa.AllStatus.begin();
	};

	start_status = find_status(dfa.AllStatus[0]);    //��ʼ��start_status start_status��int���� �����±�
	for (int i = 0; i < matrix.getN(); ++i)    //i��0��n
		for (int j = 0; j < MAX_CHAR; ++j)      //j��ascII�ַ���
			matrix.set(start_status, i, j);   //�ÿ�ʼ״̬���е�i j
	int status_no = -1;                      
	for (auto status : dfa.AllStatus)     //����DFA ALLStatus
	{
		++status_no;                  //һ��״̬�ļ�����  Ҳ����matrix���±�
		if (status->IsFinal)            //����ǽ���״̬
		{
			for (int i = 0; i < 256; ++i)    //
				matrix.set(-1,status_no,i);
		}

		for (auto edge : dfa.AllEdges)   //Ƕ��ѭ�� �������б�
		{
			if (is_dfa_status_equal(edge->Begin, status))  //����ߵ�begin״̬ = status
			{
				int pos = find_status(edge->End);           //pos=�ߵĽ���״̬
				for (int i = edge->MatchContent.left; i <= edge->MatchContent.right; ++i)  //��������߿��Խ��ܵ��ַ���
				{
					matrix.set(pos, status_no, i);           // Ȼ������ �ߵĽ���״̬   i���ַ���ʶ   pos��value Ҳ������������ַ� ��һ��ͨ���status
				}
			}
		}
	}
	for (auto iter = dfa.AllStatus.begin(); iter != dfa.AllStatus.end(); ++iter)   //�������е� allstatus
	{
		if ((*iter)->IsFinal)											 //�����һ������״̬
			end_pos.push_back(iter - dfa.AllStatus.begin());               //��������end_pos��ȥ
	}

}