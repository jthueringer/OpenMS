// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2016.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Timo Sachsenberg $
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/RNPXL/HyperScore.h>

#include <OpenMS/KERNEL/MSSpectrum.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/KERNEL/RichPeak1D.h>

#include <vector>
#include <map>
#include <cmath>
#include <iostream>

using std::vector;

namespace OpenMS
{
  double HyperScore::logfactorial_(UInt x)
  {
    UInt y;

    if (x < 2)
      return 1;
    else
    {
      double z = 0;
      for (y = 2; y <= x; y++)
      {
        z = log((double)y) + z;
      }

      return z;
    }
  }

  double HyperScore::compute(double fragment_mass_tolerance, bool fragment_mass_tolerance_unit_ppm, const PeakSpectrum& exp_spectrum, const PeakSpectrum& theo_spectrum)
  {
    double dot_product = 0.0;
    UInt y_ion_count = 0;
    UInt b_ion_count = 0;

    if (exp_spectrum.size() < 1 || theo_spectrum.size() < 1)
    {
      std::cout << "Warning: HyperScore: One of the given spectra is empty." << std::endl;
      return 0.0;
    }

    // TODO this assumes only one StringDataArray is present and it is the right one
    PeakSpectrum::StringDataArray ion_names;
    if (theo_spectrum.getStringDataArrays().size() > 0)
    {
      ion_names = theo_spectrum.getStringDataArrays()[0];
    }
    else
    {
      std::cout << "Error: HyperScore: Theoretical spectrum without StringDataArray (\"IonNames\" annotation) provided." << std::endl;
      return 0.0;
    }

    //for (MSSpectrum<Peak1D>::ConstIterator theo_peak_it = theo_spectrum.begin(); theo_peak_it != theo_spectrum.end(); ++theo_peak_it)
    for (Size i = 0; i < theo_spectrum.size(); ++i)
    {
//      if (!theo_peak_it->metaValueExists("IonName"))
//      {
//        std::cout << "Error: Theoretical spectrum without IonName annotation provided." << std::endl;
//        return 0.0;
//      }

//      const double theo_mz = theo_peak_it->getMZ();
//      const double theo_intensity = theo_peak_it->getIntensity();
      const double theo_mz = theo_spectrum[i].getMZ();
      const double theo_intensity = theo_spectrum[i].getIntensity();

      double max_dist_dalton = fragment_mass_tolerance_unit_ppm ? theo_mz * fragment_mass_tolerance * 1e-6 : fragment_mass_tolerance;

      // iterate over peaks in experimental spectrum in given fragment tolerance around theoretical peak
      Size index = exp_spectrum.findNearest(theo_mz);
      double exp_mz = exp_spectrum[index].getMZ();

      // found peak match
      if (std::abs(theo_mz - exp_mz) < max_dist_dalton)
      {
        dot_product += exp_spectrum[index].getIntensity() * theo_intensity;
//        if (theo_peak_it->getMetaValue("IonName").toString()[0] == 'y')
        if (ion_names[i][0] == 'y')
        {
          #ifdef DEBUG_HYPERSCORE
//            std::cout << theo_peak_it->getMetaValue("IonName").toString() << " intensity: " << exp_spectrum[index].getIntensity() << std::endl;
            std::cout << ion_names[i] << " intensity: " << exp_spectrum[index].getIntensity() << std::endl;
          #endif
          ++y_ion_count;
        }
//        else if (theo_peak_it->getMetaValue("IonName").toString()[0] == 'b')
        else if (ion_names[i][0] == 'b')
        {
          #ifdef DEBUG_HYPERSCORE
//            std::cout << theo_peak_it->getMetaValue("IonName").toString() << " intensity: " << exp_spectrum[index].getIntensity() << std::endl;
            std::cout << ion_names[i] << " intensity: " << exp_spectrum[index].getIntensity() << std::endl;
          #endif
          ++b_ion_count;
        }
      }
    }

    // discard very low scoring hits (basically no matching peaks)
    if (dot_product > 1e-1)
    {
      double yFact = logfactorial_(y_ion_count);
      double bFact = logfactorial_(b_ion_count);
      double hyperScore = log(dot_product) + yFact + bFact;
      return hyperScore;
    }
    else
    {
      return 0;
    }
  }

}

