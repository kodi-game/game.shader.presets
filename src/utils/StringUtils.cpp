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

#include "StringUtils.h"

using namespace SHADER;

void StringUtils::Tokenize(const std::string& input, std::vector<std::string>& tokens, const std::string& delimiters)
{
   tokens.clear();
   // Skip delimiters at beginning.
   std::string::size_type dataPos = input.find_first_not_of(delimiters);
   while (dataPos != std::string::npos)
   {
      // Find next delimiter
      const std::string::size_type nextDelimPos = input.find_first_of(delimiters, dataPos);
      // Found a token, add it to the vector.
      tokens.push_back(input.substr(dataPos, nextDelimPos - dataPos));
      // Skip delimiters.  Note the "not_of"
      dataPos = input.find_first_not_of(delimiters, nextDelimPos);
   }
}

int StringUtils::Replace(std::string &str, char oldChar, char newChar)
{
  int replacedChars = 0;
  for (std::string::iterator it = str.begin(); it != str.end(); ++it)
  {
    if (*it == oldChar)
    {
      *it = newChar;
      replacedChars++;
    }
  }

  return replacedChars;
}
