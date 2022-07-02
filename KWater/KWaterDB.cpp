#include "pch.h"
#include "KWaterDB.h"

KWaterDB::KWaterDB() : CWnd()
	,connection(nullptr)
{
}

KWaterDB::~KWaterDB()
{
}

VOID KWaterDB::Finish_with_error(MYSQL* con)
{
	AfxMessageBox((CString)mysql_error(con));
	mysql_close(con);
}

BOOL KWaterDB::ConnectToDB(MYSQL* mysql)
{
	mysql_init(mysql);
	return (BOOL)mysql_real_connect(mysql, "localhost", "root", "P@ssw0rd!", "kwatersedimentpredict", 3306, NULL, 0);
}

char* KWaterDB::CStringToChar(CString csting)
{
	wchar_t* wQuery = LPWSTR(LPCWSTR(csting));
	int strSize(WideCharToMultiByte(CP_ACP, 0, wQuery, -1, NULL, 0, NULL, NULL));
	char* cQuery = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, wQuery, -1, cQuery, strSize, 0, 0);

	return cQuery;
}

VOID KWaterDB::FreeSQL(MYSQL_RES* res)
{
	mysql_free_result(res);
	mysql_close(connection);
}

CDamTable::CDamTable()
	: KWaterDB()
	, m_cstTablename(L"Dam")
	, m_nFieldsCnt(0)
{
}

CDamTable::~CDamTable()
{
}

BOOL CDamTable::ConnectToDB()
{
	connection = mysql_init(NULL);
	return (BOOL)mysql_real_connect(GetConnection(), DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0);
}

void CDamTable::GetColumns()
{
	mysql_query(connection, "set names euckr");
	if (mysql_query(GetConnection(), "SELECT * FROM Dam"))
		Finish_with_error(GetConnection());
	MYSQL_RES* result = mysql_store_result(GetConnection());
	GetFieldsNum(result);

	MYSQL_ROW row; 
	while (row = mysql_fetch_row(result)) 
	{ 
		if (row[0] == m_cstFindDam)
		{
			for (int i = 0; i < m_nFieldsCnt; i++)
			{
				m_cstDamName = row[1];
				m_cstArea = row[4];
				m_cstLength = row[3];
				m_cstHeight = row[2];
				::SendMessage(m_ParentHwnd, MSG_UPDATE,0,0);
			}
		}
	}
	FreeSQL(result);
}

VOID CDamTable::GetFieldsNum(MYSQL_RES* res)
{
	if (res == NULL)
		Finish_with_error(GetConnection());
	m_nFieldsCnt = mysql_num_fields(res);
}

BOOL CDamTable::Insert(CString cstDamName, FLOAT fDamLength, FLOAT fDamHigh, FLOAT fBasinArea, FLOAT fFW, FLOAT FWL) 
{
	CString Insertcode;
	Insertcode.Format(_T("INSERT INTO %s (`damName`, `damLength`, `damHigh`, `basinArea`, `fullWaterLevel`, `floodWaterLevel`) values(%s ,%f, %f, %f, %f, %f)"),
		m_cstTablename, fDamLength, fDamHigh, fBasinArea, fFW, FWL);

	INT retVal(mysql_query(GetConnection(), CStringToChar(Insertcode)));
	return !retVal;
}