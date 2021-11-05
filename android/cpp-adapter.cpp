#include "jsi_adapter.h"
#include "pthread.h"
#include <jni.h>
#include <jsi/jsi.h>
#include <sys/types.h>

using namespace facebook::jsi;
using namespace std;

JavaVM *java_vm;
jobject java_object;

/**
 * A simple callback function that allows us to detach current JNI Environment
 */
void DeferThreadDetach(JNIEnv *env) {
  static pthread_key_t thread_key;

  // Set up a Thread Specific Data key, and a callback that
  // will be executed when a thread is destroyed.
  // This is only done once, across all threads, and the value
  // associated with the key for any given thread will initially
  // be NULL.
  static auto run_once = [] {
    const auto err = pthread_key_create(&thread_key, [](void *ts_env) {
      if (ts_env) {
        java_vm->DetachCurrentThread();
      }
    });
    if (err) {
      // Failed to create TSD key. Throw an exception if you want to.
    }
    return 0;
  }();

  // For the callback to actually be executed when a thread exits
  // we need to associate a non-NULL value with the key on that thread.
  // We can use the JNIEnv* as that value.
  const auto ts_env = pthread_getspecific(thread_key);
  if (!ts_env) {
    if (pthread_setspecific(thread_key, env)) {
      // Failed to set thread-specific value for key. Throw an exception if you
      // want to.
    }
  }
}

extern "C" JNIEXPORT void JNICALL
Java_com_reactnativeolm_OlmModule_nativeInstall(JNIEnv *env,
                                                              jobject thiz,
                                                              jlong jsi) {

  auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsi);

  if (runtime) {
    jsiadapter::install(*runtime);
  }

  env->GetJavaVM(&java_vm);
  java_object = env->NewGlobalRef(thiz);
}
