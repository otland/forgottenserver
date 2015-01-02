/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#include "otpch.h"

#include "rsa.h"

RSA::RSA()
{
	mpz_init(m_n);
	mpz_init2(m_d, 1024);
}

RSA::~RSA()
{
	mpz_clear(m_n);
	mpz_clear(m_d);
}

void RSA::setKey(const char* p, const char* q)
{
	std::lock_guard<std::recursive_mutex> lockClass(lock);

	mpz_t m_p, m_q, m_e;
	mpz_init2(m_p, 1024);
	mpz_init2(m_q, 1024);
	mpz_init(m_e);

	mpz_set_str(m_p, p, 10);
	mpz_set_str(m_q, q, 10);

	// e = 65537
	mpz_set_ui(m_e, 65537);

	// n = p * q
	mpz_mul(m_n, m_p, m_q);

	// d = e^-1 mod (p - 1)(q - 1)
	mpz_t p_1, q_1, pq_1;
	mpz_init2(p_1, 1024);
	mpz_init2(q_1, 1024);
	mpz_init2(pq_1, 1024);

	mpz_sub_ui(p_1, m_p, 1);
	mpz_sub_ui(q_1, m_q, 1);

	// pq_1 = (p -1)(q - 1)
	mpz_mul(pq_1, p_1, q_1);

	// m_d = m_e^-1 mod (p - 1)(q - 1)
	mpz_invert(m_d, m_e, pq_1);

	mpz_clear(p_1);
	mpz_clear(q_1);
	mpz_clear(pq_1);

	mpz_clear(m_p);
	mpz_clear(m_q);
	mpz_clear(m_e);
}

void RSA::decrypt(char* msg)
{
	std::lock_guard<std::recursive_mutex> lockClass(lock);

	mpz_t c, m;
	mpz_init2(c, 1024);
	mpz_init2(m, 1024);

	mpz_import(c, 128, 1, 1, 0, 0, msg);

	// m = c^d mod n
	mpz_powm(m, c, m_d, m_n);

	size_t count = (mpz_sizeinbase(m, 2) + 7)/8;
	memset(msg, 0, 128 - count);
	mpz_export(&msg[128 - count], nullptr, 1, 1, 0, 0, m);

	mpz_clear(c);
	mpz_clear(m);
}
