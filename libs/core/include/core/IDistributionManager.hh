// IDistributionManager.hh
//
// Copyright (C) 2002 - 2008 Rob Caelers <robc@krandor.org>
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

#ifndef IDISTRIBUTIOMANAGER_HH
#define IDISTRIBUTIOMANAGER_HH

#include <string>
#include <list>

namespace workrave
{
  class DistributionLogListener;

  class IDistributionManager
  {
  public:
    virtual ~IDistributionManager() = default;

    virtual bool is_master() const = 0;
    virtual int get_number_of_peers() = 0;
    virtual bool connect(std::string url) = 0;

    virtual bool disconnect_all() = 0;
    virtual bool reconnect_all() = 0;

    virtual bool add_log_listener(DistributionLogListener *listener) = 0;
    virtual bool remove_log_listener(DistributionLogListener *listener) = 0;
    virtual std::list<std::string> get_logs() const = 0;

    virtual bool add_peer(std::string peer) = 0;
    virtual bool remove_peer(std::string peer) = 0;

    virtual void set_peers(std::string peers, bool connect = true) = 0;
    virtual std::list<std::string> get_peers() const = 0;

    virtual bool get_enabled() const = 0;
    virtual void set_enabled(bool b) = 0;

    virtual bool get_listening() const = 0;
    virtual void set_listening(bool b) = 0;

    virtual std::string get_username() const = 0;
    virtual void set_username(std::string name) = 0;

    virtual std::string get_password() const = 0;
    virtual void set_password(std::string name) = 0;

    virtual int get_port() const = 0;
    virtual void set_port(int v) = 0;

    virtual int get_reconnect_attempts() const = 0;
    virtual void set_reconnect_attempts(int v) = 0;

    virtual int get_reconnect_interval() const = 0;
    virtual void set_reconnect_interval(int v) = 0;
  };
} // namespace workrave

#endif // IDISTRIBUTIOMANAGER_HH
