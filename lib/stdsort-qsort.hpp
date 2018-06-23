/* Copyright (C) 2017-2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
   Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
   This file is part of t-kawa-qsort.
   Written by Koichi Murase <myoga.murase@gmail.com>

   t-kawa-qsort is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   t-kawa-qsort is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with t-kawa-qsort.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef QSORT_LIB_STD_SORT_H
#define QSORT_LIB_STD_SORT_H
#include <cstddef>
#include <cstring>
#include <memory>
#include <algorithm>
#include <numeric>

namespace tkawa::qsort {

enum class byte : unsigned char {};

typedef int cmp_t(const void *, const void *);

template<typename Unsigned, typename Compare>
bool qsort_stdsort_impl(void* a, std::size_t n, std::size_t es, Compare compare) {
  if (es != sizeof(Unsigned)) return false;

  Unsigned* const begin = reinterpret_cast<Unsigned*>(a);
  Unsigned* const end = begin + n;
  std::sort(begin, end, [compare] (Unsigned const& lhs, Unsigned const& rhs) {
    return compare(&lhs, &rhs) < 0;
  });
  return true;
}

template<typename Compare>
void qsort_stdsort(void* a, std::size_t n, std::size_t es, Compare compare) {
  byte* const base = reinterpret_cast<byte*>(a);

  if (qsort_stdsort_impl<std::uint8_t>(a, n, es, compare)) return;
  if (qsort_stdsort_impl<std::uint16_t>(a, n, es, compare)) return;
  if (qsort_stdsort_impl<std::uint32_t>(a, n, es, compare)) return;
  if (qsort_stdsort_impl<std::uint64_t>(a, n, es, compare)) return;

  byte* const buffer = (byte*) std::malloc(n * sizeof(byte*) + es);
  byte** table = (byte**) buffer;
  byte* tmp = (byte*) (buffer + n * sizeof(byte*));

  for (std::size_t i = 0; i < n; i++) table[i] = base + es * i;
  std::sort(&table[0], &table[n], [compare] (byte* lhs, byte* rhs) {
    return compare(lhs, rhs) < 0;
  });

  byte* ip = base;
  for (std::size_t i = 0; i < n; i++, ip += es) {
    byte* kp = table[i];
    if (kp != ip) {
      std::size_t j = i;
      byte* jp = ip;
      std::memcpy(&tmp[0], ip, es);

      do {
        std::size_t k = (kp - (byte*) base) / es;
        table[j] = jp;
        memcpy(jp, kp, es);
        j = k;
        jp = kp;
        kp = table[k];
      } while (kp != ip);

      table[j] = jp;
      std::memcpy(jp, &tmp[0], es);
    }
  }

  std::free(buffer);
}

}

#endif
