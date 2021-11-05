#pragma once

#include <jsi/jsi.h>

#include "olm/olm.h"
#include "olm_account_host.h"
#include "olm_session.h"

using namespace std;
using namespace facebook::jsi;

namespace hostObjects {

class SessionHostObject : public HostObject {
private:
  Runtime *runtime;
  OlmSession *olmSession;

public:
  SessionHostObject(Runtime *runtime);
  OlmSession *getOlmSession();

private:
  void free();
  void createOutbound(OlmAccount *olmAccount, string identityKey,
                      string oneTimeKey);
  void createInbound(OlmAccount *olmAccount, string message);
  string encrypt(string message);
  string decrypt(size_t type, string message);

public:
  Object asJsiObject();
  Value get(Runtime &rt, const PropNameID &sym) override;
  void set(Runtime &, const PropNameID &, const Value &) override;
};
} // namespace hostObjects