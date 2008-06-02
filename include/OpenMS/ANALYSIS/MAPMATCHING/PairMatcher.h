// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_MAPMATCHING_PAIRMATCHER_H
#define OPENMS_ANALYSIS_MAPMATCHING_PAIRMATCHER_H

#include <OpenMS/ANALYSIS/MAPMATCHING/BasePairFinder.h>

#include <cmath>

namespace OpenMS
{
	/**
	 	@brief The PairMatcher allows the matching of labeled features (features with a fixed distance).
		
		Finds feature pairs that have a defined distance in RT and m/z in the same map.
	
		@ref PairMatcher_Parameters are explained on a separate page.
		
		@todo derive from BasePairFinder - takes two times the same map (Marc)
		
		@ingroup FeatureGrouping
	*/
	class PairMatcher
		: public BasePairFinder
	{
		
		public:
			
			/// Default constructor
			PairMatcher();
	
			/// Destructor
			inline virtual ~PairMatcher()
			{
			}
	
			/// Run the algorithm
			virtual void run(ConsensusMap& result_map);
	
		protected:
			
	    /// Square root of two
	    static const DoubleReal sqrt2_;
	
			/// all possible pairs (after Pairing)
			ConsensusMap pairs_;
	
			/// return the p-value at position x for the bi-Gaussian distribution
			/// with mean @p m and standard deviation @p sig1 (left) and @p sig2 (right)
			inline DoubleReal PValue_(DoubleReal x, DoubleReal m, DoubleReal sig1, DoubleReal sig2)
			{
				if (m<x)
				{
					return 1-erf((x-m)/sig2/sqrt2_);
				}
				else
				{
					return 1-erf((m-x)/sig1/sqrt2_);
				}
			}
		
		private:
			
			/// Copy constructor not implemented => private
			PairMatcher(const PairMatcher& source);

			/// Assignment operator not implemented => private
	    PairMatcher& operator=(const PairMatcher& source);

	}; // end of class PairMatcher

} // end of namespace OpenMS

#endif  // OPENMS_ANALYSIS_MAPMATCHER_PAIRMATCHER_H
