// Copyright (C) 2001 - 2013 Rob Caelers & Raymond Penners
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

#include "debug.hh"
#include <fstream>
#include <stdio.h>

#include "Application.hh"
#include "Toolkit.hh"
#include "core/CoreTypes.hh"
#include "core/ICore.hh"

#ifdef PLATFORM_OS_WINDOWS
#  include <io.h>
#  include <fcntl.h>

#  include "utils/W32ActiveSetup.hh"
#endif

extern "C" int run(int argc, char **argv);

int
run(int argc, char **argv)
{
#if defined(HAVE_CRASHPAD)
  try
    {
      workrave::crash::CrashReporter::instance().init();
    }
  catch (std::exception &e)
    {
    }
#endif

#ifdef PLATFORM_OS_WINDOWS
  W32ActiveSetup::update_all();
#endif

#ifdef TRACING
  Debug::init();
#endif

  std::shared_ptr<IToolkit> toolkit = std::make_shared<Toolkit>(argc, argv);
  std::shared_ptr<Application> app = std::make_shared<Application>(argc, argv, toolkit);
  app->main();

  return 0;
}

#if !defined(PLATFORM_OS_WINDOWS) // || (!defined(PLATFORM_OS_WINDOWS_NATIVE) && !defined(NDEBUG))
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
  (void)hInstance;
  (void)hPrevInstance;
  (void)iCmdShow;

  // InnoSetup: [...] requires that you add code to your application
  // which creates a mutex with the name you specify in this
  // directive.
  HANDLE mtx = CreateMutex(NULL, FALSE, "WorkraveMutex");
  if (mtx != NULL && GetLastError() != ERROR_ALREADY_EXISTS)
    {
      char *argv[] = {szCmdLine};
      run(sizeof(argv) / sizeof(argv[0]), argv);
    }
  return (0);
}

#endif
