// IActivityMonitor.hh --- Interface definition for the Activity Monitor
//
// Copyright (C) 2001, 2002, 2003, 2005, 2006, 2007, 2008 Rob Caelers <robc@krandor.nl>
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

#ifndef IACTIVITYMONITOR_HH
#define IACTIVITYMONITOR_HH

#include <string>
#include <iostream>

class ActivityMonitorListener;

//! State of the activity monitor.
enum ActivityState
{
  ACTIVITY_UNKNOWN,
  ACTIVITY_SUSPENDED,
  ACTIVITY_IDLE,
  ACTIVITY_NOISE,
  ACTIVITY_ACTIVE
};

inline std::ostream &
operator<<(std::ostream &stream, ActivityState mode)
{
  switch (mode)
    {
    case ACTIVITY_UNKNOWN:
      stream << "unknown";
      break;
    case ACTIVITY_SUSPENDED:
      stream << "suspended";
      break;
    case ACTIVITY_IDLE:
      stream << "idle";
      break;
    case ACTIVITY_NOISE:
      stream << "noise";
      break;
    case ACTIVITY_ACTIVE:
      stream << "active";
      break;
    }
  return stream;
}

//! Interface that all activity monitor implements.
class IActivityMonitor
{
public:
  virtual ~IActivityMonitor() {}

  //! Stops the activity monitoring.
  virtual void terminate() = 0;

  //! Suspends the activity monitoring.
  virtual void suspend() = 0;

  //! Resumes the activity monitoring.
  virtual void resume() = 0;

  //! Returns the current state
  virtual ActivityState get_current_state() = 0;

  //! Force state to be idle.
  virtual void force_idle() = 0;

  //! Sets the callback for activity monitor events.
  virtual void set_listener(ActivityMonitorListener *l) = 0;
};

#endif // IACTIVITYMONITOR_HH
