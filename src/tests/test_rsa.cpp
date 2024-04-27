#define BOOST_TEST_MODULE rsa

#include "../otpch.h"

#include "../rsa.h"

#include <boost/test/unit_test.hpp>
#include <openssl/bn.h>
#include <openssl/core_names.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

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

BOOST_AUTO_TEST_CASE(test_rsa_load_pem)
{
	using namespace std::string_view_literals;

	// fixtures/testrsa.pem contains a private key copied from OpenSSL test suite:
	// https://github.com/openssl/openssl/blob/10203a34725ec75136b03d64fd2126b321419ac1/test/testrsa.pem
	EVP_PKEY* pkey = tfs::rsa::loadPEM("src/tests/fixtures/testrsa.pem");

	// the expected values can be obtained with the following command:
	// $ openssl pkey -in testrsa.pem -noout -text

	// RSA Private-Key: (512 bit)
	// modulus:
	//     00:aa:db:7a:a9:2e:46:4f:15:71:19:96:16:6b:4f:
	//     f8:bb:e2:30:1d:fe:e9:d8:b3:59:6d:c3:c1:a7:df:
	//     ce:7c:87:18:01:70:50:9f:c8:4e:fd:17:b5:bb:02:
	//     ca:5d:d0:a3:22:86:86:b3:80:cb:74:6f:3c:ae:4c:
	//     df:c8:ae:5d:3d
	// publicExponent: 65537 (0x10001)
	// privateExponent:
	//     67:77:27:cd:a1:d7:33:f6:f1:19:a4:79:09:1d:51:
	//     ac:3d:6a:14:10:15:7e:84:05:88:e1:fd:b8:f2:60:
	//     31:aa:00:ba:84:04:8a:c3:c7:55:c6:43:29:c3:af:
	//     e3:01:20:eb:f4:c8:9c:02:17:06:71:22:82:da:af:
	//     47:3b:b2:a1
	// prime1:
	//     00:f7:20:67:35:2a:df:25:68:a0:06:a8:a2:ff:14:
	//     6d:73:f1:a8:79:70:a3:a1:81:32:a9:ef:db:4c:73:
	//     91:fe:99
	// prime2:
	//     00:b0:fe:01:9c:86:90:4c:cf:12:9a:f7:29:1d:9c:
	//     10:25:d9:9d:1c:79:06:28:0b:37:ec:37:f1:8b:ae:
	//     c7:6e:45
	// exponent1:
	//     00:9e:64:69:62:1d:71:5a:d3:bd:45:a7:4e:99:79:
	//     69:51:d8:00:e8:9d:36:73:ac:90:61:75:37:b4:bf:
	//     9a:42:79
	// exponent2:
	//     00:86:5f:92:21:a0:18:15:f8:70:8b:ae:72:4c:c4:
	//     9b:8d:e0:f5:63:14:8f:13:ff:d0:69:4a:df:db:c8:
	//     ca:28:71
	// coefficient:
	//     00:9b:29:9e:84:54:53:d0:cb:ea:5b:9c:40:28:0c:
	//     7b:3e:09:2d:f0:75:4a:c1:58:5c:d8:bc:06:28:71:
	//     34:0d:93

	// then convert the output from hexadecimal to decimal

	auto actualModulus = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_N);
	auto expectedModulus =
	    "8948525014013601095744212848895545067646997190667198465354220334608418294695300148587980446548438816958344948457547931370590147748529646338631264220634429"sv;
	BOOST_TEST(actualModulus == expectedModulus, "expected n = " << expectedModulus << ", got " << actualModulus);

	size_t publicExponent;
	EVP_PKEY_get_size_t_param(pkey, OSSL_PKEY_PARAM_RSA_E, &publicExponent);
	BOOST_TEST(publicExponent == 65537, "expected e = 65537, got " << publicExponent);

	auto actualPrivateExponent = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_D);
	auto expectedPrivateExponent =
	    "5418925373928586701966836677512206800734033866457254764400459929804603458528232352199600575306819650618162351643553279676157387691020051542771655878881953"sv;
	BOOST_TEST(actualPrivateExponent == expectedPrivateExponent,
	           "expected d = " << expectedPrivateExponent << ", got " << actualPrivateExponent);

	auto actualPrime1 = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_FACTOR1);
	auto expectedPrime1 = "111778525019405512256411851490149734274088355291175375383640373933750116023961"sv;
	BOOST_TEST(actualPrime1 == expectedPrime1, "expected p = " << expectedPrime1 << ", got " << actualPrime1);

	auto actualPrime2 = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_FACTOR2);
	auto expectedPrime2 = "80055851626777829514240655963926657772512853373739498240341188860942649617989"sv;
	BOOST_TEST(actualPrime2 == expectedPrime2, "expected q = " << expectedPrime2 << ", got " << actualPrime2);

	auto actualExponent1 = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_EXPONENT1);
	auto expectedExponent1 = "71642842111175802101569797547091560312237077742432849275826081558313221898873"sv;
	BOOST_TEST(actualExponent1 == expectedExponent1,
	           "expected dmp1 = " << expectedExponent1 << ", got " << actualExponent1);

	auto actualExponent2 = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_EXPONENT2);
	auto expectedExponent2 = "60778780742816388991112777181456807362698163365179707256151734027573164386417"sv;
	BOOST_TEST(actualExponent2 == expectedExponent2,
	           "expected dmq1 = " << expectedExponent2 << ", got " << actualExponent2);

	// note: there is only one coefficient, but OSSL_PKEY_PARAM_RSA_COEFFICIENT is not a valid param, it must be
	// OSSL_PKEY_PARAM_RSA_COEFFICIENT1
	auto actualCoefficient = get_bignum_param_as_string(pkey, OSSL_PKEY_PARAM_RSA_COEFFICIENT1);
	auto expectedCoefficient = "70182026303578669171175542309639729186931571676068678104669900696511625563539"sv;
	BOOST_TEST(actualCoefficient == expectedCoefficient,
	           "expected iqmp = " << expectedCoefficient << ", got " << actualCoefficient);
}

BOOST_AUTO_TEST_CASE(test_rsa_decrypt)
{
	using namespace std::string_literals;

	tfs::rsa::loadPEM("src/tests/fixtures/testrsa.pem");

	// the public key can be extracted from the private key with the following command:
	// $ openssl pkey -in testrsa.pem -pubout
	// it is also available in the OpenSSL test suite:
	// https://github.com/openssl/openssl/blob/933f57dfe21657f7aba8f13e0cdb3b02dd64fcc3/test/testrsapub.pem

	// the encrypted message was generated by encrypting 64 times the character 'x' (0x78) with the following command:

	// clang-format off
	// $ head -c 64 < /dev/zero | tr '\0' 'x' | openssl pkeyutl -encrypt -inkey testrsa.pem -pkeyopt rsa_padding_mode:none | xxd
	// clang-format on

	// then interleave with \x for every byte (2 digits)
	auto plaintext = std::string(64, 'x');
	auto encrypted =
	    "\x45\xe2\x7e\xa7\x76\x2f\x98\xca\xe7\xef\xd9\x3e\xb6\x87\x3d\x03\xa1\x70\x04\x27\x99\xbd\xf7\x5c\x9a\x08\x46\x32\x0d\xcc\x36\xdb\x44\x31\xf0\xa4\x87\x7e\x37\x82\x88\x59\xc9\x9c\xf8\xad\x5c\xae\x1a\x2b\x79\x0c\xcc\x9c\xbc\xb0\xb9\xd6\x7c\x6e\x51\x96\xd9\x93"s;

	tfs::rsa::decrypt(reinterpret_cast<uint8_t*>(encrypted.data()), encrypted.size());
	BOOST_TEST(encrypted == plaintext, "expected '" << plaintext << "', got '" << encrypted << "'");
}
