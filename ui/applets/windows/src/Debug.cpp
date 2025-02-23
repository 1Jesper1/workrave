// debug.cc
//
// Copyright (C) 2001, 2002, 2003, 2007, 2009, 2010 Rob Caelers <robc@krandor.org>
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#if defined(TRACING)

#  define WIN32_LEAN_AND_MEAN
#  include <windows.h> /* for GetFileAttributes */

#  include "Debug.h"

using namespace std;

std::ofstream g_log_stream;

std::string
Debug::trace_get_time()
{
  char logtime[128];
  time_t ltime;

  time(&ltime);
  struct tm tmlt;
  localtime_s(&tmlt, &ltime);
  strftime(logtime, 128, "%d%b%Y %H:%M:%S ", &tmlt);
  return logtime;
}

void
Debug::init()
{
  std::string debug_filename;
  char path_buffer[MAX_PATH];

  DWORD ret = GetTempPath(MAX_PATH, path_buffer);
  if (ret > MAX_PATH || ret == 0)
    {
      debug_filename = "C:\\temp\\";
    }
  else
    {
      debug_filename = path_buffer;
    }

  char logfile[128];
  time_t ltime;

  time(&ltime);
  struct tm tmlt;
  localtime_s(&tmlt, &ltime);
  strftime(logfile, 128, "workrave-applet-%d%b%Y-%H%M%S", &tmlt);

  debug_filename += logfile;

  g_log_stream.open(debug_filename.c_str(), std::ios::app);
  if (g_log_stream.is_open())
    {
      std::cerr.rdbuf(g_log_stream.rdbuf());
    }
}

#endif
