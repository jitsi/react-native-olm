#include "olm_session_host.h"

namespace hostObjects {

SessionHostObject::SessionHostObject(Runtime *runtime) : HostObject() {
  this->runtime = runtime;
  olmSession = adapter::createSession(runtime);
}

OlmSession *SessionHostObject::getOlmSession() { return olmSession; }

void SessionHostObject::free() { adapter::freeSession(runtime, olmSession); }

void SessionHostObject::createOutbound(OlmAccount *olmAccount,
                                       string identityKey, string oneTimeKey) {

  size_t identityKeyLength = identityKey.length();
  char *identityKeyArray = new char[identityKeyLength + 1];
  strcpy(identityKeyArray, identityKey.c_str());

  size_t oneTimeKeyLength = oneTimeKey.length();
  char *oneTimeKeyArray = new char[oneTimeKeyLength + 1];
  strcpy(oneTimeKeyArray, oneTimeKey.c_str());
  adapter::createOutbound(runtime, olmSession, olmAccount, identityKeyArray,
                          identityKeyLength, oneTimeKeyArray, oneTimeKeyLength);
}

void SessionHostObject::createInbound(OlmAccount *olmAccount, string message) {

  size_t messageLength = message.length();
  char *messageArray = new char[messageLength + 1];
  strcpy(messageArray, message.c_str());

  adapter::createInbound(runtime, olmSession, olmAccount, messageArray,
                         messageLength);
}

string SessionHostObject::encrypt(string message) {
  size_t messageLength = message.length();
  char *messageArray = new char[messageLength + 1];
  strcpy(messageArray, message.c_str());

  return adapter::encrypt(runtime, olmSession, messageArray, messageLength);
}

string SessionHostObject::decrypt(size_t type, string message) {
  size_t messageLength = message.length();
  char *messageArray = new char[messageLength + 1];
  strcpy(messageArray, message.c_str());

  return adapter::decrypt(runtime, olmSession, type, messageArray,
                          messageLength);
}

Object SessionHostObject::asJsiObject() {
  auto sharedPtr = std::make_shared<SessionHostObject>(*this);
  return Object::createFromHostObject(*runtime, sharedPtr);
}

Value SessionHostObject::get(Runtime &rt, const PropNameID &sym) {
  auto methodName = sym.utf8(rt);

  if (methodName == "free") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "free"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto sessionJsiObject = thisValue.asObject(runtime);
          auto sessionHostObject =
              sessionJsiObject.getHostObject<SessionHostObject>(runtime).get();
          sessionHostObject->free();

          return Value(true);
        });
  }

  if (methodName == "create_outbound") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "create_outbound"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto sessionJsiObject = thisValue.asObject(runtime);
          auto sessionHostObject =
              sessionJsiObject.getHostObject<SessionHostObject>(runtime).get();
          auto accountJsiObject = arguments[0].asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();

          auto identityKey = arguments[1].asString(runtime).utf8(runtime);
          auto oneTimeKey = arguments[2].asString(runtime).utf8(runtime);

          sessionHostObject->createOutbound(accountHostObject->getOlmAccount(),
                                            identityKey, oneTimeKey);

          return Value(true);
        });
  }

  if (methodName == "create_inbound") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "create_inbound"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto sessionJsiObject = thisValue.asObject(runtime);
          auto sessionHostObject =
              sessionJsiObject.getHostObject<SessionHostObject>(runtime).get();
          auto accountJsiObject = arguments[0].asObject(runtime);
          auto accountHostObject =
              accountJsiObject.getHostObject<AccountHostObject>(runtime).get();

          auto message = arguments[1].asString(runtime).utf8(runtime);

          sessionHostObject->createInbound(accountHostObject->getOlmAccount(),
                                           message);

          return Value(true);
        });
  }

  if (methodName == "encrypt") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "encrypt"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto sessionJsiObject = thisValue.asObject(runtime);
          auto sessionHostObject =
              sessionJsiObject.getHostObject<SessionHostObject>(runtime).get();

          auto message = arguments[0].asString(runtime).utf8(runtime);

          auto encryptedMessage = sessionHostObject->encrypt(message);

          return Value(runtime,
                       String::createFromUtf8(runtime, encryptedMessage));
        });
  }

  if (methodName == "decrypt") {
    return Function::createFromHostFunction(
        *runtime, PropNameID::forAscii(*runtime, "encrypt"), 0,
        [](Runtime &runtime, const Value &thisValue, const Value *arguments,
           size_t count) -> Value {
          auto sessionJsiObject = thisValue.asObject(runtime);
          auto sessionHostObject =
              sessionJsiObject.getHostObject<SessionHostObject>(runtime).get();

          auto type = arguments[0].asNumber();
          auto message = arguments[1].asString(runtime).utf8(runtime);

          auto decryptedMessage = sessionHostObject->decrypt(type, message);

          return Value(runtime,
                       String::createFromUtf8(runtime, decryptedMessage));
        });
  }

  return Value::undefined();
}

void SessionHostObject::set(Runtime &, const PropNameID &, const Value &) {}
} // namespace hostObjects
