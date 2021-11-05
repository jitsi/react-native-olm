#include <jsi/jsi.h>
#include <stdint.h>
#include <string>

#include "olm/olm.h"

using namespace facebook::jsi;
using namespace std;

namespace adapter {
OlmAccount *createAccount(Runtime *runtime);
void freeAccount(Runtime *runtime, OlmAccount *olmAccount);
string identityKeys(Runtime *runtime, OlmAccount *olmAccount);
void generateOneTimeKeys(Runtime *runtime, OlmAccount *olmAccount,
                         uint8_t aNumberOfKeys);
string oneTimeKeys(Runtime *runtime, OlmAccount *olmAccount);
void markOneTimeKeysAsPublished(Runtime *runtime, OlmAccount *olmAccount);
void removeOneTimeKeys(Runtime *runtime, OlmAccount *accountPtr,
                       OlmSession *sessionPtr);
} // namespace adapter