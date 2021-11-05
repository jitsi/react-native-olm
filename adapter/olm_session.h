#include <jsi/jsi.h>
#include <stdint.h>
#include <string>

#include "olm/olm.h"

using namespace facebook::jsi;
using namespace std;

namespace adapter {
OlmSession *createSession(Runtime *runtime);
void freeSession(Runtime *runtime, OlmSession *olmSession);
void createOutbound(Runtime *runtime, OlmSession *olmSession,
                    OlmAccount *olmAccount, char *identityKey,
                    size_t indentityKeyLenght, char *oneTimeKey,
                    size_t oneTimeKeyLenght);
void createInbound(Runtime *runtime, OlmSession *olmSession,
                   OlmAccount *olmAccount, char *message, size_t messageLength);
string decrypt(Runtime *runtime, OlmSession *olmSession,
               size_t cipherMessageType, char *cipherMessage,
               size_t cipherMessageLength);
string encrypt(Runtime *runtime, OlmSession *olmSession, char *clearMessage,
               size_t clearMessageLength);
} // namespace adapter