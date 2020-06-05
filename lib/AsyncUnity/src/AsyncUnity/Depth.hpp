#pragma once

#include <unity.h>
#include <memory>
#include <stdio.h>
#include "./Globals/Error.hpp"
#include "./Timeout.hpp"

namespace AsyncUnity {

  template <size_t MAX_DEPTH, size_t LABEL_BUFFER_SIZE, unsigned long DEFAULT_TIMEOUT>
  class Depth {
    
    public:

      size_t highUsed = 0;

      const Error * push(const char * label, const long timeout) {
        if (MAX_DEPTH == _current) {
          return &Globals::maxDepthError;
        }
        new(&(_frames[_current++])) Frame(label, timeout);
        highUsed = _current > highUsed ? _current : highUsed;
        return nullptr;
      }

      const Error * pop() {
        if (0 == _current) {
          return &Globals::minDepthError;
        }
        _current--;
        return nullptr;
      }

      const char * getLabel(const char * field) {
        _resetLabel();
        _concatenateLabel(field);
        return _currentLabel;
      }

      const char * getLabel(const char * field1, const char * field2) {
        _resetLabel();
        _concatenateLabel(field1);
        _concatenateLabel(field2);
        return _currentLabel;
      }

      const long getTimeout(const long timeout) {
        if (0 != timeout) {
          // timeout has been overriden
          return timeout;
        }
        unsigned long ret = DEFAULT_TIMEOUT;
        for (int i = 0; i < _current; i++) {
          // only apply next timeout if it is an override
          ret = _frames[i].timeout || ret;
        }
        return ret;
      }

    private:

      struct Frame {
        Frame(const char * label, const long timeout) :
          label(label),
          timeout(timeout)
        {}
        const char * label;
        const long timeout;
      };

      std::allocator<Frame> allocator;
      Frame * _frames = allocator.allocate(MAX_DEPTH);
      size_t _current = 0;

      // we use 2 buffers to make concatenation simpler
      char _label1[LABEL_BUFFER_SIZE];
      char _label2[LABEL_BUFFER_SIZE];
      char * _currentLabel;

      void _resetLabel() {
        _currentLabel = _label1;
        _currentLabel[0] = 0;
        for (int i = 0; i < _current; i++) {
          _concatenateLabel(_frames[i].label);
        }
      }

      void  _concatenateLabel(const char * field) {
        char * destination = _currentLabel == _label1 ? _label2 : _label1;
        snprintf(destination, LABEL_BUFFER_SIZE, "%s[%s]", _currentLabel, field);
        _currentLabel = destination;
      }

  };

}