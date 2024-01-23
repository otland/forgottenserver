// Copyright 2023 The Forgotten Server Authors.All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found
// in the LICENSE file.

#include "otpch.h"

#include "httpclient.h"

#include "httpclientlib.h"

#include "tasks.h"
extern Dispatcher g_dispatcher;

void HttpClient::threadMain()
{
	HttpClientLib::Request requestsHandler(
	    [this](const HttpClientLib::HttpResponse_ptr& response) { clientRequestSuccessCallback(response); },
	    [this](const HttpClientLib::HttpResponse_ptr& response) { clientRequestFailureCallback(response); });

	std::unique_lock<std::mutex> requestLockUnique(requestLock, std::defer_lock);
	while (getState() != THREAD_STATE_TERMINATED) {
		requestLockUnique.lock();

		if (pendingRequests.empty() && pendingResponses.empty()) {
			requestSignal.wait(requestLockUnique);
		}

		if (!pendingRequests.empty() || !pendingResponses.empty()) {
			bool shouldUnlock = false;

			if (!pendingRequests.empty()) {
				HttpClientLib::HttpRequest_ptr pendingRequest = std::move(pendingRequests.front());
				pendingRequests.pop_front();

				shouldUnlock = true;
				dispatchRequest(requestsHandler, pendingRequest);
			}

			if (!pendingResponses.empty()) {
				HttpClientLib::HttpResponse_ptr pendingResponse = std::move(pendingResponses.front());
				pendingResponses.pop_front();

				requestLockUnique.unlock();
				shouldUnlock = false;
				processResponse(pendingResponse);
			}

			if (shouldUnlock) {
				requestLockUnique.unlock();
			}
		} else {
			requestLockUnique.unlock();
		}
	}
}

void HttpClient::dispatchRequest(HttpClientLib::Request& requestsHandler, HttpClientLib::HttpRequest_ptr& request)
{
	bool succesfullyDispatched = false;
	switch (request->method) {
		case HttpClientLib::HttpMethod::HTTP_CONNECT:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.connect(request->url, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_TRACE:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.trace(request->url, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_OPTIONS:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.options(request->url, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_HEAD:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.head(request->url, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_DELETE:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.delete_(request->url, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_GET:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.get(request->url, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_POST:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.post(request->url, request->data, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_PATCH:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.patch(request->url, request->data, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_PUT:
			requestsHandler.setTimeout(request->timeout);
			succesfullyDispatched = requestsHandler.put(request->url, request->data, request->fields);
			break;

		case HttpClientLib::HttpMethod::HTTP_NONE:
		default:
			break;
	}

	if (request->method != HttpClientLib::HTTP_NONE && succesfullyDispatched) {
		requests.emplace(std::make_pair(requestsHandler.getRequestId(), std::move(request)));
	}
}

void HttpClient::clientRequestSuccessCallback(const HttpClientLib::HttpResponse_ptr& response)
{
	// std::cout << std::string("HTTP Response received: " + std::to_string(response->statusCode) + " (" +
	// std::to_string(response->responseTimeMs) + "ms) id " + std::to_string(response->requestId)) << std::endl;
	addResponse(response);

	std::string headerStr(reinterpret_cast<char*>(response->headerData.data()), response->headerData.size());
	std::string bodyStr(reinterpret_cast<char*>(response->bodyData.data()), response->bodyData.size());

	// Print the string to the console
	// std::cout << headerStr << std::endl;
	// std::cout << bodyStr << std::endl;
}

void HttpClient::clientRequestFailureCallback(const HttpClientLib::HttpResponse_ptr& response)
{
	std::cout << std::string("HTTP Response failed (" + response->errorMessage + ")") << std::endl;
	addResponse(response);
}

void HttpClient::processResponse(const HttpClientLib::HttpResponse_ptr& response)
{
	auto httpRequestIt = requests.find(response->requestId);
	if (httpRequestIt == requests.end()) {
		return;
	}

	HttpClientLib::HttpRequest_ptr& httpRequest = httpRequestIt->second;

	if (httpRequest->callbackData.isLuaCallback()) {
		luaClientRequestCallback(httpRequest->callbackData);
	}
	
	if (httpRequest->callbackData.callbackFunction) {
		g_dispatcher.addTask(createTask(std::bind(httpRequest->callbackData.callbackFunction, response)));
	}

	requests.erase(response->requestId);
}

void HttpClient::luaClientRequestCallback(HttpClientLib::HttpRequestCallbackData &callbackData)
{
	LuaScriptInterface *scriptInterface = callbackData.scriptInterface;
	int32_t callbackId = callbackData.callbackId;

	lua_State *luaState = scriptInterface->getLuaState();
	if (!luaState) {
		return;
	}

	if (callbackId > 0) {
		callbackData.callbackFunction = [callbackId, scriptInterface](const HttpClientLib::HttpResponse_ptr &response) {
			lua_State *luaState = scriptInterface->getLuaState();
			if (!luaState) {
				return;
			}

			if (!LuaScriptInterface::reserveScriptEnv()) {
				luaL_unref(luaState, LUA_REGISTRYINDEX, callbackId);
				std::cout << "[Error - HttpClient::luaClientRequestCallback] Call stack overflow" << std::endl;
				return;
			}

			// push function
			lua_rawgeti(luaState, LUA_REGISTRYINDEX, callbackId);

			// push parameters
			lua_createtable(luaState, 0, 11);

			LuaScriptInterface::setField(luaState, "requestId", response->requestId);
			LuaScriptInterface::setField(luaState, "version", response->version);
			LuaScriptInterface::setField(luaState, "statusCode", response->statusCode);
			LuaScriptInterface::setField(luaState, "location", response->location);
			LuaScriptInterface::setField(luaState, "contentType", response->contentType);
			LuaScriptInterface::setField(luaState, "responseTimeMs", response->responseTimeMs);
			LuaScriptInterface::setField(luaState, "headerData", response->headerData);
			LuaScriptInterface::setField(luaState, "bodySize", response->bodySize);
			LuaScriptInterface::setField(luaState, "bodyData", response->bodyData);
			LuaScriptInterface::setField(luaState, "success", response->success);
			LuaScriptInterface::setField(luaState, "errorMessage", response->errorMessage);

			LuaScriptInterface::setMetatable(luaState, -1, "HttpResponse");

			int parameter = luaL_ref(luaState, LUA_REGISTRYINDEX);
			lua_rawgeti(luaState, LUA_REGISTRYINDEX, parameter);

			ScriptEnvironment *env = scriptInterface->getScriptEnv();
			auto scriptId = env->getScriptId();
			env->setScriptId(scriptId, scriptInterface);

			scriptInterface->callFunction(1); // callFunction already reset the reserved
			                                  // script env (resetScriptEnv)

			// free resources
			luaL_unref(luaState, LUA_REGISTRYINDEX, callbackId);
			luaL_unref(luaState, LUA_REGISTRYINDEX, parameter);
		};
	}
}

void HttpClient::addResponse(const HttpClientLib::HttpResponse_ptr& response)
{
	bool signal = false;
	requestLock.lock();
	if (getState() == THREAD_STATE_RUNNING) {
		signal = pendingResponses.empty();
		pendingResponses.emplace_back(response);
	}
	requestLock.unlock();

	if (signal) {
		requestSignal.notify_one();
	}
}

void HttpClient::addRequest(const HttpClientLib::HttpRequest_ptr& request)
{
	bool signal = false;
	requestLock.lock();
	if (getState() == THREAD_STATE_RUNNING) {
		signal = pendingRequests.empty();
		pendingRequests.emplace_back(request);
	}
	requestLock.unlock();

	if (signal) {
		requestSignal.notify_one();
	}
}

void HttpClient::shutdown()
{
	requestLock.lock();
	setState(THREAD_STATE_TERMINATED);
	requestLock.unlock();
	requestSignal.notify_one();
}
