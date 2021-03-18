// Control.cc --- The main controller
//
// Copyright (C) 2001, 2002, 2003, 2005, 2010 Rob Caelers <robc@krandor.org>
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

#include <cassert>
#include <cstring>

#include "PacketBuffer.hh"

PacketBuffer::PacketBuffer() = default;

PacketBuffer::~PacketBuffer()
{
  narrow(0, -1);
  if (buffer != nullptr)
    {
      g_free(buffer);
    }
}

void
PacketBuffer::create(int size)
{
  narrow(0, -1);

  if (buffer != nullptr)
    {
      g_free(buffer);
    }

  if (size == 0)
    {
      size = 1024;
    }

  buffer = g_new(guint8, size);
  read_ptr = buffer;
  write_ptr = buffer;
  buffer_size = size;
}

void
PacketBuffer::resize(int size)
{
  // TRACE_ENTER_MSG("PacketBuffer::resize", size);
  narrow(0, -1);

  if (size == 0)
    {
      size = 1024;
    }

  if (size != buffer_size && buffer != nullptr)
    {
      int read_offset = read_ptr - buffer;
      int write_offset = write_ptr - buffer;

      if (read_offset >= size)
        {
          read_offset = size - 1;
        }

      if (write_offset >= size)
        {
          write_offset = size - 1;
        }

      // TRACE_MSG(read_offset << " " << write_offset);

      buffer = g_renew(guint8, buffer, size);

      // TRACE_MSG(buffer);

      read_ptr = buffer + read_offset;
      write_ptr = buffer + write_offset;
      buffer_size = size;
    }
  // TRACE_EXIT();
}

void
PacketBuffer::grow(int size)
{
  // TRACE_ENTER_MSG("PacketBuffer::grow()", size)
  if (size < GROW_SIZE)
    {
      size = GROW_SIZE;
    }

  resize(buffer_size + size);
  // TRACE_EXIT();
}

void
PacketBuffer::pack(const guint8 *data, int size)
{
  if (write_ptr + size + 2 >= buffer + buffer_size)
    {
      grow(size + 2);
    }

  pack_ushort(size);
  memcpy(write_ptr, data, size);
  write_ptr += size;
}

void
PacketBuffer::pack_raw(const guint8 *data, int size)
{
  if (write_ptr + size >= buffer + buffer_size)
    {
      grow(size);
    }

  memcpy(write_ptr, data, size);
  write_ptr += size;
}

void
PacketBuffer::pack_string(const std::string &data)
{
  pack_string(data.c_str());
}

void
PacketBuffer::pack_string(const gchar *data)
{
  int size = 0;
  if (data != nullptr)
    {
      size = strlen(data);
    }

  if (write_ptr + size + 2 >= buffer + buffer_size)
    {
      grow(size + 2);
    }

  pack_ushort(size);

  if (size > 0)
    {
      memcpy(write_ptr, data, size);
      write_ptr += size;
    }
}

void
PacketBuffer::poke_string(int pos, const gchar *data)
{
  int size = 0;
  if (data != nullptr)
    {
      size = strlen(data);
    }

  if (pos + size + 2 >= buffer_size)
    {
      grow(size + 2);
    }

  poke_ushort(pos, size);

  if (size > 0)
    {
      memcpy(buffer + pos + 2, data, size);
    }
}

void
PacketBuffer::pack_ushort(guint16 data)
{
  if (write_ptr + 2 >= buffer + buffer_size)
    {
      grow(2);
    }

  guint8 *w = (guint8 *)write_ptr;
  w[0] = ((data & 0x0000ff00) >> 8);
  w[1] = ((data & 0x000000ff));

  write_ptr += 2;
}

void
PacketBuffer::pack_ulong(guint32 data)
{
  if (write_ptr + 4 >= buffer + buffer_size)
    {
      grow(4);
    }

  guint8 *w = (guint8 *)write_ptr;
  w[0] = ((data & 0xff000000) >> 24);
  w[1] = ((data & 0x00ff0000) >> 16);
  w[2] = ((data & 0x0000ff00) >> 8);
  w[3] = ((data & 0x000000ff));

  write_ptr += 4;
}

void
PacketBuffer::pack_byte(guint8 data)
{
  if (write_ptr + 1 >= buffer + buffer_size)
    {
      grow(1);
    }

  write_ptr[0] = data;
  write_ptr++;
}

void
PacketBuffer::poke_byte(int pos, guint8 data)
{
  if (pos + 1 > buffer_size)
    {
      grow(pos + 1 - buffer_size);
    }

  buffer[pos] = data;
}

void
PacketBuffer::poke_ushort(int pos, guint16 data)
{
  if (pos + 2 > buffer_size)
    {
      grow(pos + 2 - buffer_size);
    }

  guint8 *w = (guint8 *)buffer;

  w[pos] = ((data & 0x0000ff00) >> 8);
  w[pos + 1] = ((data & 0x000000ff));
}

int
PacketBuffer::unpack(guint8 **data)
{
  g_assert(data != nullptr);

  int size = unpack_ushort();

  guint8 *r = (guint8 *)read_ptr;

  if (read_ptr + size <= buffer + buffer_size)
    {
      *data = g_new(guint8, size);
      memcpy(*data, r, size);
      read_ptr += size;
    }
  else
    {
      size = 0;
    }

  return size;
}

int
PacketBuffer::unpack_raw(guint8 **data, int size)
{
  g_assert(data != nullptr);

  guint8 *r = (guint8 *)read_ptr;

  if (read_ptr + size <= buffer + buffer_size)
    {
      *data = g_new(guint8, size);
      memcpy(*data, r, size);
      read_ptr += size;
    }
  else
    {
      size = 0;
    }

  return size;
}

gchar *
PacketBuffer::unpack_string()
{
  gchar *str = nullptr;

  if (read_ptr + 2 <= buffer + buffer_size)
    {
      int length = unpack_ushort();

      if (read_ptr + length <= buffer + buffer_size)
        {
          str = g_new(gchar, length + 1);
          for (int i = 0; i < length; i++)
            {
              str[i] = *read_ptr;
              read_ptr++;
            }

          str[length] = '\0';
        }
    }

  return str;
}

guint32
PacketBuffer::unpack_ulong()
{
  guint32 ret = 0;
  guint8 *r = (guint8 *)read_ptr;

  if (read_ptr + 4 <= buffer + buffer_size)
    {
      ret = (((guint32)(r[0]) << 24) + ((guint32)(r[1]) << 16) + ((guint32)(r[2]) << 8) + ((guint32)(r[3])));
      read_ptr += 4;
    }

  return ret;
}

guint16
PacketBuffer::unpack_ushort()
{
  guint16 ret = 0;
  guint8 *r = (guint8 *)read_ptr;

  if (read_ptr + 2 <= write_ptr)
    {
      ret = (r[0] << 8) + r[1];
      read_ptr += 2;
    }
  return ret;
}

guint8
PacketBuffer::unpack_byte()
{
  guint8 ret = 0;

  if (read_ptr + 1 <= write_ptr)
    {
      ret = read_ptr[0];
      read_ptr++;
    }
  return ret;
}

int
PacketBuffer::peek(int pos, guint8 **data)
{
  g_assert(data != nullptr);

  int size = peek_ushort(pos);

  if (read_ptr + 2 + pos + size <= buffer + buffer_size)
    {
      *data = g_new(guint8, size);
      memcpy(*data, read_ptr + 2 + pos, size);
    }
  else
    {
      size = 0;
    }

  return size;
}

gchar *
PacketBuffer::peek_string(int pos)
{
  gchar *str = nullptr;

  if (read_ptr + pos + 2 <= buffer + buffer_size)
    {
      int length = peek_ushort(pos);

      if (read_ptr + 2 + pos + length <= buffer + buffer_size)
        {
          str = g_new(gchar, length + 1);
          memcpy(str, read_ptr + pos + 2, length);
          str[length] = '\0';
        }
    }

  return str;
}

guint32
PacketBuffer::peek_ulong(int pos)
{
  guint32 ret = 0;
  if (read_ptr + pos + 4 <= buffer + buffer_size)
    {
      guint8 *r = (guint8 *)read_ptr;

      ret = (((guint32)(r[pos]) << 24) + ((guint32)(r[pos + 1]) << 16) + ((guint32)(r[pos + 2]) << 8) + ((guint32)(r[pos + 3])));
    }

  return ret;
}

guint16
PacketBuffer::peek_ushort(int pos)
{
  guint16 ret = 0;
  if (read_ptr + pos + 2 <= write_ptr)
    {
      guint8 *r = (guint8 *)read_ptr;
      ret = (r[pos] << 8) + r[pos + 1];
    }
  return ret;
}

guint8
PacketBuffer::peek_byte(int pos)
{
  guint8 ret = 0;
  if (read_ptr + pos + 1 <= buffer + buffer_size)
    {
      ret = read_ptr[pos];
    }
  return ret;
}

void
PacketBuffer::reserve_size(int &pos)
{
  pos = bytes_written();
  pack_ushort(0);
}

void
PacketBuffer::update_size(int pos)
{
  poke_ushort(pos, bytes_written() - pos - 2);
}

int
PacketBuffer::read_size(int &pos)
{
  int size = unpack_ushort();

  pos = bytes_read() + size;

  return size;
}

void
PacketBuffer::skip_size(int &pos)
{
  int size = (pos - bytes_read());
  skip(size);
}

void
PacketBuffer::insert(int pos, int size)
{
  if (pos < bytes_written())
    {
      int move = bytes_written() - pos;

      memmove(buffer + pos + size, buffer + pos, move);

      write_ptr += size;
    }
}

void
PacketBuffer::narrow(int pos, int size)
{
  // TRACE_ENTER_MSG("PacketBuffer::narrow", pos << " " << size);
  if (pos == 0 && size == -1)
    {
      if (original_buffer != nullptr)
        {
          // unnarrow.
          buffer = original_buffer;
          buffer_size = original_buffer_size;
          original_buffer_size = 0;
          original_buffer = nullptr;
        }
    }
  else
    {
      if (pos == -1)
        {
          pos = bytes_read();
        }

      if (original_buffer == nullptr)
        {
          original_buffer = buffer;
          original_buffer_size = buffer_size;
        }

      if (size > original_buffer_size - pos)
        {
          size = original_buffer_size - pos;
        }

      buffer = original_buffer + pos;
      buffer_size = size;
      read_ptr = buffer;
    }
  // TRACE_EXIT();
}
