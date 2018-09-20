//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "video/drawing_context.hpp"

#include <algorithm>

#include "supertux/globals.hpp"
#include "util/obstackpp.hpp"
#include "video/drawing_request.hpp"
#include "video/renderer.hpp"
#include "video/surface.hpp"
#include "video/video_system.hpp"
#include "video/viewport.hpp"

DrawingContext::DrawingContext(VideoSystem& video_system_, obstack& obst, bool overlay) :
  m_video_system(video_system_),
  m_obst(obst),
  m_overlay(overlay),
  m_viewport(0, 0,
             m_video_system.get_viewport().get_screen_width(),
             m_video_system.get_viewport().get_screen_height()),
  m_ambient_color(Color::WHITE),
  m_transform_stack(1),
  m_colormap_canvas(DrawingTarget::COLORMAP, *this, m_obst),
  m_lightmap_canvas(DrawingTarget::LIGHTMAP, *this, m_obst)
{
}

DrawingContext::~DrawingContext()
{
  clear();
}

void
DrawingContext::set_ambient_color(Color ambient_color)
{
  m_ambient_color = ambient_color;
}

Rectf
DrawingContext::get_cliprect() const
{
  return Rectf(get_translation().x,
               get_translation().y,
               get_translation().x + static_cast<float>(m_viewport.get_width()),
               get_translation().y + static_cast<float>(m_viewport.get_height()));
}

void
DrawingContext::set_drawing_effect(DrawingEffect effect)
{
  transform().drawing_effect = effect;
}

DrawingEffect
DrawingContext::get_drawing_effect() const
{
  return transform().drawing_effect;
}

void
DrawingContext::set_alpha(float alpha)
{
  transform().alpha = alpha;
}

float
DrawingContext::get_alpha() const
{
  return transform().alpha;
}

DrawingTransform&
DrawingContext::transform()
{
  assert(!m_transform_stack.empty());
  return m_transform_stack.back();
}

const DrawingTransform&
DrawingContext::transform() const
{
  assert(!m_transform_stack.empty());
  return m_transform_stack.back();
}

void
DrawingContext::push_transform()
{
  m_transform_stack.push_back(transform());
}

void
DrawingContext::pop_transform()
{
  m_transform_stack.pop_back();
  assert(!m_transform_stack.empty());
}

/* EOF */
