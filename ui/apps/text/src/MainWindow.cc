// MainWindow.cc --- Main info Window
//
// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2007 Rob Caelers & Raymond Penners
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

#include "preinclude.h"

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "nls.h"
#include "debug.hh"

#include <unistd.h>
#include <iostream>

#include "TimerBoxTextView.hh"
#include "TimerBoxControl.hh"
#include "MainWindow.hh"
#include "GUI.hh"
#include "Util.hh"
#include "Text.hh"

#include "CoreFactory.hh"
#include "IConfigurator.hh"
#include "IStatistics.hh"

const string MainWindow::CFG_KEY_MAIN_WINDOW = "gui/main_window";
const string MainWindow::CFG_KEY_MAIN_WINDOW_ALWAYS_ON_TOP = "gui/main_window/always_on_top";

const string MainWindow::CFG_KEY_MAIN_WINDOW_START_IN_TRAY = "gui/main_window/start_in_tray";

const string MainWindow::CFG_KEY_MAIN_WINDOW_X = "gui/main_window/x";
const string MainWindow::CFG_KEY_MAIN_WINDOW_Y = "gui/main_window/y";
const string MainWindow::CFG_KEY_MAIN_WINDOW_HEAD = "gui/main_window/head";

//! Constructor.
MainWindow::MainWindow()
  : enabled(true)
  , timer_box_control(NULL)
  , timer_box_view(NULL)
  , monitor_suspended(false)
{
  init();
}

//! Destructor.
MainWindow::~MainWindow()
{
  TRACE_ENTER("MainWindow::~MainWindow");

  delete timer_box_control;

  TRACE_EXIT();
}

//! Initializes the main window.
void
MainWindow::init()
{
  TRACE_ENTER("MainWindow::init");

  enabled = TimerBoxControl::is_enabled("main_window");

  timer_box_view = new TimerBoxTextView();
  timer_box_control = new TimerBoxControl("main_window", *timer_box_view);

  workrave::config::IConfigurator::Ptr config = CoreFactory::get_configurator();
  config->add_listener(TimerBoxControl::CFG_KEY_TIMERBOX + "main_window", this);

  TRACE_EXIT();
}

//! Updates the main window.
void
MainWindow::update()
{
  timer_box_control->update();
}

void
MainWindow::config_changed_notify(const string &key)
{
  TRACE_ENTER_MSG("MainWindow::config_changed_notify", key);
  if (key != CFG_KEY_MAIN_WINDOW_HEAD && key != CFG_KEY_MAIN_WINDOW_X && key != CFG_KEY_MAIN_WINDOW_Y)
    {
      // XXX: config changed.
    }
  TRACE_EXIT();
}

bool
MainWindow::get_always_on_top()
{
  bool b;
  bool rc;
  b = CoreFactory::get_configurator()->get_value(MainWindow::CFG_KEY_MAIN_WINDOW_ALWAYS_ON_TOP, rc);
  if (!b)
    {
      rc = false;
    }
  return rc;
}

void
MainWindow::set_always_on_top(bool b)
{
  CoreFactory::get_configurator()->set_value(MainWindow::CFG_KEY_MAIN_WINDOW_ALWAYS_ON_TOP, b);
}

bool
MainWindow::get_start_in_tray()
{
  bool b;
  bool rc;
  b = CoreFactory::get_configurator()->get_value(CFG_KEY_MAIN_WINDOW_START_IN_TRAY, rc);
  if (!b)
    {
      rc = false;
    }
  return rc;
}

void
MainWindow::set_start_in_tray(bool b)
{
  CoreFactory::get_configurator()->set_value(CFG_KEY_MAIN_WINDOW_START_IN_TRAY, b);
}
