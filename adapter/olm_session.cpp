#include "olm_session.h"
#include <sstream>

namespace adapter {
OlmSession *createSession(Runtime *runtime) {
  size_t sessionSize = olm_session_size();
  OlmSession *olmSession = (OlmSession *)malloc(sessionSize);

  if (olmSession) {
    // init session object
    olmSession = olm_session(olmSession);
  } else {
    detail::throwJSError(*runtime, "createSession(): failure - OOM");
  }

  if (!olmSession) {
    detail::throwJSError(*runtime,
                         "createSession(): failure - init session OOM");
  }

  return olmSession;
}

void freeSession(Runtime *runtime, OlmSession *olmSession) {
  if (!olmSession) {
    detail::throwJSError(*runtime, "freeSession(): failure - null Session ptr");
  } else {
    olm_clear_session(olmSession);
    free(olmSession);
  }
}

void createOutbound(Runtime *runtime, OlmSession *olmSession,
                    OlmAccount *olmAccount, char *identityKey,
                    size_t identityKeyLength, char *oneTimeKey,
                    size_t oneTimeKeyLength) {
  size_t randomSize = olm_create_outbound_session_random_length(olmSession);
  uint8_t *randomBuffPtr = (uint8_t *)malloc(randomSize * sizeof(uint8_t));

  size_t sessionResult = olm_create_outbound_session(
      olmSession, olmAccount, identityKey, identityKeyLength, oneTimeKey,
      oneTimeKeyLength, (void *)randomBuffPtr, randomSize);
  if (sessionResult == olm_error()) {
    detail::throwJSError(*runtime, std::to_string(sessionResult).c_str());
  }

  if (randomBuffPtr) {
    memset(randomBuffPtr, 0, randomSize);
    free(randomBuffPtr);
  }
}

void createInbound(Runtime *runtime, OlmSession *olmSession,
                   OlmAccount *olmAccount, char *message,
                   size_t messageLength) {
  size_t sessionResult;

  if (!message) {
    detail::throwJSError(*runtime, "createInbound(): failure - null message");
  } else {
    sessionResult = olm_create_inbound_session(olmSession, olmAccount,
                                               (void *)message, messageLength);

    if (sessionResult == olm_error()) {
      detail::throwJSError(*runtime, std::to_string(sessionResult).c_str());
    }
  }
}

string encrypt(Runtime *runtime, OlmSession *olmSession, char *clearMessage,
               size_t clearMessageLength) {
  
  size_t messageType = olm_encrypt_message_type(olmSession);
  
  size_t randomLength = olm_encrypt_random_length(olmSession);
  uint8_t *randomBuff = (uint8_t *)malloc(randomLength * sizeof(uint8_t));

  size_t encryptedMessageLength =
      olm_encrypt_message_length(olmSession, clearMessageLength);

  void *encryptedMessage = malloc(encryptedMessageLength * sizeof(uint8_t));

  size_t result = olm_encrypt(olmSession, (void const *)clearMessage,
                              clearMessageLength, randomBuff, randomLength,
                              encryptedMessage, encryptedMessageLength);
  if (result == olm_error()) {
    detail::throwJSError(*runtime, std::to_string(result).c_str());
  }

  std::ostringstream json;
  json << "{ \"type\":" << messageType << ",\"body\": \"" << reinterpret_cast<char *>(encryptedMessage) << "\" } ";

  free(encryptedMessage);

  return json.str();
}

string decrypt(Runtime *runtime, OlmSession *sessionPtr,
               size_t cipherMessageType, char *cipherMessage,
               size_t cipherMessageLength) {
  uint8_t *plainTextMessage = NULL;
  char *tempEncryptedMessage = NULL;

  if (!sessionPtr) {
    detail::throwJSError(*runtime, "decrypt(): failure - null Session ptr");
  } else if (!cipherMessage) {
    detail::throwJSError(*runtime,
                         "decrypt(): failure - invalid encrypted message");
  } else {
    tempEncryptedMessage =
        static_cast<char *>(malloc(cipherMessageLength * sizeof(uint8_t)));
    memcpy(tempEncryptedMessage, cipherMessage, cipherMessageLength);

    size_t maxPlainTextLength = olm_decrypt_max_plaintext_length(
        sessionPtr, static_cast<size_t>(cipherMessageType),
        static_cast<void *>(tempEncryptedMessage), cipherMessageLength);

    if (maxPlainTextLength == olm_error()) {
      detail::throwJSError(*runtime, std::to_string(maxPlainTextLength).c_str());
    } else {
      plainTextMessage =
          static_cast<uint8_t *>(malloc(maxPlainTextLength * sizeof(uint8_t)));

      memcpy(tempEncryptedMessage, cipherMessage, cipherMessageLength);
      size_t plaintextLength = olm_decrypt(
          sessionPtr, cipherMessageType, (void *)tempEncryptedMessage,
          cipherMessageLength, plainTextMessage, maxPlainTextLength);

      if (plaintextLength == olm_error()) {
        detail::throwJSError(*runtime, std::to_string(plaintextLength).c_str());
      }

      plainTextMessage[plaintextLength] = '\0';
    }
  }

  return reinterpret_cast<char *>(plainTextMessage);
}
} // namespace adapter