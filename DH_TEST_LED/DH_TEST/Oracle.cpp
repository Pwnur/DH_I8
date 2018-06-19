#include "Oracle.h"


BOOL Connect ()
{
	if (m_ConnectState == TRUE)
		return TRUE;

	m_OracleEnv = Environment::createEnvironment(Environment::DEFAULT);
	if (!m_OracleEnv)
	{
		return FALSE;
	}
	m_OracleConnect = m_OracleEnv->createConnection("apical", "apical", "218.16.56.108:1521/orcl");
	if (!m_OracleConnect)
	{
		return FALSE;
	}

	m_ConnectState = TRUE;
	return TRUE;
}

BOOL DoSql (CString Sql)
{
	Statement *Stmt = NULL;
	ResultSet *Rs = NULL;

	Stmt = m_OracleConnect->createStatement();
	if (!Stmt)
	{
		AfxMessageBox ("createStatement error");
		return FALSE;
	}
	
	Stmt->setSQL (Sql.GetBuffer ());
	Stmt->executeUpdate ();
	Stmt->execute ("commit");

	m_OracleConnect->terminateStatement(Stmt);
	
	return TRUE;
}

BOOL GetDate (CString &mDate, CString &mTime)
{
	CString Sql;
	Statement *Stmt = NULL;
	ResultSet *Rs = NULL;

	Sql.Format ("select to_char(sysdate,'yyyymmdd') as dbdate,to_char(sysdate,'hh24miss')  as dbtime from dual where '1' = '1'");
	Stmt = m_OracleConnect->createStatement(Sql.GetBuffer ());
	if (!Stmt)
	{
		AfxMessageBox ("createStatement error");
		return FALSE;
	}
	
	Stmt->setPrefetchRowCount(32);
	Rs = Stmt->executeQuery();

	
	if(Rs)
	{
		while (Rs->next()) 
		{
			static string str1;
			str1 = Rs->getString(1);
			mDate = str1.c_str();

			static string str2;
			str2 = Rs->getString(2);
			mTime = str2.c_str();
			
			Stmt->closeResultSet(Rs);  	
			m_OracleConnect->terminateStatement(Stmt);
			return TRUE;
		}
	}
	else
	{
		m_OracleConnect->terminateStatement(Stmt);
		return FALSE;
	}
	
	
	return FALSE;
}

BOOL ExitMes (CString iUserCode)
{
	CString Sql;
	Statement *Stmt = NULL;
	ResultSet *Rs = NULL;

	Sql.Format ("select * from TBLRES where RESCODE='%s'", iUserCode.GetBuffer ());
	Stmt = m_OracleConnect->createStatement(Sql.GetBuffer ());
	if (!Stmt)
	{
		AfxMessageBox ("createStatement error");
		return FALSE;
	}
	
	Stmt->setPrefetchRowCount(32);
	Rs = Stmt->executeQuery();

	static string RESCODE;
	static string RESDESC;
	static string RESGROUP;
	static string RESTYPE;
	static string SEGCODE;
	static string SSCODE;
	static string SHIFTTYPECODE;
	static string MUSER;
	static int MDATE;
	static int MTIME;
	static string EATTRIBUTE1;
	static string REWORKROUTECODE;
	static int ORGID;
	static string REWORKCODE;
	static string DCTCODE;
	static string CREWCODE;
	static string COMPUTERNAME;
	static string COMPUTERMAC;
	static string COMPUTERIP;
	static string JIGCONTROL;

	
	if(Rs)
	{
		while (Rs->next()) 
		{
			RESCODE = Rs->getString(1);
			RESDESC = Rs->getString(2);
			RESGROUP = Rs->getString(3);
			RESTYPE = Rs->getString(4);
			SEGCODE = Rs->getString(5);
			SSCODE = Rs->getString(6);
			SHIFTTYPECODE = Rs->getString(7);
			MUSER = Rs->getString(8);
			MDATE = Rs->getInt(9);
			MTIME = Rs->getInt(10);
			EATTRIBUTE1 = Rs->getString(11);
			REWORKROUTECODE = Rs->getString(12);
			ORGID = Rs->getInt(13);
			REWORKCODE = Rs->getString(14);
			DCTCODE = Rs->getString(15);
			CREWCODE = Rs->getString(16);
			COMPUTERNAME = Rs->getString(17);
			COMPUTERMAC = Rs->getString(18);
			COMPUTERIP = Rs->getString(19);
			JIGCONTROL = Rs->getString(20);
			
			Stmt->closeResultSet(Rs);  	
			m_OracleConnect->terminateStatement(Stmt);

			CString mDate, mTime;
			GetDate (mDate, mTime);
			
			CString UpdateSql;
			UpdateSql.Format ("UPDATE TBLRES SET segcode='%s',jigcontrol='%s',orgid='%d',muser='%s',computermac='',computername='',crewcode='',resdesc='%s',sscode='%s',resgroup='',mtime='%s',computerip='',restype='%s',shifttypecode='%s',eattribute1='',reworkcode='',dctcode='',mdate='%s',reworkroutecode='' WHERE rescode='%s'", SEGCODE.c_str(), JIGCONTROL.c_str(), ORGID, MUSER.c_str(), RESDESC.c_str(), SSCODE.c_str(), mTime.GetBuffer(), RESTYPE.c_str(), SHIFTTYPECODE.c_str(), mDate.GetBuffer(), RESCODE.c_str());
			
//			AfxMessageBox (UpdateSql);

			DoSql (UpdateSql);
			
			return TRUE;
		}
	}
	else
	{
		m_OracleConnect->terminateStatement(Stmt);
		return FALSE;
	}
	
	
	return FALSE;
}
BOOL DBDLL::EXITMES (CString iResCode)
{
	BOOL b;
	
	b = g_pOracle->Connect ();
	if (!b)
	{
		AfxMessageBox ("Connect Oracle error\n");
		return FALSE;
	}
	
	b = g_pOracle->ExitMes (iResCode);
	if (!b)
	{
		return FALSE;
	}

	return TRUE;
}


