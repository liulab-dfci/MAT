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

#include "BEDFileWriter.h"
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

using namespace std;
using namespace affxbed;

#pragma warning(disable: 4996)


/*! The tab character. */
#define TAB "\t"


/*
 * Initialize the class.
 */
BEDFileWriter::BEDFileWriter()
{
}

/*
 * Clears any used memory.
 */
BEDFileWriter::~BEDFileWriter()
{
	Clear();
}

/*
 * Save the intervals to a BED file.
 */
bool BEDFileWriter::Write()
{
	// Return if no intervals
	if (intervalGroups.size() == 0)
		return true;

	// Create the output file.
	ofstream outstr(fileName.c_str(), ios::out);
	if (!outstr)
		return false;

	// Determine the start and stop position in the first sequence.
	int start=0;
	int stop=0;
	string seq;
	bool bFirst=true;
	IntervalEntryListConstIt iter;
	IntervalGroupListIt groupIt;
	for (groupIt=intervalGroups.begin(); groupIt!=intervalGroups.end(); ++groupIt)
	{
		IntervalGroup &group = *groupIt;
		for (iter=group.intervals.begin(); iter!=group.intervals.end(); ++iter)
		{
			const IntervalEntry &interval = *iter;
			if (bFirst)
			{
			bFirst = false;
			seq = interval.seq;
			start = interval.start;
			}

			if (interval.seq != seq)
			break;

			stop = interval.stop;
		}
	}
	FormatBrowser(seq.c_str(), start, stop);
	if (track.length() == 0)
	{
		track = TRACK_LINE_START;
		track += DEFAULT_TRACK_NAME;
	}

	// Write the header line
	outstr << browser << endl;
	outstr << track << endl;

	// Save the intervals
	for (groupIt=intervalGroups.begin(); groupIt!=intervalGroups.end(); ++groupIt)
	{
		IntervalGroup &group = *groupIt;
		TagValuePairTypeList::iterator paramIt;
		for (paramIt=group.parameters.begin(); paramIt!=group.parameters.end(); ++paramIt)
		{
			outstr << "#" << (*paramIt).Tag << TAB << (*paramIt).Value << endl;
		}
		for (iter=group.intervals.begin(); iter!=group.intervals.end(); ++iter)
		{
			const IntervalEntry &interval = *iter;
			outstr << interval.seq << "\t" << interval.start << "\t" << interval.stop;
			if (interval.probeSetName.length() > 0)
			{
				outstr << "\t" << interval.probeSetName << "\t" << interval.overlap << "\t" << interval.strand;
			}
			outstr << endl;
		}
	}

	// Check the status
	if (outstr.fail() != 0)
		return false;

	return true;
}
