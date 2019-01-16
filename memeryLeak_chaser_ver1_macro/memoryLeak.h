#include "stdafx.h"


#include <iostream>
using namespace std;

#include "LlinkedList.h"
#include <typeinfo>
#include <typeindex>


//------------------------------------------------------------------------------
//	�޸� �Ҵ� ��ũ�� �� �޸� ���� ��ũ��
//------------------------------------------------------------------------------
#define _MALLOC(_data_type, cnt_element) g_MemoryLeak._malloc< _data_type >( cnt_element , (const char*)&__FILE__, __LINE__ )

#define _DELETE(_addr) g_MemoryLeak._delete(_addr, (const char*)&__FILE__, __LINE__ )

//------------------------------------------------------------------------------
//	�޸� �Ҵ� ����Ʈ ��� 
//------------------------------------------------------------------------------
template <typename DATA_TYPE>
struct NODE_MALLLOC{

	DATA_TYPE * _ptr = NULL;///<�Ҵ� �ּ� 
	void * _addr = NULL;	///<�ּ� ��

	int _sz_byte = -1;		///<����Ʈ ũ��
	char _data_type[20];	///<������ Ÿ�� ��

	bool _is_arr = false;	///<�迭 ����
	int _cnt_element;		///<�迭 �� �迭 ����

							//------------------------------------------------------------------------------
							//	������ ���� ��ġ �� ���� �Ҵ� ��ũ�� ȣ�� �ڵ� ����.
							//------------------------------------------------------------------------------
	char *_file_name = NULL;	//ȣ�� �� ���� ��
	int _line = -1;				//ȣ�� �� ���� ��ġ
};

//------------------------------------------------------------------------------
//	�޸� ���� ����Ʈ ���
//------------------------------------------------------------------------------
template <typename DATA_TYPE>
struct NODE_FREE{

	DATA_TYPE * _ptr = NULL;	///<�Ҵ� �ּҰ�.
	void * _addr = NULL;		///< �ּҰ�.

	int _sz_byte = -1;		///<����Ʈ ũ��.
	char _data_type[20];		///<������ Ÿ��.

	bool _is_arr = false;	///<�迭 ����.
	int _cnt_element;			///<�迭 �� �迭 ����.


	//------------------------------------------------------------------------------
	//	������ ��ũ�� ���� �� �� ȣ�� ����.
	//------------------------------------------------------------------------------
	char *_file_name = NULL;
	int _line = -1; 


	//------------------------------------------------------------------------------
	//	�Ҹ��� ��ũ�� ���� �� �� ȣ�� ����.
	//------------------------------------------------------------------------------
	char *file_name_free = NULL;
	int line_free = -1; //���� �ڵ� ����
};


//------------------------------------------------------------------------------
//	�޸� ���� Ž�� Ŭ����.
//------------------------------------------------------------------------------
class MemoryLeak {
private:

	
	float _total_sz_byte_malloc;	///<�� ���� ������.

	float _sz_alloc;				///<���� ������.

	float _total_sz_byte_free = 0;	///<�� ���� ������.

	int _total_cnt_malloc;			///<�� ���� Ƚ��.
	
	int _total_cnt_delete;			///<�� �Ҹ� Ƚ��.


	//------------------------------------------------------------------------------
	//	�޸� ���� ����Ʈ 
	//------------------------------------------------------------------------------
	LinkedList< NODE_MALLLOC<void*> * > _list_malloc;

	//------------------------------------------------------------------------------
	//	�޸� ���� ����Ʈ 
	//------------------------------------------------------------------------------
	LinkedList< NODE_FREE<void* >* > _list_free;

public:


	//------------------------------------------------------------------------------
	//	������
	//------------------------------------------------------------------------------
	MemoryLeak(){

		//�� ���� ������
		_total_sz_byte_malloc = 0;

		//���� ������
		_sz_alloc = 0;

		//�� ���� ������
		_total_sz_byte_free = 0;

		//�� ���� Ƚ��
		_total_cnt_malloc = 0;

		//�� �Ҹ� Ƚ��
		_total_cnt_delete = 0;
	}



	//------------------------------------------------------------------------------
	//	��� �޸� ���� ����Ʈ ���
	//------------------------------------------------------------------------------
	void DisplayMallocStatics(){

		//������ ����Ʈ ������
		int num_element = -1;
		num_element = _list_malloc.size();

		//��ȸ ������ ����
		NODE_MALLLOC<void*> *cur_ptr = NULL;

		cout << "[Memory Allocation List]" << endl;

		if (num_element == 0){

			cout << "	�� [empty]" << endl << endl << endl;
			return;
		}

		//------------------------------------------------------------------------------
		//	����Ʈ ��� ��ȸ �� ��� ���� ���
		//------------------------------------------------------------------------------
		for (int i = 0; i < num_element; i++){

			cur_ptr = _list_malloc.get(i);

			cout << "  [" << i << "] " << (unsigned int*)cur_ptr->_addr << "(type: " << cur_ptr->_data_type << ")" << endl;
			cout << "  �� Byte Size: " << cur_ptr->_sz_byte << "(" << cur_ptr->_data_type << "*" << cur_ptr->_cnt_element << ")" << "Byte" << endl;
			cout << "  �� File : " << cur_ptr->_file_name << endl;
			cout << "  �� Line: " << cur_ptr->_line << endl;
			cout << "  �� Share: " << ((cur_ptr->_sz_byte / _total_sz_byte_malloc  )) * 100 << "%" << endl << endl;
		}
	}


	//------------------------------------------------------------------------------
	//	��� �޸� ���� ����Ʈ ���
	//------------------------------------------------------------------------------
	void DisplayFreeStatics(){

		//�Ҹ��� ����Ʈ ������
		int num_element = -1;
		num_element = _list_free.size();

		//��ȸ ������ ����
		NODE_FREE<void*> *cur_ptr = NULL;

		//------------------------------------------------------------------------------
		//	����Ʈ ��� ��ȸ �� ��� ���� ���
		//------------------------------------------------------------------------------

		cout << "[Memory Free Memory List]" << endl;

		if (num_element == 0){

			cout << " ��[empty]" << endl << endl << endl;
			return;
		}

		for (int i = 0; i < num_element; i++){

			cur_ptr = _list_free.get(i);

			cout << "  [" << i << "] " << (unsigned int*)cur_ptr->_addr << "(type: " << cur_ptr->_data_type << ")" << endl;
			cout << "  �� Byte Size: " << cur_ptr->_sz_byte << "(" << cur_ptr->_data_type << "*" << cur_ptr->_cnt_element << ")" "Byte" << endl;
			cout << "  �� File: " << cur_ptr->file_name_free << "(con: " << cur_ptr->_file_name << ")" << endl;
			cout << "  �� Line: " << cur_ptr->line_free << "(con: " << cur_ptr->_line << ")" << endl;
			cout << "  �� Share: " << ((cur_ptr->_sz_byte / _total_sz_byte_malloc)) * 100 << "%" << endl << endl;
		
		
		}

		cout << "===========================================================" << endl << endl;
	}


	//------------------------------------------------------------------------------
	//	��� ���
	//------------------------------------------------------------------------------
	void DisplayAllStatics(){

		int cnt_malloc = _list_malloc.size();
		int cnt_free = _list_free.size();

		


		//�� ���1 
		cout << "[Total Statics]" << endl;
		cout << "	[Total Malloc]" << endl;
		cout << "	�� Size: " << _total_sz_byte_malloc << "Byte" << endl;
		cout << "	�� Count: " << _total_cnt_malloc << endl << endl;


		cout << "	[Total Free]" << endl;
		cout << "	�� Size: " << _total_sz_byte_free << "Byte" << endl;
		cout << "	�� Count: " << _total_cnt_delete << endl << endl;

		cout << "===========================================================" << endl << endl;

		cout << "[Malloc Statics]" << endl;
		cout << "	�� Size: " << _total_sz_byte_malloc - _total_sz_byte_free << "Byte" << endl;
		cout << "	�� Count: " << cnt_malloc << endl;
		cout << "	�� Share: " << (1.0-(_total_sz_byte_free / _total_sz_byte_malloc)) * 100 << "%" << endl << endl;


		cout << "[Free Statics]" << endl;
		cout << "	�� Size: " << _total_sz_byte_free << "Byte" << endl;
		cout << "	�� Count: " << cnt_free << endl;
		cout << "	�� Share: " << (_total_sz_byte_free/ _total_sz_byte_malloc) *100<<"%"<< endl << endl<<endl;


		cout << "===========================================================" << endl << endl;


		DisplayMallocStatics();
		DisplayFreeStatics();


	}


	//------------------------------------------------------------------------------
	//	�޸� ���� �Լ�.
	//------------------------------------------------------------------------------
	template <typename DATA_TYPE>
	DATA_TYPE* _malloc(int cnt_element = 1, const char *file_name = NULL, int _line = -1)
	{

		//------------------------------------------------------------------------------
		//	������ ����.
		//------------------------------------------------------------------------------
		DATA_TYPE *out_ptr = new DATA_TYPE[cnt_element];

		//------------------------------------------------------------------------------
		//	������ ����Ʈ�� ���� �� ��� ����.
		//------------------------------------------------------------------------------
		NODE_MALLLOC<void*> *new_node = new NODE_MALLLOC<void*>;

		//------------------------------------------------------------------------------
		//	������ ������ ����.
		//------------------------------------------------------------------------------

		//�ּ� ����.
		new_node->_addr = out_ptr;

		//������ Ÿ�� ����.
		strcpy_s(new_node->_data_type, typeid(DATA_TYPE).name());

		int sz_byte_tmp = -1;

		//�迭 �� ���.
		if (cnt_element > 1) {

			//�迭 ���� ����.
			new_node->_cnt_element = cnt_element;

			//�迭 ���� ����.
			new_node->_is_arr = true;

			//������ ����.
			sz_byte_tmp = sizeof(DATA_TYPE) * cnt_element;
			new_node->_sz_byte = sz_byte_tmp;

		}

		//���� Ÿ���� ���.
		else{

			new_node->_cnt_element = 1;

			//�迭 ���� ����
			new_node->_is_arr = false;

			//������ ����
			sz_byte_tmp = sizeof(DATA_TYPE);
		}

		//�� ���� Ƚ�� ����.
		_total_cnt_malloc++;

		//�� ���� ������ ����.
		_total_sz_byte_malloc += sz_byte_tmp;

		//���� ������ ����.
		_sz_alloc=+ sz_byte_tmp;


		//���� ����.
		new_node->_line = _line;

		//���ϸ� ����.
		new_node->_file_name = (char*)file_name;

		//���� ����Ʈ�� ����.
		_list_malloc.add(new_node);

		//------------------------------------------------------------------------------
		//	������ ��ȯ
		//------------------------------------------------------------------------------
		return out_ptr;
	}



	//------------------------------------------------------------------------------
	//	�޸� ���� �Լ�.
	//------------------------------------------------------------------------------
	template <typename DATA_TYPE>
	bool _delete(DATA_TYPE delete_addr, const char *file_name = NULL, int _line = -1)
	{

		//������ ����Ʈ ��ȸ ��� ������ ����
		NODE_MALLLOC<void*> *cur_ptr = NULL;

		//------------------------------------------------------------------------------
		//	������ ����Ʈ ��ȸ 
		//
		//	:������ ����Ʈ ��ȸ�ϸ� delete_addr�� ���� ���� ��尡 �ִ��� Ž��
		//------------------------------------------------------------------------------
		void *addr = NULL;	//�ּ� ���� ���̵� ������


		//------------------------------------------------------------------------------
		//	���� ��尡 �����ϴ��� üũ�ϴ� �� Ÿ�� 
		//------------------------------------------------------------------------------
		bool check_find_delele_node = false;
		//����Ʈ ������
		int num_element = -1;
		num_element = _list_malloc.size();
		for (int i = 0; i < num_element; i++)
		{
			cur_ptr = _list_malloc.get(i);
			addr = cur_ptr->_addr;

			//------------------------------------------------------------------------------
			//	���� ��尡 ������ ����Ʈ�� �����ϴ� ��� 
			//
			//	:������ ����Ʈ���� �ش� ����Ʈ ��带 ����
			//	:�÷��� ����
			//------------------------------------------------------------------------------
			if (addr == delete_addr)
			{
				check_find_delele_node = true;
				_list_malloc.remove(i);
				break;
			}
		}

		//------------------------------------------------------------------------------
		//	���� ��尡 ������ ����Ʈ�� �������� �ʴ� ���
		//
		//	:�� ã��
		//------------------------------------------------------------------------------
		if (check_find_delele_node == false)
		{
			return false;
		}

		//------------------------------------------------------------------------------
		//	���� ��尡 ������ ����Ʈ�� �����ϴ� ���
		//
		//	�Ҹ��� ����Ʈ ��带 ���� �� �Ҹ��� ����Ʈ�� ����
		//------------------------------------------------------------------------------
		else if (check_find_delele_node == true)
		{

			NODE_FREE<void*> *release_ptr = new NODE_FREE<void*>;

			//�Ҹ��� ������
			release_ptr->_ptr = cur_ptr->_ptr;

			//�ּ� ��
			release_ptr->_addr = cur_ptr->_addr;

			//����Ʈ ũ��
			int sz_byte_tmp = -1;
			sz_byte_tmp = cur_ptr->_sz_byte;
			release_ptr->_sz_byte = sz_byte_tmp;


			//�� ���� ������ ũ�� ����
			//�� ���� Ƚ�� ����
			_total_cnt_delete++;
			_total_sz_byte_free += sz_byte_tmp;

			//���� ������ ����
			_sz_alloc -= sz_byte_tmp;


			//������ Ÿ��.
			strcpy_s(release_ptr->_data_type, cur_ptr->_data_type);

			//�迭 ����.
			release_ptr->_is_arr = cur_ptr->_is_arr;

			//�迭 ����.
			release_ptr->_cnt_element = cur_ptr->_cnt_element;

			//������ �Լ� ȣ�� ���� ��ġ �� ���� �ڵ�.
			release_ptr->_file_name = cur_ptr->_file_name;
			release_ptr->_line = cur_ptr->_line; //���� �ڵ� ����.

			//�Ҹ��� �Լ� ȣ�� ���� ��ġ �� ���� �ڵ�.
			release_ptr->line_free = _line; //���� �ڵ� ����
			release_ptr->file_name_free = (char*)file_name;

			//�Ҹ��� ����Ʈ�� ����.
			_list_free.add(release_ptr);

			//�Ҹ��� ����.
			delete(delete_addr);

			return true;

		}

	}
};

static MemoryLeak g_MemoryLeak;







