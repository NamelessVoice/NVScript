#include "lg/objstd.h"
#include "lg/interface.h"
#include "lg/types.h"
#include "lg/scrmsgs.h"

extern IMalloc* g_pMalloc;


/* 
 * So just what does sPersistent do anyway, and why do we need it?
 * 
 * The sPersistent class (not related to the OLE IPersist class) allows 
 * class data to be streamed out to a file (or some file-like object)
 * and later streamed back in. A class that needs to be streamed implements 
 * sPersist::Persistence. When called, it is supposed to call a 
 * static function pointer (gm_pfnIO) with a static context pointer (gm_pContextIO)
 * and the address and size of the data that will be read or written. 
 * Another static flag (gm_fReading) tells which way the data is going, 
 * though for simple structures it's not necessary differentiate.
 *
 * Those static members are what makes it impossible (or, at least, very-very difficult)
 * to implement sPersistent from a script. 
 * 
 * But where does Thief/SShock use sPersist? I can't actually find where script
 * messages are streamed to, if at all. Which makes me wonder if the persistence 
 * of sScrMsg is a feature that was never actually used. If this is indeed the case,
 * then it doesn't matter that our sPersist implementation is a no-op.
 *
 * If the persistence is being used, then the most obvious places are when a SAV 
 * database is being written (but, as I said, I've never seen message data in a SAV),
 * or when the sim is interrupted, such as a missloop state change (i.e. book mode).
 * 
 * The most telling thing I can think of is that when I create my own script messages,
 * they appear to work just fine. 
 */

#ifndef _MSC_VER
////////////////////
// sPersistent hack
////////////////////

/* Thankfully, none of these have arguments.
 * Otherwise, it'd be a hell of a lot more complicated
 * to deal with the stack.
 */
static int hackPersistent_Persistence()
{
	register sScrMsg* msg;
#ifdef __GNUC__
	asm ("movl %%ecx,%0\n\tsubl $0x8,%0" : "=r" (msg) : : "ecx" );
#else
	/* I need TASM32
	asm {
		mov	msg,ecx
		sub	msg,8
	}
	*/
	return 0;
#endif
	return msg->Persistence();
}

static const char* hackPersistent_GetName()
{
	register sScrMsg* msg;
#ifdef __GNUC__
	asm ("movl %%ecx,%0\n\tsubl $0x8,%0" : "=r" (msg) : : "ecx" );
#else
	/*
	asm {
		mov	msg,ecx
		sub	msg,8
	}
	*/
	return NULL;
#endif
	return msg->GetName();
}

static void hackPersistent_dtor(void*)
{
	// Hopefully, this will never actually be called.
}

static sPersistent_vtable hackPersistent_vtable = { 
	hackPersistent_dtor,
	hackPersistent_Persistence,
	hackPersistent_GetName
};
#endif


///////////////
// sScrMsgBase
///////////////
HRESULT __stdcall sScrMsgBase::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
		*ppv = (IUnknown*)this;
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
ULONG __stdcall sScrMsgBase::AddRef()
{
	return InterlockedIncrement((LONG*)&m_iRef);
}
ULONG __stdcall sScrMsgBase::Release()
{
	ULONG uRefCnt = InterlockedDecrement((LONG*)&m_iRef);
	if (uRefCnt == 0)
		delete this;
	return uRefCnt;
}


///////////
// sScrMsg
///////////

sScrMsg::~sScrMsg()
{
}

sScrMsg::sScrMsg() {
	m_iRef = 1;
	// Make sure everything is initialized
#ifndef _MSC_VER
	persistent_hack = &hackPersistent_vtable;
#endif
	from = 0;
	to = 0;
	message = NULL;
	time = 0;
	flags = 0;
	data.type = kMT_Undef;
	data2.type = kMT_Undef;
	data3.type = kMT_Undef;
}

#ifndef _MSC_VER
int sScrMsg::Persistent_Persistence(void)
{
	register int retval;
#ifdef __GNUC__
	asm (
		"\txor  %0,%0\n"
		"\tmovl %1,%%ecx\n"
		"\taddl $0x8,%%ecx\n"
		"\tmovl (%%ecx),%%edx\n"
		"\ttest %%edx,%%edx\n"
		"\tje   l1\n"
		"\tcmp  $0xCCCCCCCC,%%edx\n"
		"\tje   l1\n"
		"\tcall *0x4(%%edx)\n"
	 "l1:\t\n"
		: "=a" (retval) : "g" (this) : "ecx","edx" );
#else
	/*
	asm {
		xor eax,eax
		mov ecx,this
		add ecx,8
		mov edx,dword ptr [ecx]
		test edx,edx
		je   l1
		cmp  edx,0xCCCCCCCC
		je   l1
		call dword ptr [edx+4]
		move retval,eax
	l1:
	}
	*/
	return 0;
#endif
	return retval;
}

const char* sScrMsg::Persistent_GetName(void) const 
{
	register const char* retval;
#ifdef __GNUC__
	// bleh... GCC is tossing the retval initialization
	asm (
		"\tmovl %2,%0\n"
		"\tmovl %1,%%ecx\n"
		"\taddl $0x8,%%ecx\n"
		"\tmovl (%%ecx),%%edx\n"
		"\ttest %%edx,%%edx\n"
		"\tje   l2\n"
		"\tcmp  $0xCCCCCCCC,%%edx\n"
		"\tje   l2\n"
		"\tcall *0x8(%%edx)\n"
	 "l2:\t\n"
		: "=a" (retval) : "g" (this) , "g" ("") : "ecx","edx" );
#else
	/*
	retval = "";
	asm {
		mov ecx,this
		add ecx,8
		mov edx,dword ptr [ecx]
		test edx,edx
		je   l2
		cmp  edx,0xCCCCCCCC
		je   l2
		call dword ptr [edx+8]
		move retval,eax
	l2:
	}
	*/
	return NULL;
#endif
	return retval;
}
#endif

