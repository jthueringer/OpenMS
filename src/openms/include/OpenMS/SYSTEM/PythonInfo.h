// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
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
// $Maintainer: Chris Bielow $
// $Authors: Chris Bielow $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/config.h>

namespace OpenMS
{
  class String;
  /**
    @brief Detect Python and retrieve information.

    Similar classes exist for other external tools, e.g. JavaInfo .

    @ingroup System
  */
  class OPENMS_DLLAPI PythonInfo
  {
  public:
    /**
      @brief Determine if Python is installed and executable

      The call fails if either Python is not installed or if a relative location is given and Python is not on the search PATH.
      If Python is found, the executable name will be modified to the absolute path.
      If Python is not found, an error message will be put into @p error_msg

      @param python_executable Path to Python executable. Can be absolute, relative or just a filename
      @param error_msg On error, contains detailed error description (e.g. 
      @return Returns false if Java executable can not be called; true if Java executable can be executed
    **/
    static bool canRun(String& python_executable, String& error_msg);


    /**
     @brief Determine if the Python given in @p python_executable has the package @p package_name already installed

     If Python cannot be found, the function will just return false.
     Thus, make sure that PythonInfo::canRun() succeeds before calling this function.

     @param python_executable As determined by canRun()...
     @param package_name The package you want to test (mind lower/upper case!)
     @return true if package is installed
    */
    static bool isPackageInstalled(const String& python_executable, const String& package_name);

    /**
     @brief Determine the version of Python given in @p python_executable by calling '--version'

     If Python cannot be found, the function will return the empty string.
     Thus, make sure that PythonInfo::canRun() succeeds before calling this function.

     @param python_executable As determined by canRun()...
     @return the output of 'python --version'
    */
    static String getVersion(const String& python_executable);
  };

}

