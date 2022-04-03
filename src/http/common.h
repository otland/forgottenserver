// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HTTP_COMMON_H
#define FS_HTTP_COMMON_H

#include <boost/asio.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

namespace Http
{

namespace asio = boost::asio;
/// Synchronization primitive provided by asio
using Strand = asio::io_service::strand;
using ErrorCode = boost::system::error_code;

using Request = boost::beast::http::request<boost::beast::http::string_body>;
using Response = boost::beast::http::response<boost::beast::http::string_body>;
using RequestID = uint32_t;
using PeerId = uint32_t;

} //namespace Http

#endif // FS_HTTP_COMMON_H
