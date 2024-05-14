#if !defined(ERROR_MANAGER_H)
#define ERROR_MANAGER_H

#include "api.h"

class AdapterCBRoveClass;
struct TryBlock;
struct CatchBlock;

#define MAX_TRY_DEPTH 5

class ErrorManagerClass
{
private:
    int mTryDepth = 0;
    int mCatchCounts[MAX_TRY_DEPTH] = {0};
    ErrorCode mThrown[MAX_TRY_DEPTH] = {ErrorCode::ERNone};

    ErrorCode getCurrentError();
    void clearCurrentError();
    void addCatch();
    void removeCatch();
    int addTry();
    void removeTry();
public:
    void error(ErrorCode errorCode);
    
    friend AdapterCBRoveClass;
    friend TryBlock;
    friend CatchBlock;
};

extern ErrorManagerClass ErrorManager;

struct TryBlock
{
    int depth;
    bool init = false;
    TryBlock()
    {
        depth = ErrorManager.addTry();
    }

    operator bool()
    {
        bool res = init;
        init = true;
        return res;
    }

    ~TryBlock()
    {
        ErrorManager.removeTry();
    }
};

struct CatchBlock
{
    bool init = false;
    ErrorCode errorCode;
    CatchBlock(ErrorCode err)
    {
        errorCode = err;
    }
    
    operator bool()
    {
        if (ErrorManager.getCurrentError() != errorCode) return false;
        bool res = init;
        init = false;
        return res;
    }

    ~CatchBlock()
    {
        if (ErrorManager.getCurrentError() == errorCode) ErrorManager.clearCurrentError();
        ErrorManager.removeCatch();
    }
};

#define TRY for (TryBlock __CONCAT(_TB_,__LINE__){}; __CONCAT(_TB_,__LINE__);)
// #define THROW(err) {ErrorManager.error(err); break;}
#define THROW(err) {AdapterCBRove.setErrorCode(err); break;}
// #define THROW_RETURN(err, returnValue) {ErrorManager.error(err); return returnValue;}
#define THROW_RETURN(err, returnValue) {AdapterCBRove.setErrorCode(err); return returnValue;}
#define CATCH(err) ErrorManager.addCatch(); for (CatchBlock __CONCAT(_CB_,__LINE__){err}; __CONCAT(_CB_,__LINE__);)

// Use case:
/*
bool innerFunc()
{
    bool locallyUnhandledFail = true;
    if (locallyUnhandledFail)
    {
        // This throws and exits
        THROW_RETURN(ErrorCode::ERAdapterNotInit, false);
    }
    return true;
}

void otherFunc()
{
    TRY
    {
        // Try blocks can be stacked either in a same function or in inner functions
        TRY
        {
            if (!innerFunc()) break; // break will skip to the catch blocks

            // Throwing locally will also skip to the catch blocks
            if (true)
            {
                THROW(ErrorCode::ERServoNACK);
            }
            else
            {
                THROW(ErrorCode::ERRTXBufferOvervlow);
            }
        }
        CATCH(ErrorCode::ERAdapterNotInit)
        {
            // This only gets called if ERAdapterNotInit was thrown
        }
        CATCH(ErrorCode::ERServoNACK)
        {
            // This only gets called if ERServoNACK was thrown
        }

        
    }
    CATCH(ErrorCode::ERRTXBufferOvervlow)
    {
        // This only gets called if ERRTXBufferOvervlow was thrown and not caught by an inner catch block
    }
}
// */
#endif // ERROR_MANAGER_H
