/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_HTTP_API_COMMON_H_140089624C7E205078FEBA74EC391A0E
#define FS_HTTP_API_COMMON_H_140089624C7E205078FEBA74EC391A0E

#include <boost/asio.hpp>
#include <beast/http/message.hpp>
#include <beast/http/string_body.hpp>

namespace http_api
{

namespace asio = boost::asio;
using IoService = asio::io_service;
/// Synchronization primitive provided by asio
using Strand = IoService::strand;
using ErrorCode = boost::system::error_code;

using Request = beast::http::request<beast::http::string_body>;
using Response = beast::http::response<beast::http::string_body>;
using RequestID = uint32_t;
using PeerID = uint32_t;

} //namespace http_api

#endif // FS_HTTP_API_COMMON_H_140089624C7E205078FEBA74EC391A0E
