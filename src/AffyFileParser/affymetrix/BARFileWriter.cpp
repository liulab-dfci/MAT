/////////////////////////////////////////////////////////////////
//
// Copyright (C) 2005 Affymetrix, Inc.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2.1 of the License,
// or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
//
/////////////////////////////////////////////////////////////////

#include "BARFileWriter.h"
#include "FileWriter.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <istream>
#include <fstream>
#include <cstring>		// port to gcc 4.3, by Tao Liu
#include <climits>		// port to gcc 4.3, by Tao Liu
#include <cstdlib>		// port to gcc 4.3, by Tao Liu
#include <cstdio>		// port to gcc 4.3, by Tao Liu
#include <cctype>		// port to gcc 4.3, by Tao Liu
#include <algorithm>		// port to gcc 4.3, by Tao Liu
#include <iterator>		// port to gcc 4.3, by Tao Liu
#include <memory>		// port to gcc 4.3, by Tao Liu
#include <typeinfo>		// port to gcc 4.3, by Tao Liu

#ifdef _INCLUDE_UNISTD_HEADER_
#include <unistd.h>
#endif

#pragma warning(disable: 4996) // don't show deprecated warnings.

//////////////////////////////////////////////////////////////////////

#define BAR_VERSION 2.0f

//////////////////////////////////////////////////////////////////////

using namespace affxbar;
using namespace affxbarwriter;

//////////////////////////////////////////////////////////////////////

CBARFileWriter::CBARFileWriter() : CBARFileData()
{
}

//////////////////////////////////////////////////////////////////////

CBARFileWriter::~CBARFileWriter()
{
	Close();
}

//////////////////////////////////////////////////////////////////////

bool CBARFileWriter::CreateNewFile()
{
	m_Version = 1.0f;

	// Open the file.
	m_strError = "";
	m_NewBarFile.open(m_FileName.c_str(), std::ios::out | std::ios::binary);
	if (!m_NewBarFile)
	{
		m_strError = "Unable to open the file.";
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

bool CBARFileWriter::Save()
{
	// Magic number
	char magic[9];
	sprintf(magic, "barr%c%c%c%c", '\r', '\n', '\032', '\n');
	WriteFixedString(m_NewBarFile, std::string(magic), 8);

	// Version
	WriteFloat(m_NewBarFile, BAR_VERSION);

	// Number of sequendes
	WriteInt(m_NewBarFile, m_NumberSequences);

	// Columns
	int i = 0;
	WriteInt(m_NewBarFile, m_NumberColumns);
	for (i=0; i<m_NumberColumns; i++)
		WriteInt(m_NewBarFile, (int)m_ColumnTypes[i]);

	// Parameters
	WriteInt(m_NewBarFile, m_NumberParameters);
	for (i=0; i<m_NumberParameters; i++)
	{
		WriteString_N(m_NewBarFile, m_Parameters[i].Tag);
		WriteString_N(m_NewBarFile, m_Parameters[i].Value);
	}

	// Results
	for (i=0; i<m_NumberSequences; i++)
	{
		WriteString_N(m_NewBarFile, m_Results[i].GetName());
		WriteString_N(m_NewBarFile, m_Results[i].GetGroupName());
		WriteString_N(m_NewBarFile, m_Results[i].GetVersion());
		WriteInt(m_NewBarFile, m_Results[i].GetNumberParameters());
		for (int iParam=0; iParam<m_Results[i].GetNumberParameters(); iParam++)
		{
			WriteString_N(m_NewBarFile, m_Results[i].GetParameter(iParam).Tag);
			WriteString_N(m_NewBarFile, m_Results[i].GetParameter(iParam).Value);
		}
		WriteInt(m_NewBarFile, m_Results[i].GetNumberDataPoints());
		BarSequenceResultData data;
		for (int j=0; j<m_Results[i].GetNumberDataPoints(); j++)
		{
			for (int k=0; k<GetNumberColumns(); k++)
			{
				m_Results[i].GetData(j,k,data);
				if (m_ColumnTypes[k] == BAR_DATA_INTEGER)
					WriteInt(m_NewBarFile, data.iValue);
				else
					WriteFloat(m_NewBarFile, data.fValue);
			}
		}
	}

	// Save the footer
	WriteFixedString(m_NewBarFile, "END\n", 4);

	// Close the file
	m_NewBarFile.close();

	return !m_NewBarFile.fail();
}

//////////////////////////////////////////////////////////////////////

