#pragma once



/// 커스텀하게 힙에서 할당 받는 애들은 전부 메모리 정보 붙여주기
__declspec(align(MEMORY_ALLOCATION_ALIGNMENT))
struct MemAllocInfo : SLIST_ENTRY
{
	MemAllocInfo(int size) : mAllocSize(size), mExtraInfo(-1)
	{}
	
	long mAllocSize; ///< MemAllocInfo가 포함된 크기
	long mExtraInfo; ///< 기타 추가 정보 (예: 타입 관련 정보 등)

}; ///< total 16 바이트

inline void* AttachMemAllocInfo(MemAllocInfo* header, int size)
{
	//TODO: header에 MemAllocInfo를 펼친 다음에 실제 앱에서 사용할 메모리 주소를 void*로 리턴... 실제 사용되는 예 및 DetachMemAllocInfo 참고.
	return 0;
}

inline MemAllocInfo* DetachMemAllocInfo(void* ptr)
{
	MemAllocInfo* header = reinterpret_cast<MemAllocInfo*>(ptr);
	--header;
	return header;
}

__declspec(align(MEMORY_ALLOCATION_ALIGNMENT))
class SmallSizeMemoryPool
{
public:
	SmallSizeMemoryPool(DWORD allocSize);

	MemAllocInfo* Pop();
	void Push(MemAllocInfo* ptr);
	

private:
	SLIST_HEADER mFreeList; ///< 반드시 첫번째 위치

	const DWORD mAllocSize;
	volatile long mAllocCount = 0;
};

class MemoryPool
{
public:
	MemoryPool();

	void* Allocate(int size);
	void Deallocate(void* ptr, long extraInfo);

private:
	enum Config
	{
		/// 함부로 바꾸면 안됨. 철저히 계산후 바꿀 것
		MAX_SMALL_POOL_COUNT = 1024/32 + 1024/128 + 2048/256, ///< ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		MAX_ALLOC_SIZE = 4096
	};

	/// 원하는 크기의 메모리를 가지고 있는 풀에 O(1) access를 위한 테이블
	SmallSizeMemoryPool* mSmallSizeMemoryPoolTable[MAX_ALLOC_SIZE+1];

};

extern MemoryPool* GMemoryPool;


/// 요놈을 상속 받아야만 xnew/xdelete 사용할 수 있게...
struct PooledAllocatable {};


template <class T, class... Args>
T* xnew(Args... arg)
{
	static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "only allowed when PooledAllocatable");

	//TODO: T* obj = xnew<T>(...); 처럼 사용할 수있도록 메모리풀에서 할당하고 생성자 불러주고 리턴.

	void* alloc = nullptr; 
	
	//TODO: ... ...

	return reinterpret_cast<T*>(alloc);
}

template <class T>
void xdelete(T* object)
{
	static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "only allowed when PooledAllocatable");

	//TODO: object의 소멸자 불러주고 메모리풀에 반납.
	
}