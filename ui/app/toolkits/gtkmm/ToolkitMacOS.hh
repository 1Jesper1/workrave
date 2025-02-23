// Copyright (C) 2021 Rob Caelers <robc@krandor.nl>
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

#ifndef TOOLKIT_MACOS_HH
#define TOOLKIT_MACOS_HH

#include "Toolkit.hh"
#include "MacOSLocker.hh"

class ToolkitMacOS : public Toolkit
{
public:
  ToolkitMacOS(int argc, char **argv);
  ~ToolkitMacOS() override = default;

  // IToolkit
  void init(std::shared_ptr<IApplication> app) override;
  auto get_locker() -> std::shared_ptr<Locker> override;

private:
  void setup_environment();

private:
  std::shared_ptr<MacOSLocker> locker;
};

#endif // TOOLKIT_MACOS_HH
