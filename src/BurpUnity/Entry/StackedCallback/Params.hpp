#pragma once

#include <functional>

namespace BurpUnity {
  namespace Entry {
    namespace StackedCallback {

      struct Params {

        using f_callback = std::function<void()>;

        const char * label;
        const int line;
        const f_callback cb;

      };

    }
  }
}
