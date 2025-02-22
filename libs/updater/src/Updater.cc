// Copyright (C) 2014 Rob Caelers <robc@krandor.nl>
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

#include "Updater.hh"

#if defined(PLATFORM_OS_MACOS)
#  include "SparkleUpdater.hh"
#endif

using namespace workrave::updater;

Updater::Ptr
UpdaterFactory::create(std::string appcast_url)
{
#if defined(PLATFORM_OS_MACOS)
  return std::make_shared<SparkleUpdater>(appcast_url);
#endif
  return Updater::Ptr();
}
