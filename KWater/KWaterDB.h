#include "pch.h"
#include "MySQLDefinition.h"

#pragma once
class KWaterDB : public CWnd
{
public:
	KWaterDB();
	virtual ~KWaterDB();

public:

public:

	BOOL					ConnectToDB(MYSQL* mysql);

	CString					m_cstSchema;

	VOID						Finish_with_error(MYSQL* con);
	VOID						FreeSQL(MYSQL_RES* res);

	char*						CStringToChar(CString csting);
};

class CDamTable : public KWaterDB
{
public:
	CDamTable();
	virtual ~CDamTable();

protected:
	MYSQL* connection;
	HWND					m_ParentHwnd;

	CString					m_cstTablename;
	CString					m_cstDamName;
	CString					m_cstArea;
	CString					m_cstLength;
	CString					m_cstHeight;
	
	INT						m_nFieldsCnt;

public:
	MYSQL* GetConnection() { return connection; };
	CString					m_cstFindDam;

	//Getter
	void						GetColumns();
	void						GetFieldsNum(MYSQL_RES* res);
	CString					GetName() { return m_cstDamName;	}
	CString					GetArea() { return m_cstArea;	}
	CString					GetLength() { return m_cstLength;	}
	CString					GetHeight() { return m_cstHeight;	}

	//Setter
	void						SetHwnd(HWND hwnd) { m_ParentHwnd = hwnd; }
	void						SetFindDamcode(CString Dam) { m_cstFindDam = Dam; };
	virtual BOOL			ConnectToDB();
	BOOL					Insert(CString cstDamName, FLOAT fDamLength, FLOAT fDamHigh, FLOAT fBasinArea, FLOAT fFW, FLOAT FWL);
	

};

class CPredictionsoilerosionTable : public KWaterDB
{
public:
	CPredictionsoilerosionTable();
	virtual ~CPredictionsoilerosionTable();
protected:
	MYSQL* connection;
	CString					m_cstTablename;
public:
	MYSQL* GetConnection() { return connection; };
	virtual BOOL			ConnectToDB();
	BOOL					Insert(INT nDam, INT nYear, INT nMonth, FLOAT fTPrecip, FLOAT fRfIntensity, FLOAT fRfSoil, FLOAT fRfSoil_Intensity, FLOAT fSunshine, FLOAT eva);
};