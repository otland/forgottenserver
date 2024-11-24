#include "otpch.h"

#include "base64.h"

#include <openssl/evp.h>

std::string tfs::base64::encode(std::string_view input)
{
	std::unique_ptr<BIO, decltype(&BIO_free_all)> b64(BIO_new(BIO_f_base64()), BIO_free_all);
	BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);

	BIO* sink = BIO_new(BIO_s_mem());
	BIO_push(b64.get(), sink);
	BIO_write(b64.get(), input.data(), input.size());
	BIO_flush(b64.get());

	const char* encoded;
	auto len = BIO_get_mem_data(sink, &encoded);
	return {encoded, static_cast<size_t>(len)};
}

std::string tfs::base64::decode(std::string_view input)
{
	std::unique_ptr<BIO, decltype(&BIO_free_all)> b64(BIO_new(BIO_f_base64()), BIO_free_all);
	BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);

	BIO* source = BIO_new_mem_buf(input.data(), input.size()); // read-only source
	BIO_push(b64.get(), source);

	size_t decodedLength = 3 * input.size() / 4;
	auto decoded = std::string(decodedLength, '\0');

	const int len = BIO_read(b64.get(), decoded.data(), decodedLength);
	return decoded.substr(0, len);
}
