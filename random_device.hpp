/*=============================================================================
  Copyright (C) cpprefjp https://cpprefjp.github.io/reference/random/random_device.html
  CC 3.0 BY (See https://creativecommons.org/licenses/by/3.0/deed.ja)
=============================================================================*/
#pragma once

#include <random>

#if defined(__MINGW32__) && defined(__GNUC__) && !defined(__clang__)

#include <system_error>
#include <limits>
#include <string>
#include <Windows.h>
#include <wincrypt.h>

namespace workaround_mingw_gcc {
class random_device {
private:
  class crypt_context {
  private:
    HCRYPTPROV prov_;
  public:
    crypt_context(DWORD prov_type, LPCTSTR container = nullptr, LPCTSTR provider = nullptr, DWORD flags = 0) {
      const auto success = ::CryptAcquireContext(&this->prov_, container, provider, prov_type, flags);
      if (!success) {
        throw std::system_error(
          std::error_code(::GetLastError(), std::system_category()),
          "CryptAcquireContext:(" + std::to_string(success) + ')'
        );
      }
    }
    crypt_context(const crypt_context&) = delete;
    void operator=(const crypt_context&) = delete;
    ~crypt_context() noexcept {
      ::CryptReleaseContext(this->prov_, 0);
    }
    //HCRYPTPROV& get() noexcept { return this->prov_; }
    const HCRYPTPROV& get() const noexcept { return this->prov_; }
  };
  crypt_context prov_;

public:
  using result_type = unsigned int;
  explicit random_device(const std::string& /*token*/ = "workaround_mingw_gcc ")
  : prov_(PROV_RSA_FULL)
  {}
  random_device(const random_device&) = delete;
  void operator=(const random_device&) = delete;
  //~random_device() = default;
  double entropy() const noexcept { return 0.0; }
  result_type operator()() {
    result_type re;
    const auto success = ::CryptGenRandom(this->prov_.get(), sizeof(re), reinterpret_cast<BYTE*>(&re));
    if (!success) {
      throw std::system_error(
        std::error_code(::GetLastError(), std::system_category()),
        "CryptGenRandom:(" + std::to_string(success) + ')'
      );
    }
    return re;
  }
  static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
  static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
};
} // namespace workaround_mingw_gcc

namespace cpprefjp {
using workaround_mingw_gcc::random_device;
}

#else //defined(__MINGW32__) && defined(__GNUC__) && !defined(__clang__)

namespace cpprefjp {
using std::random_device;
}

#endif //defined(__MINGW32__) && defined(__GNUC__) && !defined(__clang__)
