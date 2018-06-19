#define DB_METHOD_EXPORTS
#ifdef DB_METHOD_EXPORTS
# define DLLExport _declspec(dllexport)
#else
# define DLLExport _declspec(dllimport)
#endif
#include <afxmt.h>
#include "stdafx.h"

class  DLLExport DBDLL
{
private:
	DBDLL ();
	DBDLL (const DBDLL &);  
    DBDLL & operator = (const DBDLL &);  
public:
	static DBDLL &GetInstance();
	~DBDLL ();

	BOOL SN2UUID (CString MOCODE, CString OPCODE, CString SN, CString &UUID);
	BOOL EXITMES (CString iResCode);
	BOOL INSERTDATA (CString SN,CString UUIN,CString CODE,CString T1 = NULL);
	BOOL DELETEDATA (CString SN,CString UUIN,CString CODE,CString T1 = NULL);
	BOOL SN2CODE (CString SN,CString &CODE);
	BOOL PALLET2SN (CString PALLET,CString SN);
	BOOL CARTONCODE2RCARD (CString CARTONCODE,CString &RCARD);
};
