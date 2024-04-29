#define BOOST_TEST_MODULE rsa

#include "../otpch.h"

#include "../rsa.h"

#include <boost/test/unit_test.hpp>
#include <openssl/core_names.h>
#include <openssl/evp.h>

struct PrivateKeyFixture
{
	// contains the private key from key.pem in the root of the repository
	std::string_view privateKey =
	    "-----BEGIN RSA PRIVATE KEY-----\n"
	    "MIICXAIBAAKBgQCbZGkDtFsHrJVlaNhzU71xZROd15QHA7A+bdB5OZZhtKg3qmBWHXzLlFL6AIBZ\n"
	    "SQmIKrW8pYoaGzX4sQWbcrEhJhHGFSrT27PPvuetwUKnXT11lxUJwyHFwkpb1R/UYPAbThW+sN4Z\n"
	    "MFKKXT8VwePL9cQB1nd+EKyqsz2+jVt/9QIDAQABAoGAQovTtTRtr3GnYRBvcaQxAvjIV9ZUnFRm\n"
	    "C7Y3i1KwJhOZ3ozmSLrEEOLqTgoc7R+sJ1YzEiDKbbete11EC3gohlhW56ptj0WDf+7ptKOgqiEy\n"
	    "Kh4qt1sYJeeGz4GiiooJoeKFGdtk/5uvMR6FDCv6H7ewigVswzf330Q3Ya7+jYECQQERBxsga6+5\n"
	    "x6IofXyNF6QuMqvuiN/pUgaStUOdlnWBf/T4yUpKvNS1+I4iDzqGWOOSR6RsaYPYVhj9iRABoKyx\n"
	    "AkEAkbNzB6vhLAWht4dUdGzaREF3p4SwNcu5bJRa/9wCLSHaS9JaTq4lljgVPp1zyXyJCSCWpFnl\n"
	    "0WvK3Qf6nVBIhQJBANS7rK8+ONWQbxENdZaZ7Rrx8HUTwSOS/fwhsGWBbl1Qzhdq/6/sIfEHkfeH\n"
	    "1hoH+IlpuPuf21MdAqvJt+cMwoECQF1LyBOYduYGcSgg6u5mKVldhm3pJCA+ZGxnjuGZEnet3qeA\n"
	    "eb05++112fyvO85ABUun524z9lokKNFh45NKLjUCQGshzV43P+RioiBhtEpB/QFzijiS4L2HKNu1\n"
	    "tdhudnUjWkaf6jJmQS/ppln0hhRMHlk9Vus/bPx7LtuDuo6VQDo=\n"
	    "-----END RSA PRIVATE KEY-----\n";
};

struct Deleter
{
	void operator()(char* ptr) const { OPENSSL_free(ptr); }
	void operator()(BIGNUM* bn) const { BN_free(bn); }
	void operator()(EVP_PKEY_CTX* ctx) const { EVP_PKEY_CTX_free(ctx); }
};

template <class T>
using C_ptr = std::unique_ptr<T, Deleter>;

auto get_bignum_param(const EVP_PKEY* pkey, const char* key_name)
{
	BIGNUM* bn = nullptr;
	EVP_PKEY_get_bn_param(pkey, key_name, &bn);
	return C_ptr<BIGNUM>{bn};
}

auto bignum_as_string(const C_ptr<BIGNUM>& bn)
{
	C_ptr<char> ptr{BN_bn2dec(bn.get())};
	return std::string{ptr.get()};
}

auto get_bignum_param_as_string(const EVP_PKEY* pkey, const char* key_name)
{
	auto bn = get_bignum_param(pkey, key_name);
	return bignum_as_string(bn);
}

BOOST_FIXTURE_TEST_CASE(test_rsa_load_pem, PrivateKeyFixture)
{
	EVP_PKEY* pkey = tfs::rsa::loadPEM(privateKey);

	// the expected values can be obtained with the following command:
	// $ openssl pkey -in key.pem -noout -text

	// RSA Private-Key: (1024 bit)
	// modulus:
	//     00:9b:64:69:03:b4:5b:07:ac:95:65:68:d8:73:53:
	//     bd:71:65:13:9d:d7:94:07:03:b0:3e:6d:d0:79:39:
	//     96:61:b4:a8:37:aa:60:56:1d:7c:cb:94:52:fa:00:
	//     80:59:49:09:88:2a:b5:bc:a5:8a:1a:1b:35:f8:b1:
	//     05:9b:72:b1:21:26:11:c6:15:2a:d3:db:b3:cf:be:
	//     e7:ad:c1:42:a7:5d:3d:75:97:15:09:c3:21:c5:c2:
	//     4a:5b:d5:1f:d4:60:f0:1b:4e:15:be:b0:de:19:30:
	//     52:8a:5d:3f:15:c1:e3:cb:f5:c4:01:d6:77:7e:10:
	//     ac:aa:b3:3d:be:8d:5b:7f:f5
	// publicExponent: 65537 (0x10001)
	// privateExponent:
	//     42:8b:d3:b5:34:6d:af:71:a7:61:10:6f:71:a4:31:
	//     02:f8:c8:57:d6:54:9c:54:66:0b:b6:37:8b:52:b0:
	//     26:13:99:de:8c:e6:48:ba:c4:10:e2:ea:4e:0a:1c:
	//     ed:1f:ac:27:56:33:12:20:ca:6d:b7:ad:7b:5d:44:
	//     0b:78:28:86:58:56:e7:aa:6d:8f:45:83:7f:ee:e9:
	//     b4:a3:a0:aa:21:32:2a:1e:2a:b7:5b:18:25:e7:86:
	//     cf:81:a2:8a:8a:09:a1:e2:85:19:db:64:ff:9b:af:
	//     31:1e:85:0c:2b:fa:1f:b7:b0:8a:05:6c:c3:37:f7:
	//     df:44:37:61:ae:fe:8d:81
	// ...

	// then convert the output from hexadecimal to decimal

	auto actualModulus = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_N);
	auto expectedModulus =
	    "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";
	BOOST_TEST(actualModulus == expectedModulus, "expected n = " << expectedModulus << ", got " << actualModulus);

	size_t publicExponent;
	EVP_PKEY_get_size_t_param(pkey, OSSL_PKEY_PARAM_RSA_E, &publicExponent);
	BOOST_TEST(publicExponent == 65537, "expected e = 65537, got " << publicExponent);

	auto actualPrivateExponent = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_D);
	auto expectedPrivateExponent =
	    "46730330223584118622160180015036832148732986808519344675210555262940258739805766860224610646919605860206328024326703361630109888417839241959507572247284807035235569619173792292786907845791904955103601652822519121908367187885509270025388641700821735345222087940578381210879116823013776808975766851829020659073";
	BOOST_TEST(actualPrivateExponent == expectedPrivateExponent,
	           "expected d = " << expectedPrivateExponent << ", got " << actualPrivateExponent);

	size_t bits;
	EVP_PKEY_get_size_t_param(pkey, OSSL_PKEY_PARAM_RSA_BITS, &bits);
	BOOST_TEST(bits == 1024, "expected 1024 bits, got " << bits);
}

BOOST_FIXTURE_TEST_CASE(test_rsa_decrypt, PrivateKeyFixture)
{
	tfs::rsa::loadPEM(privateKey);

	// the public key can be extracted from the private key with the following command:
	// $ openssl pkey -in key.pem -pubout

	// the encrypted message was generated by encrypting 64 times the character 'x' (0x78) with the following command:
	// $ head -c 128 < /dev/zero | tr '\0' 'x' | openssl pkeyutl -encrypt -inkey key.pem -pkeyopt rsa_padding_mode:none

	// then interleave with \x for every byte (2 digits)

	std::string plaintext(128, 'x');
	std::string encrypted =
	    "\x72\x17\x59\x03\xe4\xe9\xf8\x51\xce\x44\x0f\x83\x35\xbf\x65\xf0\x23\xe9\x80\xfc\x8c\x80\x43\x08\xa4\x0e\xd2\xc1\x1d\x7d"
	    "\x03\x38\xb0\x3b\x0b\xb6\xd1\xf9\xf4\x55\xdc\x71\x12\xc2\x17\x92\xee\xd3\x22\xfa\xd4\x24\xd3\xd5\x05\x5d\x38\x34\xd4\x12"
	    "\xdf\x3b\x0d\xc5\xa8\x59\xe5\x9d\x1f\x92\xb6\x3f\x54\x0a\xe0\x44\xeb\x6e\x55\x0a\x8e\xd0\xd1\xf7\x84\x1d\x3c\x0b\xcc\x3e"
	    "\x2b\x08\x83\x3d\xa7\x83\x67\xb8\x3d\x49\xda\x13\xde\x41\x18\x7f\x42\xb2\x80\x8f\x9b\xe6\xfe\x4b\xb7\xe2\xab\x98\x0f\x4a"
	    "\xdd\x52\xe9\xb1\x5b\xef\x25\x03";

	tfs::rsa::decrypt(reinterpret_cast<uint8_t*>(encrypted.data()), encrypted.size());
	BOOST_TEST(encrypted == plaintext, "expected '" << plaintext << "', got '" << encrypted << "'");
}
