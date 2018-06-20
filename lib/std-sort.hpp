#ifndef QSORT_LIB_STD_SORT_H
#define QSORT_LIB_STD_SORT_H
#include <cstddef>
#include <cstring>
#include <memory>
#include <limits>
#include <algorithm>
#include <numeric>

namespace tkawa::qsort {

enum class byte : unsigned char {};

typedef int cmp_t(const void *, const void *);

template<typename Unsigned, typename Compare>
bool qsort_stdsort_impl(void* a, std::size_t n, std::size_t es, Compare compare) {
  constexpr int digits_per_elem = std::numeric_limits<Unsigned>::digits;
  constexpr int digits_per_byte = std::numeric_limits<unsigned char>::digits;
  if (es * digits_per_byte != digits_per_elem) return false;

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
  constexpr int bits = std::numeric_limits<unsigned char>::digits;

  if (qsort_stdsort_impl<std::uint8_t>(a, n, es, compare)) return;
  if (qsort_stdsort_impl<std::uint16_t>(a, n, es, compare)) return;
  if (qsort_stdsort_impl<std::uint32_t>(a, n, es, compare)) return;
  if (qsort_stdsort_impl<std::uint64_t>(a, n, es, compare)) return;

  byte* const buffer = (byte*) std::malloc(n * sizeof(byte*) + es);
  byte** table = (byte**) buffer;
  byte* tmp = (byte*) (buffer + n * sizeof(byte*));

  //auto table = std::make_unique<byte*[]>(n);
  std::size_t i = 0;
  std::generate(&table[0], &table[n], [=, &i] () { return base + es * i++; });
  std::sort(&table[0], &table[n], [compare] (byte* lhs, byte* rhs) {
    return compare(lhs, rhs) < 0;
  });

  //auto tmp = std::make_unique<byte[]>(es);
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
