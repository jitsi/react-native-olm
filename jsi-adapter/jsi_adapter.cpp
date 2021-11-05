#include "jsi_adapter.h"

#include "olm_account_host.h"
#include "olm_session_host.h"
#include <jsi/jsi.h>

using namespace facebook::jsi;
using namespace std;
using namespace hostObjects;

namespace jsiadapter {
void install(Runtime &jsiRuntime) {

  Object module = Object(jsiRuntime);

  auto createOlmAccount = Function::createFromHostFunction(
      jsiRuntime, PropNameID::forAscii(jsiRuntime, "createOlmAccount"), 0,
      [](Runtime &runtime, const Value &thisValue, const Value *arguments,
         size_t count) -> Value {
        auto acountHostObject = AccountHostObject(&runtime);
        auto accountJsiObject = acountHostObject.asJsiObject();

        return move(accountJsiObject);
      });

  module.setProperty(jsiRuntime, "createOlmAccount", move(createOlmAccount));

  auto createOlmSession = Function::createFromHostFunction(
      jsiRuntime, PropNameID::forAscii(jsiRuntime, "createOlmSession"), 0,
      [](Runtime &runtime, const Value &thisValue, const Value *arguments,
         size_t count) -> Value {
        auto sessionHostObject = SessionHostObject(&runtime);
        auto sessionJsiObject = sessionHostObject.asJsiObject();

        return move(sessionJsiObject);
      });

  module.setProperty(jsiRuntime, "createOlmSession", move(createOlmSession));

  jsiRuntime.global().setProperty(jsiRuntime, "_olm", move(module));
}
} // namespace jsiadapter
