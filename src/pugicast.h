// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PUGICAST_H
#define FS_PUGICAST_H

namespace pugi {

template <class T>
T cast(const char* str);

template <>
inline float cast(const char* str)
{
	return std::strtof(str, nullptr);
}
template <>
inline double cast(const char* str)
{
	return std::strtod(str, nullptr);
}
template <>
inline long cast(const char* str)
{
	return std::strtol(str, nullptr, 0);
}
template <>
inline long long cast(const char* str)
{
	return std::strtoll(str, nullptr, 0);
}
template <>
inline unsigned long cast(const char* str)
{
	return std::strtoul(str, nullptr, 0);
}
template <>
inline unsigned long long cast(const char* str)
{
	return std::strtoull(str, nullptr, 0);
}

template <>
inline char cast(const char* str)
{
	return static_cast<char>(cast<long>(str));
}
template <>
inline signed char cast(const char* str)
{
	return static_cast<signed char>(cast<long>(str));
}
template <>
inline short cast(const char* str)
{
	return static_cast<short>(cast<long>(str));
}
template <>
inline int cast(const char* str)
{
	return static_cast<int>(cast<long>(str));
}
template <>
inline unsigned char cast(const char* str)
{
	return static_cast<unsigned char>(cast<unsigned long>(str));
}
template <>
inline unsigned short cast(const char* str)
{
	return static_cast<unsigned short>(cast<unsigned long>(str));
}
template <>
inline unsigned int cast(const char* str)
{
	return static_cast<unsigned int>(cast<unsigned long>(str));
}

} // namespace pugi

#endif // FS_PUGICAST_H
