/*
 *      Copyright (C) 2015-2017 Garrett Brown
 *      Copyright (C) 2015-2017 Team Kodi
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <string>
#include <vector>

namespace SHADER
{
  class StringUtils
  {
  public:
    /*! \brief Split a string by the specified delimiters.
    Splits a string using one or more delimiting characters, ignoring empty tokens.
    Differs from Split() in two ways:
    1. The delimiters are treated as individual characters, rather than a single delimiting string.
    2. Empty tokens are ignored.
    \return a vector of tokens
    */
    static void Tokenize(const std::string& input, std::vector<std::string>& tokens, const std::string& delimiters);

    template<typename CONTAINER>
    static std::string Join(const CONTAINER &strings, const std::string& delimiter)
    {
       std::string result;
       for (const auto& str : strings)
          result += str + delimiter;

       if (!result.empty())
          result.erase(result.size() - delimiter.size());
       return result;
    }

    static int Replace(std::string &str, char oldChar, char newChar);
  };
}
