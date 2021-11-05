#include "olm_account.h"

namespace adapter {
OlmAccount *createAccount(Runtime *runtime) {
  const char *errorMessage = NULL;
  size_t accountSize = olm_account_size();
  OlmAccount *accountPtr = (OlmAccount *)malloc(accountSize);

  if (accountPtr) {
    accountPtr = olm_account(accountPtr);
  }

  if (!accountPtr) {
    errorMessage = "initializeOlmAccountMemory(): failure - OOM";
  } else {
    size_t randomSize = olm_create_account_random_length(accountPtr);

    size_t accountRetCode;

    uint8_t *randomBuffPtr = (uint8_t *)malloc(randomSize);

    accountRetCode =
        olm_create_account(accountPtr, (void *)randomBuffPtr, randomSize);

    if (accountRetCode == olm_error()) {
      errorMessage = olm_account_last_error(accountPtr);
    }

    if (randomBuffPtr) {
      memset(randomBuffPtr, 0, randomSize);
      free(randomBuffPtr);
    }
  }

  if (errorMessage) {
    detail::throwJSError(*runtime, errorMessage);
    if (accountPtr) {
      olm_clear_account(accountPtr);
      free(accountPtr);
    }
  }

  return accountPtr;
}

void freeAccount(Runtime *runtime, OlmAccount *accountPtr) {
  if (!accountPtr) {
    detail::throwJSError(*runtime, "freeOlmAccount: failure accountPtr null");
  } else {
    olm_clear_account(accountPtr);
    free(accountPtr);
  }
}

string identityKeys(Runtime *runtime, OlmAccount *accountPtr) {
  if (!accountPtr) {
    detail::throwJSError(*runtime, "identityKeys(): null OlmAccount ptr");
  } else {
    size_t identityKeysLength = olm_account_identity_keys_length(accountPtr);
    uint8_t *identityKeysBytesPtr = (uint8_t *)malloc(identityKeysLength + 1);
    identityKeysBytesPtr[identityKeysLength] = '\0';

    if (!identityKeysBytesPtr) {
      detail::throwJSError(*runtime, "identityKeys(): identity keys array OOM");
    } else {
      size_t keysResult = olm_account_identity_keys(
          accountPtr, identityKeysBytesPtr, identityKeysLength);

      if (keysResult == olm_error()) {
        auto errorMessage = (const char *)olm_account_last_error(accountPtr);
        detail::throwJSError(*runtime, errorMessage);
      } else {
        return reinterpret_cast<char *>(identityKeysBytesPtr);
      }
    }
  }

  return NULL;
}

void generateOneTimeKeys(Runtime *runtime, OlmAccount *accountPtr,
                         uint8_t aNumberOfKeys) {
  const char *errorMessage = NULL;

  if (!accountPtr) {
    errorMessage = "generateOneTimeKeysJni(): failure - null Account ptr";
  } else {
    size_t randomLength = olm_account_generate_one_time_keys_random_length(
        accountPtr, (size_t)aNumberOfKeys);
    uint8_t *randomBufferPtr =
        (uint8_t *)malloc(randomLength * sizeof(uint8_t));

    size_t result = olm_account_generate_one_time_keys(
        accountPtr, (size_t)aNumberOfKeys, (void *)randomBufferPtr,
        randomLength);

    if (result == olm_error()) {
      errorMessage = olm_account_last_error(accountPtr);
    }

    if (errorMessage) {
      detail::throwJSError(*runtime, errorMessage);
    }

    if (randomBufferPtr) {
      memset(randomBufferPtr, 0, randomLength);
      free(randomBufferPtr);
    }
  }
}

string oneTimeKeys(Runtime *runtime, OlmAccount *accountPtr) {
  if (!accountPtr) {
    detail::throwJSError(*runtime,
                         "oneTimeKeysJni(): failure - null Account ptr");
  } else {
    size_t keysLength = olm_account_one_time_keys_length(accountPtr);
    uint8_t *keysBytesPtr = (uint8_t *)malloc(keysLength * sizeof(uint8_t));

    if (!keysBytesPtr) {
      detail::throwJSError(
          *runtime, "oneTimeKeysJni(): failure - one time keys array OOM");
    } else {
      size_t keysResult =
          olm_account_one_time_keys(accountPtr, keysBytesPtr, keysLength);

      if (keysResult == olm_error()) {
        auto errorMessage = (const char *)olm_account_last_error(accountPtr);
        detail::throwJSError(*runtime, errorMessage);
        return nullptr;
      } else {
        return reinterpret_cast<char *>(keysBytesPtr);
      }
    }
  }

  return NULL;
}

void markOneTimeKeysAsPublished(Runtime *runtime, OlmAccount *accountPtr) {
  const char *errorMessage = NULL;

  if (!accountPtr) {
    errorMessage = "markOneTimeKeysAsPublishedJni(): failure - null account pt";
  } else {
    size_t result = olm_account_mark_keys_as_published(accountPtr);

    if (result == olm_error()) {
      errorMessage = (const char *)olm_account_last_error(accountPtr);
    }
  }

  if (errorMessage) {
    detail::throwJSError(*runtime, errorMessage);
  }
}

void removeOneTimeKeys(Runtime *runtime, OlmAccount *accountPtr,
                       OlmSession *sessionPtr) {
  const char *errorMessage = NULL;
  if (!sessionPtr) {
    errorMessage = "removeOneTimeKeysJni(): failure - null session ptr";
  } else if (!accountPtr) {
    errorMessage = "removeOneTimeKeysJni(): failure - null account ptr";
  } else {
    size_t result = olm_remove_one_time_keys(accountPtr, sessionPtr);

    if (result == olm_error()) {
      errorMessage = olm_account_last_error(accountPtr);
    }
  }

  if (errorMessage) {
    detail::throwJSError(*runtime, errorMessage);
  }
}
} // namespace adapter
