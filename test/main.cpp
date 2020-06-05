#ifndef NATIVE
#include <Arduino.h>
#endif

#include <AsyncUnity.hpp>
#include <AsyncUnity/Globals.hpp>

#include <MyLib.hpp>

MyLib myLib;

Module module("MyLib", [](Describe & module) {

  module.it("should fail", []() {
    TEST_ASSERT_TRUE(false);
  });

  module.async("callMeNextLoop", [](Async & async, f_done & done) {
    myLib.callMeNextLoop(100, [&](int value) {
      async.it("should pass", [&]() {
        TEST_ASSERT_EQUAL(100, value);
      });
      async.it("should fail", [&]() {
        TEST_ASSERT_EQUAL(10, value);
      });
      // signal that we are done, this
      // should not be done in a function
      // that asserts as the assert may
      // exit that function before it gets
      // there and thus cause a timeout
      // failure
      done();
    });
  });

  module.it("should pass", []() {
    TEST_ASSERT_TRUE(true);
  });

});

void setup()
{
  module.start();
}

void loop()
{
  myLib.loop();
  module.loop();
}

int main()
{
  setup();
  while (module.isRunning())
  {
    loop();
  }

  return module.status;
}

