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

#if !defined(AFX_CHPFILEWRITER_H__A16FCA8B_7B8D_4071_81CD_F6B2BB61A169__INCLUDED_)
#define AFX_CHPFILEWRITER_H__A16FCA8B_7B8D_4071_81CD_F6B2BB61A169__INCLUDED_

/*! \file CHPFileWriter.h This file provides CHP file writing capabilities.
 */

//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma warning(disable: 4786) // identifier was truncated in the debug information
#endif

//////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include "CHPFileData.h"
#include "CDFFileData.h"

//////////////////////////////////////////////////////////////////////

namespace affxchpwriter
{

//////////////////////////////////////////////////////////////////////

/*! This class is used for writing CHP files. */
class CCHPFileWriter : public affxchp::CCHPFileData
{
public:
	/*! Constructor */
	CCHPFileWriter();

	/*! Destructor */
	~CCHPFileWriter();

protected:
	/*! The output file stream object */
	std::ofstream m_NewChpFile;

public:
	/*! Creates the new file (empty) given the file name.
	 * @return True if successful
	 */
	bool CreateNewFile();

	/*! Allocates memory for the results objects and prepares the object for writing.
	 * @param cdf The associated CDF file.
	 */
	void InitializeForWriting(affxcdf::CCDFFileData &cdf);

	/*! Allocates memory for the results objects and prepares the object for writing.
	 * @param numRows The number of rows of features on the array
	 * @param numCols The number of columns of features on the array
	 * @param numProbeSets The number of probe sets.
	 * @param chipType The chip type or probe array type
	 * @param probeSetType The type of probe sets in the file
	 */
	void InitializeForWriting(int numRows, int numCols, int numProbeSets, const char *chipType, affxcdf::GeneChipProbeSetType probeSetType);

	/*! Sets the parent CEL file name.
	 * @param str The parent CEL file name (full path).
	 */
	void SetParentCelFileName(const char *str);

	/*! Sets the prog ID of the algorithm used to create the CHP file data.
	 * @param str The prog ID.
	 */
	void SetProgID(const char *str);

	/*! Sets the algorithm name used to create the CHP file data.
	 * @param str The name of the algorithm.
	 */
	void SetAlgorithmName(const char *str);

	/*! Sets the algorithm version used to create the CHP file data.
	 * @param str The algorithm version.
	 */
	void SetAlgorithmVersion(const char *str);

	/*! Adds a parameter name/value pair to the list of algorithm parameters
	 * @param tag The name of the parameter
	 * @param value The value of the parameter
	 */
	void AddAlgorithmParameter(const char *tag, const char *value);

	/*! Adds a parameter name/value pair to the list of summary parameters
	 * @param tag The name of the parameter
	 * @param value The value of the parameter
	 */
	void AddChipSummaryParameter(const char *tag, const char *value);

	/*! Sets the expression probe set result
	 * @param index The index to the probe set results
	 * @param pEntry A pointer to the results
	 */
	void SetExpressionEntry(int index, affxchp::CExpressionProbeSetResults *pEntry);

	/*! Sets the genotyping probe set result
	 * @param index The index to the probe set results
	 * @param pEntry A pointer to the results
	 */
	void SetMappingEntry(int index, affxchp::CGenotypeProbeSetResults *pEntry);

	/*! Saves the results to the file.
	 * @return True if successful
	 */
	bool Save();

	/*! Initializes the object which stores the background zone information
	 * @param nZones The number of zones
	 * @param smoothFactor The smooth factor used by the background algorithm.
	 */
	void AddBackgroundInfo(int nZones, float smoothFactor);

	/*! Adds a background zone to the list
	 * @param x The central x coordinate of the background zone.
	 * @param y The central y coordinate of the background zone.
	 * @param value The background value for the zone.
	 */
	void AddBackgroundZone(int x, int y, float value);
};

////////////////////////////////////////////////////////////////////

} // namespace

////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CHPFILEWRITER_H__A16FCA8B_7B8D_4071_81CD_F6B2BB61A169__INCLUDED_)
