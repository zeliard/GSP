#include "stdafx.h"
#include "CircularBuffer.h"
#include <assert.h>




void CircularBuffer::Remove(size_t len)
{
	size_t cnt = len ;
	
	/// Read와 마찬가지로 A가 있다면 A영역에서 먼저 삭제

	if ( mARegionSize > 0 )
	{
		size_t aRemove = (cnt > mARegionSize) ? mARegionSize : cnt ;
		mARegionSize -= aRemove ;
		mARegionPointer += aRemove ;
		cnt -= aRemove ;
	}

	// 제거할 용량이 더 남은경우 B에서 제거 
	if ( cnt > 0 && mBRegionSize > 0 )
	{
		size_t bRemove = (cnt > mBRegionSize) ? mBRegionSize : cnt ;
		mBRegionSize -= bRemove ;
		mBRegionPointer += bRemove ;
		cnt -= bRemove ;
	}

	/// A영역이 비워지면 B를 A로 스위치 
	if ( mARegionSize == 0 )
	{
		if ( mBRegionSize > 0 )
		{
			/// 앞으로 당겨 붙이기
			if ( mBRegionPointer != mBuffer )
				memmove(mBuffer, mBRegionPointer, mBRegionSize) ;
	
			mARegionPointer = mBuffer ;
			mARegionSize = mBRegionSize ;
			mBRegionPointer = nullptr ;
			mBRegionSize = 0 ;
		}
		else
		{
			mBRegionPointer = nullptr ;
			mBRegionSize = 0 ;
			mARegionPointer = mBuffer ;
			mARegionSize = 0 ;
		}
	}
}


