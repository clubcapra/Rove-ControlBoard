#include "ErrorManager.h"
#include "adapterCBRove.h"

ErrorCode ErrorManagerClass::getCurrentError()
{
    if (mTryDepth == 0) return AdapterCBRove.getErrorCode();
    return mThrown[mTryDepth-1];
}

void ErrorManagerClass::clearCurrentError()
{
    mThrown[mTryDepth] = ErrorCode::ERNone;
}

void ErrorManagerClass::addCatch()
{
    ++mCatchCounts[mTryDepth];
}

void ErrorManagerClass::removeCatch()
{
    int c = --mCatchCounts[mTryDepth];
    if (c == 0)
    {
        if (mTryDepth == 0)
        {
            error(mThrown[0]);
            return;
        }
        mThrown[mTryDepth] = mThrown[mTryDepth+1];
    }
}

int ErrorManagerClass::addTry()
{
    return mTryDepth++;
}

void ErrorManagerClass::removeTry()
{
    --mTryDepth;
}

void ErrorManagerClass::error(ErrorCode errorCode)
{
    if (mTryDepth == 0) AdapterCBRove.setErrorCode(errorCode);
    else
    {
        mThrown[mTryDepth-1] = errorCode;
    }
}

ErrorManagerClass ErrorManager{};