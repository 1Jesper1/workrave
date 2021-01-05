// main.cc --- Main
//
// Copyright (C) 2001, 2002, 2003, 2007, 2008 Rob Caelers & Raymond Penners
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "preinclude.h"

#include "debug.hh"
#include <fstream>

#include "GUI.hh"
#ifdef PLATFORM_OS_WINDOWS
#endif

extern "C" int run(int argc, char **argv);

int
run(int argc, char **argv)
{
  GUI *gui = new GUI(argc, argv);

#ifdef PLATFORM_OS_WINDOWS
  dll_hell_check();
#endif

  gui->main();

  delete gui;

  return 0;
}

#if !defined(PLATFORM_OS_WINDOWS) || !defined(NDEBUG)
int
main(int argc, char **argv)
{
  int ret = run(argc, argv);

  return ret;
}

#else

#  include <windows.h>

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  char *argv[] = {szCmdLine};
  char buf[1000];

  // InnoSetup: [...] requires that you add code to your application
  // which creates a mutex with the name you specify in this
  // directive.
  HANDLE mtx = CreateMutex(NULL, FALSE, "WorkraveMutex");
  if (mtx != NULL && GetLastError() != ERROR_ALREADY_EXISTS)
    {
      run(sizeof(argv) / sizeof(argv[0]), argv);
    }
  return (0);
}

#endif
