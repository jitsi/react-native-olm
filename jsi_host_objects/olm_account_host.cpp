#include "olm_account_host.h"
#include "olm_session_host.h"

namespace hostObjects {

AccountHostObject::AccountHostObject(Runtime *runtime) : HostObject() {
  this->runtime = runtime;
  olmAccount = adapter::createAccount(runtime);
}

OlmAccount *AccountHostObject::getOlmAccount() { return olmAccount; }

void AccountHostObject::free() { adapter::freeAccount(runtime, olmAccount); }

string AccountHostObject::identityKeys() {
  return adapter::identityKeys(runtime, olmAccount);
}

void AccountHostObject::generateOneTimeKeys(uint8_t aNumberOfKeys) {
  adapter::generateOneTimeKeys(runtime, olmAccount, aNumberOfKeys);
}

string AccountHostObject::oneTimeKeys() {
  return adapter::oneTimeKeys(runtime, olmAccount);
}

void AccountHostObject::markOneTimeKeysAsPublished() {
  adapter::markOneTimeKeysAsPublished(runtime, olmAccount);
}

void AccountHostObject::removeOneTimeKeys(OlmSession *olmSession) {
  adapter::removeOneTimeKeys(runtime, olmAccount, olmSession);
}

Object AccountHostObject::asJsiObject() {
  auto sharedPtr = std::make_shared<AccountHostObject>(*this);
  return Object::createFromHostObject(*runtime, sharedPtr);
}

Value AccountHostObject::get(Runtime &rt, const PropNameID &sym) {
  auto methodName = sym.utf8(rt);

  if (methodName == "free") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "free"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();
          accountHostObject->free();

          return Value(true);
        });
  }

  if (methodName == "identity_keys") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "identity_keys"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();
          string key = accountHostObject->identityKeys();

          return Value(runtime, String::createFromUtf8(runtime, key));
        });
  }

  if (methodName == "generate_one_time_keys") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "generate_one_time_keys"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();
          auto aNumberOfKeys = arguments[0].asNumber();
          accountHostObject->generateOneTimeKeys(aNumberOfKeys);

          return Value(true);
        });
  }

  if (methodName == "one_time_keys") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "one_time_keys"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();
          string key = accountHostObject->oneTimeKeys();

          return Value(runtime, String::createFromUtf8(runtime, key));
        });
  }

  if (methodName == "remove_one_time_keys") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "remove_one_time_keys"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();

          auto sessionJsiObject = arguments[0].asObject(runtime);
          auto sessionHostObject =
              sessionJsiObject.getHostObject<SessionHostObject>(runtime).get();

          accountHostObject->removeOneTimeKeys(
              sessionHostObject->getOlmSession());

          return Value(true);
        });
  }

  if (methodName == "mark_keys_as_published") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "mark_keys_as_published"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();

          accountHostObject->markOneTimeKeysAsPublished();
          return Value(true);
        });
  }

  if (methodName == "free") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "free"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto accountJsiObject = thisValue.asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();
          accountHostObject->free();

          return Value(true);
        });
  }

  return Value::undefined();
}

void AccountHostObject::set(Runtime &, const PropNameID &, const Value &) {}
} // namespace hostObjects