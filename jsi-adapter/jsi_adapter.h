#ifndef ANDROID_JSIADAPTER_H
#define ANDROID_JSIADAPTER_H

namespace facebook {
namespace jsi {
class Runtime;
}
} // namespace facebook

namespace jsiadapter {
void install(facebook::jsi::Runtime &jsiRuntime);
}

#endif // ANDROID_JSIADAPTER_H
