#pragma once

#include <jsi/jsi.h>

#include "olm/olm.h"
#include "olm_account.h"

using namespace std;
using namespace facebook::jsi;

namespace hostObjects {

class AccountHostObject : public HostObject {
private:
  Runtime *runtime;
  OlmAccount *olmAccount;

public:
  AccountHostObject(Runtime *runtime);
  OlmAccount *getOlmAccount();

public:
  Object asJsiObject();
  Value get(Runtime &rt, const PropNameID &sym) override;
  void set(Runtime &, const PropNameID &, const Value &) override;

private:
  string identityKeys();
  void free();
  void generateOneTimeKeys(uint8_t aNumberOfKeys);
  string oneTimeKeys();
  void markOneTimeKeysAsPublished();
  void removeOneTimeKeys(OlmSession *olmSession);
};
} // namespace hostObjects
