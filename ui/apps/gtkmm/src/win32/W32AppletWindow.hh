// AppletWindow.hh --- Applet window
//
// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2009, 2010, 2011, 2012 Rob Caelers & Raymond Penners
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

#ifndef W32APPLETWINDOW_HH
#define W32APPLETWINDOW_HH

#include <windows.h>
#include <process.h>
#include <string>
#include <gdk/gdkwin32.h>

#include "commonui/TimerBoxViewBase.hh"
#include "commonui/UiTypes.hh"
#include "utils/Signals.hh"

#include "Applet.hh"
#include "AppletWindow.hh"
#include "MenuModel.hh"
#include "MenuHelper.hh"

class W32AppletWindow
  : public AppletWindow
  , public TimerBoxViewBase
  , public workrave::utils::Trackable
{
public:
  W32AppletWindow(MenuModel::Ptr menu_model);
  virtual ~W32AppletWindow();

  void set_slot(workrave::BreakId id, int slot) override;
  void set_time_bar(workrave::BreakId id,
                    int value,
                    TimerColorId primary_color,
                    int primary_value,
                    int primary_max,
                    TimerColorId secondary_color,
                    int secondary_value,
                    int secondary_max) override;
  void update_view() override;
  void update_time_bars();
  void update_menu();
  bool is_visible() const override;
  void set_geometry(Orientation orientation, int size) override;

  GdkFilterReturn win32_filter_func(void *xevent, GdkEvent *event);

private:
  void init_menu(HWND dest);
  void init_thread();
  void add_menu(const std::string &text, short cmd, int flags);

  bool on_applet_command(int command);

  enum MenuFlag
  {
    MENU_FLAG_TOGGLE = APPLET_MENU_FLAG_TOGGLE,
    MENU_FLAG_SELECTED = APPLET_MENU_FLAG_SELECTED,
    MENU_FLAG_POPUP = APPLET_MENU_FLAG_POPUP
  };

private:
  void update_applet_window();

  static unsigned __stdcall run_event_pipe_static(void *);

private:
  void init();
  void init_menu();
  void process_menu(menus::Node::Ptr node, bool popup = false);
  void run_event_pipe();

  HWND applet_window{nullptr};
  bool menu_sent{false};

  AppletHeartbeatData local_heartbeat_data;
  AppletMenuData local_menu_data;
  HWND local_applet_window{nullptr};

  AppletHeartbeatData heartbeat_data;
  AppletMenuData menu_data;
  CRITICAL_SECTION heartbeat_data_lock;
  HANDLE heartbeat_data_event{nullptr};
  HANDLE thread_abort_event{nullptr};
  HANDLE thread_handle{nullptr};
  volatile unsigned thread_id{0};
  MenuModel::Ptr menu_model;
  MenuHelper menu_helper;
};

#endif // W32APPLETWINDOW_HH
