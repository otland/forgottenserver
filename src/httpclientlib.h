/****************************************************************************
 *
 *  Copyright (c) 2023, Danilo Pucci
 *
 *  This file is part of the HTTP Client header-only library.
 *
 *  Source Code:
 *  https://github.com/danilopucci/httpclient/
 *
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so,subject to the
 *  following conditions:
 *
 *  + The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *  + Credit is appreciated, but not required, if you find this project
 *    useful enough to include in your application, product, device, etc.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

#ifndef HTTPCLIENTLIB_H
#define HTTPCLIENTLIB_H

#include <boost/algorithm/string.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/ssl.hpp>
#include <chrono>
#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>

// Disable old-style-cast warnings for this file
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

namespace HttpClientLib {
class HttpConnection;
class HttpResponse;

using HttpResponse_ptr = std::shared_ptr<HttpResponse>;
using HttpResponse_cb = std::function<void(const HttpResponse_ptr&)>;
using HttpFailure_cb = std::function<void(const HttpResponse_ptr&)>;

class HttpUrl
{
public:
	HttpUrl(const std::string& url_) : url(url_)
	{
		parseUrl(url);
	};

	bool isValid() { return valid; }

	bool isProtocolSecure() const { return secure; }

	std::string url;
	std::string target;

	std::string protocol;
	std::string host;
	int port;
	std::string path;
	std::string query;
	std::string fragment;

private:
	void parseUrl(const std::string& url)
	{
		static const std::regex urlRegex(R"(^(https?:\/\/)?([^\/:]+)(:\d+)?(\/.*)?$)");
		valid = false;

		std::smatch matches;
		if (std::regex_match(url, matches, urlRegex)) {
			auto& scheme = matches[1];
			auto& host = matches[2];
			auto& port = matches[3];
			auto& arguments = matches[4];

			setProtocol(scheme);
			setHost(host);
			setPort(port);

			if (arguments.matched) {
				target = arguments.str();

				parsePath(arguments.str());
				parseQuery(arguments.str());
				parseFragment(arguments.str());
			} else {
				target = "/";
			}

			valid = true;
		}
	}

	void parsePath(const std::string& arguments)
	{
		static const std::regex pathRegex(R"(/([^?#]*))");
		std::smatch match;
		if (std::regex_search(arguments, match, pathRegex)) {
			setPath(match[1]);
		}
	}

	void parseQuery(const std::string& arguments)
	{
		static const std::regex queryRegex(R"(\?([^#]*))");
		std::smatch match;
		if (std::regex_search(arguments, match, queryRegex)) {
			setQuery(match[1]);
		}
	}

	void parseFragment(const std::string& arguments)
	{
		static const std::regex fragmentRegex(R"(#(.*))");
		std::smatch match;
		if (std::regex_search(arguments, match, fragmentRegex)) {
			setFragment(match[1]);
		}
	}

	void setProtocol(const std::ssub_match& match)
	{
		if (match.matched) {
			protocol = match.str();
			boost::algorithm::to_lower(protocol);
		} else {
			protocol = "http://";
		}

		secure = protocol == "https://";
	}

	void setHost(const std::ssub_match& match)
	{
		if (match.matched) {
			host = match.str();
		}
	}

	void setPort(const std::ssub_match& match)
	{
		if (match.matched) {
			port = match.str().empty() ? 0 : std::stoi(match.str().substr(1));
		} else {
			if (protocol.find("https://") != std::string::npos) {
				port = 443;
			} else if (protocol.find("http://") != std::string::npos) {
				port = 80;
			}
		}
	}

	void setPath(const std::ssub_match& match)
	{
		if (match.matched) {
			path = match.str();
		}
	}

	void setQuery(const std::ssub_match& match)
	{
		if (match.matched) {
			query = match.str();
		}
	}

	void setFragment(const std::ssub_match& match)
	{
		if (match.matched) {
			fragment = match.str();
		}
	}

private:
	bool valid;
	bool secure;
};

class HttpResponse
{
public:
	uint32_t requestId;
	int version;
	int statusCode;
	std::string location;
	std::string contentType;
	uint32_t responseTimeMs;

	std::string headerData;

	size_t bodySize;
	std::string bodyData;

	bool success;
	std::string errorMessage;

private:
	void buildHeaderData(const boost::beast::http::response_parser<boost::beast::http::dynamic_body>& response)
	{
		auto& responseHeader = response.get();
		statusCode = responseHeader.result_int();
		version = responseHeader.version();
		location = std::string(responseHeader[boost::beast::http::field::location]);
		contentType = std::string(responseHeader[boost::beast::http::field::content_type]);

		auto headers = responseHeader.base();
		for (const auto& header : headers) {
			boost::beast::string_view headerName = header.name_string();
			boost::beast::string_view headerValue = header.value();

			std::string headerString = std::string(headerName) + ": " + std::string(headerValue) + "\n";
			headerData.insert(headerData.end(), headerString.begin(), headerString.end());
		}

		bodySize = 0;
		if (responseHeader.has_content_length()) {
			bodySize = std::stoul(std::string(responseHeader[boost::beast::http::field::content_length]));
		}
	}

	inline void buildBodyData(const boost::beast::http::response_parser<boost::beast::http::dynamic_body>& response)
	{
		bodyData = boost::beast::buffers_to_string(response.get().body().data());
	}

	void setResponseTime(uint32_t responseTimeMs_) { responseTimeMs = responseTimeMs_; }

	void setRequestId(uint32_t requestId_) { requestId = requestId_; }

	friend class HttpConnectionBase;
	friend class HttpConnection;
	friend class HttpsConnection;
};

class HttpConnectionBase : public std::enable_shared_from_this<HttpConnectionBase>
{
public:
	HttpConnectionBase(boost::asio::any_io_executor executor, uint32_t id, HttpResponse_cb responseCallback,
	                   HttpFailure_cb failureCallback) :
	    resolver(executor),
	    stream(executor),
	    timer(executor),
	    id(id),
	    responseData(std::make_shared<HttpResponse>()),
	    responseCallback(responseCallback),
	    failureCallback(failureCallback)
	{
		setTimeout(30000);
		responseData->setRequestId(id);
	}

	virtual ~HttpConnectionBase() {}

	inline void setTimeout(int timeout_) { timeout = timeout_; }

	virtual void create(const boost::beast::http::request<boost::beast::http::string_body>& request_,
	                    const std::string& url, uint32_t port, bool skipBody = false)
	{
		timer.expires_after(std::chrono::milliseconds(timeout));
		timer.async_wait(std::bind(&HttpConnectionBase::onTimeout, shared_from_this(), std::placeholders::_1));

		request = request_;
		connectionStart = std::chrono::steady_clock::now();
		response.skip(skipBody);
		resolve(url, port);
	}

	virtual void resolve(const std::string& url, uint32_t port)
	{
		resolver.async_resolve(url, std::to_string(port),
		                       std::bind(&HttpConnectionBase::onResolve, this->shared_from_this(),
		                                 std::placeholders::_1, std::placeholders::_2));
	}

	virtual void onResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::results_type results)
	{
		if (!error) {
			connect(results);
		} else {
			onError("Failed to resolve to HTTP address: " + error.message());
		}
	}

	virtual void connect(const boost::asio::ip::tcp::resolver::results_type& results)
	{
		boost::asio::async_connect(
		    stream, results.begin(), results.end(),
		    std::bind(&HttpConnectionBase::onConnect, this->shared_from_this(), std::placeholders::_1));
	}

	virtual void onConnect(const boost::system::error_code& error)
	{
		if (!error) {
			writeRequest();
		} else {
			onError("Failed to connect to HTTP socket: " + error.message());
		}
	}

	virtual void writeRequest()
	{
		boost::beast::http::async_write(
		    stream, request,
		    std::bind(&HttpConnectionBase::onRequestWrite, this->shared_from_this(), std::placeholders::_1));
	}

	virtual void onRequestWrite(const boost::beast::error_code& error)
	{
		if (!error) {
			readHeader();
		} else {
			close();
			onError("Failed to write HTTP request: " + error.message());
		}
	}

	virtual void readHeader()
	{
		buffer.max_size(MAX_HEADER_CHUNCK_SIZE);
		boost::beast::http::async_read_header(
		    stream, buffer, response,
		    std::bind(&HttpConnectionBase::onReadHeader, this->shared_from_this(), std::placeholders::_1));
	}

	virtual void onReadHeader(const boost::beast::error_code& error)
	{
		if (!error || response.is_header_done()) {
			responseData->setRequestId(id);
			responseData->buildHeaderData(response);
			readBody();
		} else {
			close();
			onError("Failed to read HTTP header: " + error.message());
		}
	}

	virtual void readBody()
	{
		timer.expires_after(std::chrono::milliseconds(timeout));
		buffer.max_size(MAX_BODY_CHUNCK_SIZE);
		boost::beast::http::async_read_some(
		    stream, buffer, response,
		    std::bind(&HttpConnectionBase::onReadBody, this->shared_from_this(), std::placeholders::_1));
	}

	virtual void onReadBody(const boost::beast::error_code& error)
	{
		if (error && error != boost::beast::http::error::end_of_stream) {
			close();
			onError("Failed to read HTTP body: " + error.message());
			return;
		}

		if (error == boost::beast::http::error::end_of_stream || response.is_done()) {
			responseData->setResponseTime(calculateResponseTime());
			responseData->buildBodyData(response);
			responseData->success = true;
			onSuccess(responseData);

			close();
			return;
		}

		readBody();
	}

	virtual void close()
	{
		timer.cancel();
		boost::system::error_code ec;
		stream.close(ec);
	}

	virtual void onShutdown() {}

	void onTimeout(const boost::system::error_code& error)
	{
		if (!error) {
			close();
			onError("Failed on HTTP: timeout");
		}
	}

	void inline onError(const std::string& reason)
	{
		if (failureCallback) {
			if (!responseData) {
				responseData = std::make_shared<HttpResponse>();
			}

			responseData->success = false;
			responseData->errorMessage = reason;

			failureCallback(responseData);
		}
	}

	void inline onSuccess(const HttpResponse_ptr& responseData)
	{
		if (responseCallback) {
			responseCallback(responseData);
		}
	}

protected:
	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::socket stream;

	boost::asio::steady_timer timer;

	int timeout;
	uint32_t id;

	boost::beast::flat_buffer buffer;
	boost::beast::http::request<boost::beast::http::string_body> request;
	boost::beast::http::response_parser<boost::beast::http::dynamic_body> response;

	std::chrono::steady_clock::time_point connectionStart;

	const int MAX_HEADER_CHUNCK_SIZE = 8 * 1024;
	const int MAX_BODY_CHUNCK_SIZE = 64 * 1024;

	HttpResponse_ptr responseData;
	HttpResponse_cb responseCallback;
	HttpFailure_cb failureCallback;

	uint32_t calculateResponseTime()
	{
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> duration = end - connectionStart;
		return static_cast<uint32_t>(duration.count());
	}
};

class HttpConnection : public HttpConnectionBase
{
public:
	HttpConnection(boost::asio::io_context& ioContext, uint32_t id, HttpResponse_cb responseCallback,
	               HttpFailure_cb failureCallback) :
	    HttpConnectionBase(boost::asio::make_strand(ioContext), id, responseCallback, failureCallback)
	{}
};

class HttpsConnection : public HttpConnectionBase
{
public:
	HttpsConnection(boost::asio::io_context& ioContext, uint32_t id, boost::asio::ssl::context& sslContext,
	                HttpResponse_cb responseCallback, HttpFailure_cb failureCallback) :
	    HttpConnectionBase(boost::asio::make_strand(ioContext), id, responseCallback, failureCallback),
	    sslStream(stream, sslContext)
	{}

	void create(const boost::beast::http::request<boost::beast::http::string_body>& request_, const std::string& url,
	            uint32_t port, bool skipBody = false) override
	{
		sslStream.set_verify_mode(boost::asio::ssl::verify_peer);
		sslStream.set_verify_callback([](bool, boost::asio::ssl::verify_context&) { return true; });

		if (!SSL_set_tlsext_host_name(sslStream.native_handle(), url.c_str())) {
			boost::beast::error_code ec2(static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category());
			onError("HTTPS error" + ec2.message());
			return;
		}

		timer.expires_after(std::chrono::milliseconds(timeout));
		timer.async_wait(std::bind(&HttpConnectionBase::onTimeout, shared_from_this(), std::placeholders::_1));

		request = request_;
		connectionStart = std::chrono::steady_clock::now();
		response.skip(skipBody);
		resolve(url, port);
	}

	void onConnect(const boost::system::error_code& error) override
	{
		if (!error) {
			handshake();
		} else {
			onError("Failed to connect to HTTP socket: " + error.message());
		}
	}

	void handshake()
	{
		auto self(shared_from_this());
		sslStream.async_handshake(boost::asio::ssl::stream_base::client,
		                          [&, self](const boost::system::error_code& error) {
			                          if (!error) {
				                          writeRequest();
			                          } else {
				                          onError("Failed SSL handshake: " + error.message());
			                          }
		                          });
	}

	void writeRequest() override
	{
		boost::beast::http::async_write(
		    sslStream, request,
		    std::bind(&HttpConnectionBase::onRequestWrite, this->shared_from_this(), std::placeholders::_1));
	}

	void readHeader() override
	{
		buffer.max_size(MAX_HEADER_CHUNCK_SIZE);
		boost::beast::http::async_read_header(
		    sslStream, buffer, response,
		    std::bind(&HttpConnectionBase::onReadHeader, this->shared_from_this(), std::placeholders::_1));
	}

	void readBody() override
	{
		timer.expires_after(std::chrono::milliseconds(timeout));
		buffer.max_size(MAX_BODY_CHUNCK_SIZE);
		boost::beast::http::async_read_some(
		    sslStream, buffer, response,
		    std::bind(&HttpConnectionBase::onReadBody, this->shared_from_this(), std::placeholders::_1));
	}

	void close() override
	{
		timer.cancel();
		sslStream.async_shutdown(std::bind(&HttpConnectionBase::onShutdown, this->shared_from_this()));
	}

	void onShutdown() override
	{
		boost::system::error_code ec;
		stream.close(ec);
	}

private:
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> sslStream;
};

class Request
{
public:
	Request() : context(), guard(boost::asio::make_work_guard(context)), requestId(1)
	{
		thread = std::thread([this]() { context.run(); });
	}

	Request(const HttpResponse_cb& responseCallback) :
	    context(), guard(boost::asio::make_work_guard(context)), requestId(1), responseCallback(responseCallback)
	{
		thread = std::thread([this]() { context.run(); });
	}

	Request(const HttpResponse_cb& responseCallback, const HttpFailure_cb& failureCallback) :
	    context(),
	    guard(boost::asio::make_work_guard(context)),
	    requestId(1),
	    responseCallback(responseCallback),
	    failureCallback(failureCallback)
	{
		thread = std::thread([this]() { context.run(); });
	}

	~Request()
	{
		context.stop();

		guard.reset();
		if (thread.joinable()) {
			thread.join();
		}
	}

	inline uint32_t getRequestId() { return requestId; }

	inline void setTimeout(uint32_t timeout) { requestTimeoutMs = timeout; }

	bool connect(const std::string& url) { return connect(url, emptyFields); }

	bool connect(const std::string& url, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request CONNECT: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::connect);

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request CONNECT (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool trace(const std::string& url) { return trace(url, emptyFields); }

	bool trace(const std::string& url, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request TRACE: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::trace);

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request TRACE (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool options(const std::string& url) { return options(url, emptyFields); }

	bool options(const std::string& url, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request OPTIONS: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::options);

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request OPTIONS (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool head(const std::string& url) { return head(url, emptyFields); }

	bool head(const std::string& url, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request HEAD: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::head);
			const bool skipBody = true;

			doRequest(httpUrl, request, skipBody);
		} catch (std::exception& e) {
			onError("error during HTTP request HEAD (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool delete_(const std::string& url) { return delete_(url, emptyFields); }

	bool delete_(const std::string& url, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request DELETE: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::delete_);

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request DELETE (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool get(const std::string& url) { return get(url, emptyFields); }

	bool get(const std::string& url, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request GET: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::get);

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request GET (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool post(const std::string& url, const std::string& postData) { return post(url, postData, emptyFields); }

	bool post(const std::string& url, const std::string& postData, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request POST: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::post);
			request.body() = postData;
			request.prepare_payload();

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request POST (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool patch(const std::string& url, const std::string& patchData) { return patch(url, patchData, emptyFields); }

	bool patch(const std::string& url, const std::string& patchData,
	           std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request PATCH: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::patch);
			request.body() = patchData;
			request.prepare_payload();

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request PATCH (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

	bool put(const std::string& url, const std::string& putData) { return put(url, putData, emptyFields); }

	bool put(const std::string& url, const std::string& putData, std::unordered_map<std::string, std::string>& fields)
	{
		HttpUrl httpUrl(url);

		if (!httpUrl.isValid()) {
			onError("error during HTTP request PUT: invalid URL: " + url);
			return false;
		}

		try {
			boost::beast::http::request<boost::beast::http::string_body> request = buildBasicRequest(httpUrl, fields);
			setUniqueRequestId();
			request.method(boost::beast::http::verb::put);
			request.body() = putData;
			request.prepare_payload();

			doRequest(httpUrl, request);
		} catch (std::exception& e) {
			onError("error during HTTP request PUT (" + url + "): " + e.what());
			return false;
		}

		return true;
	}

private:
	std::thread thread;
	boost::asio::io_context context;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard;

	std::unordered_map<std::string, std::string> emptyFields;

	uint32_t requestId;
	uint32_t requestTimeoutMs = 0;

	HttpResponse_cb responseCallback;
	HttpFailure_cb failureCallback;

	boost::beast::http::request<boost::beast::http::string_body> buildBasicRequest(
	    const HttpUrl& httpUrl, std::unordered_map<std::string, std::string>& fields)
	{
		boost::beast::http::request<boost::beast::http::string_body> request;

		request.version(11);
		request.prepare_payload();
		request.keep_alive(false);

		request.set(boost::beast::http::field::host, httpUrl.host);

		for (auto& field : fields) {
			request.insert(field.first, field.second);
		}

		request.target(httpUrl.target);

		return request;
	}

	void setUniqueRequestId() { requestId++; }

	void doRequest(const HttpUrl& httpUrl, boost::beast::http::request<boost::beast::http::string_body>& request,
	               bool skipBody = false)
	{
		std::shared_ptr<HttpConnectionBase> httpConnection;

		if (httpUrl.isProtocolSecure()) {
			boost::asio::ssl::context sslContext{boost::asio::ssl::context::tlsv12_client};
			sslContext.set_default_verify_paths();

			httpConnection = std::make_shared<HttpsConnection>(
			    context, requestId, sslContext,
			    std::bind(&Request::requestSuccessCallback, this, std::placeholders::_1),
			    std::bind(&Request::requestFailureCallback, this, std::placeholders::_1));
		} else {
			httpConnection = std::make_shared<HttpConnection>(
			    context, requestId, std::bind(&Request::requestSuccessCallback, this, std::placeholders::_1),
			    std::bind(&Request::requestFailureCallback, this, std::placeholders::_1));
		}

		if (requestTimeoutMs > 0) {
			httpConnection->setTimeout(requestTimeoutMs);
		}

		httpConnection->create(request, httpUrl.host, httpUrl.port, skipBody);
	}

	void requestSuccessCallback(HttpResponse_ptr response)
	{
		if (responseCallback) {
			responseCallback(response);
		} else {
			std::cout << "HTTP response received (" << response->responseTimeMs
			          << "ms) but Request has no responseCallback" << std::endl;
		}
	}

	void requestFailureCallback(HttpResponse_ptr response)
	{
		if (failureCallback) {
			failureCallback(response);
		} else {
			std::cout << "HTTP failure but Request has no failureCallback. Failure reason: " << response->errorMessage
			          << std::endl;
		}
	}

	void onError(const std::string& reason) { std::cout << "Could not complete HTTP request: " << reason << std::endl; }
};

} // namespace HttpClientLib

// Restore the warning settings to their previous state
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif // HTTPCLIENT_H
