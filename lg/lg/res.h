
extern  const GUID  IID_IRes;
interface IRes : IUnknown
{
	virtual void* Lock(void) = 0;
	virtual void Unlock(void) = 0;
};
/*
int ExtractPartial(long,long,void*)
int HasSetData(void)
unsigned int GetLockCount(void)
unsigned long GetManData(void)
IStore* GetStore(void)
void* LoadData(unsigned long*, unsigned long*, IResMemOverride*)
void* Extract(void*)
void* LoadTranslation(void*, long,const char*,int*,unsigned long*,IResMemOverride*)
void* DataPeek(void)
void* PreLoad(void)
const char* GetName(void)
void GetVariant(char**)
void SetCanonStore(IStore*)
void SetName(const char*)
void Unlock(void)
int AsyncLock(int)
int AsyncExtract(int, void*, long)
int SetStore(IStore*)
int FreeData(void*,long,IResMemOverride*)
int AsyncPreload(void)
int Drop(void)
int IsAsyncFulfilled(void)
long GetAsyncResult(void**)
long GetSize(void)
long AsyncKill(void)
unsigned long GetAppData(void)
char** GetTranslatableTypes(int*)
IResType* GetType(void)
IStore* GetcanonStore(void)
IStoreStream* OpenStream(void)
void* Lock(void)
void AllowStorageReset(int)
void GetStreamName(int,char**)
void SetAppData(unsigned long)
void SetManData(unsigned long)
void GetCanonPathName(char**)
void GetCanonPath(char**)
void SetData(void*)
void ExtractBlocks(void*, long, long (*)(IRes*,void*,long,long,void*),void*)
*/
extern  const GUID  IID_IResControl;
extern  const GUID  IID_IResHack;
extern  const GUID  IID_IResMem;
extern  const GUID  IID_IResMemOverride;
extern  const GUID  IID_IResStats;
extern  const GUID  IID_IResType;
interface IResType : IUnknown
{
	virtual char const * GetName(void) = 0;
	virtual void EnumerateExts(void (__cdecl *)(char const *, IResType *, void *), void *) = 0;
	virtual int IsLegalExt(char const *) = 0;
	virtual IRes * CreateRes(IStore *, char const *, char const *, IResMemOverride **) = 0;
};

extern  const GUID  IID_ISearchPath;
interface ISearchPath : IUnknown
{
	STDMETHOD_(void,Clear)(void) PURE;
	STDMETHOD_(ISearchPath*,Copy)(void) PURE;
	STDMETHOD_(void,AddPath)(const char *) PURE;
	STDMETHOD_(void,AddPathTrees)(const char *,int) PURE;
	STDMETHOD_(void,Ready)(void) PURE;
};
/*
STDMETHOD_(int,Next)(void*,IStore**,char*,char*) PURE;
STDMETHOD_(IStore*,Find)(const char*,unsigned int,IStore**,const char*) PURE;
STDMETHOD_(void*,BeginContents)(const char*,unsigned int,const char*) PURE;
STDMETHOD_(void,Iterate)(void (__cdecl *)(ISearchPath*,const char*,int,void*)int,void*) PURE;
STDMETHOD_(void,SetContext)(ISearchPath*) PURE;
STDMETHOD_(void,SetVariants)(ISearchPath*) PURE;
STDMETHOD_(void,EndContents)(void*) PURE;
*/

extern  const GUID  IID_IStringRes ;
