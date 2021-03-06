﻿#pragma once
#include <cstddef>
namespace std_future{
	template <class C>
	constexpr auto size(const C& c) -> decltype(c.size()) { return c.size(); }
	template <class T, std::size_t N>
	constexpr std::size_t size(const T (&)[N]) noexcept { return N; }
}
