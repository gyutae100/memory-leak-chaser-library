#include "stdafx.h"


#include <iostream>
using namespace std;

#include "LlinkedList.h"
#include <typeinfo>
#include <typeindex>


//------------------------------------------------------------------------------
//	메모리 할당 매크로 및 메모리 해제 매크로
//------------------------------------------------------------------------------
#define _MALLOC(_data_type, cnt_element) g_MemoryLeak._malloc< _data_type >( cnt_element , (const char*)&__FILE__, __LINE__ )

#define _DELETE(_addr) g_MemoryLeak._delete(_addr, (const char*)&__FILE__, __LINE__ )

//------------------------------------------------------------------------------
//	메모리 할당 리스트 노드 
//------------------------------------------------------------------------------
template <typename DATA_TYPE>
struct NODE_MALLLOC{

	DATA_TYPE * _ptr = NULL;///<할당 주소 
	void * _addr = NULL;	///<주소 값

	int _sz_byte = -1;		///<바이트 크기
	char _data_type[20];	///<데이터 타입 명

	bool _is_arr = false;	///<배열 유무
	int _cnt_element;		///<배열 시 배열 길이

							//------------------------------------------------------------------------------
							//	생성자 파일 위치 및 동적 할당 매크로 호출 코드 라인.
							//------------------------------------------------------------------------------
	char *_file_name = NULL;	//호출 시 파일 명
	int _line = -1;				//호출 시 라인 위치
};

//------------------------------------------------------------------------------
//	메모리 해제 리스트 노드
//------------------------------------------------------------------------------
template <typename DATA_TYPE>
struct NODE_FREE{

	DATA_TYPE * _ptr = NULL;	///<할당 주소값.
	void * _addr = NULL;		///< 주소값.

	int _sz_byte = -1;		///<바이트 크기.
	char _data_type[20];		///<데이터 타입.

	bool _is_arr = false;	///<배열 유무.
	int _cnt_element;			///<배열 시 배열 길이.


	//------------------------------------------------------------------------------
	//	생성자 매크로 파일 명 및 호출 라인.
	//------------------------------------------------------------------------------
	char *_file_name = NULL;
	int _line = -1; 


	//------------------------------------------------------------------------------
	//	소멸자 매크로 파일 명 및 호출 라인.
	//------------------------------------------------------------------------------
	char *file_name_free = NULL;
	int line_free = -1; //해제 코드 라인
};


//------------------------------------------------------------------------------
//	메모리 누수 탐지 클래스.
//------------------------------------------------------------------------------
class MemoryLeak {
private:

	
	float _total_sz_byte_malloc;	///<총 생성 사이즈.

	float _sz_alloc;				///<생성 사이즈.

	float _total_sz_byte_free = 0;	///<총 삭제 사이즈.

	int _total_cnt_malloc;			///<총 생성 횟수.
	
	int _total_cnt_delete;			///<총 소멸 횟수.


	//------------------------------------------------------------------------------
	//	메모리 생성 리스트 
	//------------------------------------------------------------------------------
	LinkedList< NODE_MALLLOC<void*> * > _list_malloc;

	//------------------------------------------------------------------------------
	//	메모리 해제 리스트 
	//------------------------------------------------------------------------------
	LinkedList< NODE_FREE<void* >* > _list_free;

public:


	//------------------------------------------------------------------------------
	//	생성자
	//------------------------------------------------------------------------------
	MemoryLeak(){

		//총 생성 사이즈
		_total_sz_byte_malloc = 0;

		//생성 사이즈
		_sz_alloc = 0;

		//총 삭제 사이즈
		_total_sz_byte_free = 0;

		//총 생성 횟수
		_total_cnt_malloc = 0;

		//총 소멸 횟수
		_total_cnt_delete = 0;
	}



	//------------------------------------------------------------------------------
	//	모든 메모리 생성 리스트 출력
	//------------------------------------------------------------------------------
	void DisplayMallocStatics(){

		//생성자 리스트 사이즈
		int num_element = -1;
		num_element = _list_malloc.size();

		//순회 포인터 선언
		NODE_MALLLOC<void*> *cur_ptr = NULL;

		cout << "[Memory Allocation List]" << endl;

		if (num_element == 0){

			cout << "	┗ [empty]" << endl << endl << endl;
			return;
		}

		//------------------------------------------------------------------------------
		//	리스트 노드 순회 및 노드 정보 출력
		//------------------------------------------------------------------------------
		for (int i = 0; i < num_element; i++){

			cur_ptr = _list_malloc.get(i);

			cout << "  [" << i << "] " << (unsigned int*)cur_ptr->_addr << "(type: " << cur_ptr->_data_type << ")" << endl;
			cout << "  ┠ Byte Size: " << cur_ptr->_sz_byte << "(" << cur_ptr->_data_type << "*" << cur_ptr->_cnt_element << ")" << "Byte" << endl;
			cout << "  ┠ File : " << cur_ptr->_file_name << endl;
			cout << "  ┗ Line: " << cur_ptr->_line << endl;
			cout << "  ┗ Share: " << ((cur_ptr->_sz_byte / _total_sz_byte_malloc  )) * 100 << "%" << endl << endl;
		}
	}


	//------------------------------------------------------------------------------
	//	모든 메모리 해제 리스트 출력
	//------------------------------------------------------------------------------
	void DisplayFreeStatics(){

		//소멸자 리스트 사이즈
		int num_element = -1;
		num_element = _list_free.size();

		//순회 포인터 선언
		NODE_FREE<void*> *cur_ptr = NULL;

		//------------------------------------------------------------------------------
		//	리스트 노드 순회 및 노드 정보 출력
		//------------------------------------------------------------------------------

		cout << "[Memory Free Memory List]" << endl;

		if (num_element == 0){

			cout << " ┗[empty]" << endl << endl << endl;
			return;
		}

		for (int i = 0; i < num_element; i++){

			cur_ptr = _list_free.get(i);

			cout << "  [" << i << "] " << (unsigned int*)cur_ptr->_addr << "(type: " << cur_ptr->_data_type << ")" << endl;
			cout << "  ┠ Byte Size: " << cur_ptr->_sz_byte << "(" << cur_ptr->_data_type << "*" << cur_ptr->_cnt_element << ")" "Byte" << endl;
			cout << "  ┠ File: " << cur_ptr->file_name_free << "(con: " << cur_ptr->_file_name << ")" << endl;
			cout << "  ┗ Line: " << cur_ptr->line_free << "(con: " << cur_ptr->_line << ")" << endl;
			cout << "  ┗ Share: " << ((cur_ptr->_sz_byte / _total_sz_byte_malloc)) * 100 << "%" << endl << endl;
		
		
		}

		cout << "===========================================================" << endl << endl;
	}


	//------------------------------------------------------------------------------
	//	통계 출력
	//------------------------------------------------------------------------------
	void DisplayAllStatics(){

		int cnt_malloc = _list_malloc.size();
		int cnt_free = _list_free.size();

		


		//총 통계1 
		cout << "[Total Statics]" << endl;
		cout << "	[Total Malloc]" << endl;
		cout << "	┠ Size: " << _total_sz_byte_malloc << "Byte" << endl;
		cout << "	┗ Count: " << _total_cnt_malloc << endl << endl;


		cout << "	[Total Free]" << endl;
		cout << "	┠ Size: " << _total_sz_byte_free << "Byte" << endl;
		cout << "	┗ Count: " << _total_cnt_delete << endl << endl;

		cout << "===========================================================" << endl << endl;

		cout << "[Malloc Statics]" << endl;
		cout << "	┠ Size: " << _total_sz_byte_malloc - _total_sz_byte_free << "Byte" << endl;
		cout << "	┗ Count: " << cnt_malloc << endl;
		cout << "	┗ Share: " << (1.0-(_total_sz_byte_free / _total_sz_byte_malloc)) * 100 << "%" << endl << endl;


		cout << "[Free Statics]" << endl;
		cout << "	┠ Size: " << _total_sz_byte_free << "Byte" << endl;
		cout << "	┗ Count: " << cnt_free << endl;
		cout << "	┗ Share: " << (_total_sz_byte_free/ _total_sz_byte_malloc) *100<<"%"<< endl << endl<<endl;


		cout << "===========================================================" << endl << endl;


		DisplayMallocStatics();
		DisplayFreeStatics();


	}


	//------------------------------------------------------------------------------
	//	메모리 생성 함수.
	//------------------------------------------------------------------------------
	template <typename DATA_TYPE>
	DATA_TYPE* _malloc(int cnt_element = 1, const char *file_name = NULL, int _line = -1)
	{

		//------------------------------------------------------------------------------
		//	생성자 선언.
		//------------------------------------------------------------------------------
		DATA_TYPE *out_ptr = new DATA_TYPE[cnt_element];

		//------------------------------------------------------------------------------
		//	생성자 리스트에 삽입 할 노드 생성.
		//------------------------------------------------------------------------------
		NODE_MALLLOC<void*> *new_node = new NODE_MALLLOC<void*>;

		//------------------------------------------------------------------------------
		//	생성자 포인터 설정.
		//------------------------------------------------------------------------------

		//주소 설정.
		new_node->_addr = out_ptr;

		//데이터 타입 설정.
		strcpy_s(new_node->_data_type, typeid(DATA_TYPE).name());

		int sz_byte_tmp = -1;

		//배열 인 경우.
		if (cnt_element > 1) {

			//배열 길이 설정.
			new_node->_cnt_element = cnt_element;

			//배열 여부 설정.
			new_node->_is_arr = true;

			//사이즈 설정.
			sz_byte_tmp = sizeof(DATA_TYPE) * cnt_element;
			new_node->_sz_byte = sz_byte_tmp;

		}

		//단일 타입인 경우.
		else{

			new_node->_cnt_element = 1;

			//배열 여부 설정
			new_node->_is_arr = false;

			//사이즈 설정
			sz_byte_tmp = sizeof(DATA_TYPE);
		}

		//총 생성 횟수 증가.
		_total_cnt_malloc++;

		//총 생성 사이즈 증가.
		_total_sz_byte_malloc += sz_byte_tmp;

		//생성 사이즈 증가.
		_sz_alloc=+ sz_byte_tmp;


		//라인 설정.
		new_node->_line = _line;

		//파일명 설정.
		new_node->_file_name = (char*)file_name;

		//생성 리스트에 삽입.
		_list_malloc.add(new_node);

		//------------------------------------------------------------------------------
		//	생성자 반환
		//------------------------------------------------------------------------------
		return out_ptr;
	}



	//------------------------------------------------------------------------------
	//	메모리 해제 함수.
	//------------------------------------------------------------------------------
	template <typename DATA_TYPE>
	bool _delete(DATA_TYPE delete_addr, const char *file_name = NULL, int _line = -1)
	{

		//생성자 리스트 순회 노드 포인터 선언
		NODE_MALLLOC<void*> *cur_ptr = NULL;

		//------------------------------------------------------------------------------
		//	생성자 리스트 순회 
		//
		//	:생성자 리스트 순회하며 delete_addr과 같은 삭제 노드가 있는지 탐색
		//------------------------------------------------------------------------------
		void *addr = NULL;	//주소 저장 보이드 포인터


		//------------------------------------------------------------------------------
		//	삭제 노드가 존재하는지 체크하는 불 타입 
		//------------------------------------------------------------------------------
		bool check_find_delele_node = false;
		//리스트 사이즈
		int num_element = -1;
		num_element = _list_malloc.size();
		for (int i = 0; i < num_element; i++)
		{
			cur_ptr = _list_malloc.get(i);
			addr = cur_ptr->_addr;

			//------------------------------------------------------------------------------
			//	삭제 노드가 생성자 리스트에 존재하는 경우 
			//
			//	:생성자 리스트에서 해당 리스트 노드를 삭제
			//	:플래그 설정
			//------------------------------------------------------------------------------
			if (addr == delete_addr)
			{
				check_find_delele_node = true;
				_list_malloc.remove(i);
				break;
			}
		}

		//------------------------------------------------------------------------------
		//	삭제 노드가 생성자 리스트에 존재하지 않는 경우
		//
		//	:못 찾음
		//------------------------------------------------------------------------------
		if (check_find_delele_node == false)
		{
			return false;
		}

		//------------------------------------------------------------------------------
		//	삭제 노드가 생성자 리스트에 존재하는 경우
		//
		//	소멸자 리스트 노드를 설정 후 소멸자 리스트에 삽입
		//------------------------------------------------------------------------------
		else if (check_find_delele_node == true)
		{

			NODE_FREE<void*> *release_ptr = new NODE_FREE<void*>;

			//소멸자 포인터
			release_ptr->_ptr = cur_ptr->_ptr;

			//주소 값
			release_ptr->_addr = cur_ptr->_addr;

			//바이트 크기
			int sz_byte_tmp = -1;
			sz_byte_tmp = cur_ptr->_sz_byte;
			release_ptr->_sz_byte = sz_byte_tmp;


			//총 삭제 사이즈 크기 증가
			//총 생성 횟수 증가
			_total_cnt_delete++;
			_total_sz_byte_free += sz_byte_tmp;

			//생성 사이즈 감소
			_sz_alloc -= sz_byte_tmp;


			//데이터 타입.
			strcpy_s(release_ptr->_data_type, cur_ptr->_data_type);

			//배열 유무.
			release_ptr->_is_arr = cur_ptr->_is_arr;

			//배열 길이.
			release_ptr->_cnt_element = cur_ptr->_cnt_element;

			//생성자 함수 호출 파일 위치 및 생성 코드.
			release_ptr->_file_name = cur_ptr->_file_name;
			release_ptr->_line = cur_ptr->_line; //생성 코드 라인.

			//소멸자 함수 호출 파일 위치 및 생성 코드.
			release_ptr->line_free = _line; //해제 코드 라인
			release_ptr->file_name_free = (char*)file_name;

			//소멸자 리스트에 삽입.
			_list_free.add(release_ptr);

			//소멸자 해제.
			delete(delete_addr);

			return true;

		}

	}
};

static MemoryLeak g_MemoryLeak;







