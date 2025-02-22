// Copyright (C) 2022 Rob Caelers <robc@krandor.nl>
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

#include "FrameWidget.hh"

using namespace ui::prefwidgets::gtkmm;

FrameWidget::FrameWidget(std::shared_ptr<ui::prefwidgets::Frame> def, std::shared_ptr<ContainerWidget> container, BuilderRegistry *registry)
  : ContainerWidget(registry)
  , def(def)
{
  panel = Gtk::manage(new HigCategoryPanel(def->get_label().c_str()));
  panel->set_border_width(12);
  add_to_size_groups(def, panel);

  container->add_widget(*panel);
}

void
FrameWidget::add(std::shared_ptr<Widget> w)
{
  children.push_back(w);
}

void
FrameWidget::add_label(const std::string &label, Gtk::Widget &widget, bool expand, bool fill)
{
  panel->add_label(label.c_str(), widget, expand, fill);
}

void
FrameWidget::add_widget(Gtk::Widget &widget, bool expand, bool fill)
{
  panel->add_widget(widget, expand, fill);
}
