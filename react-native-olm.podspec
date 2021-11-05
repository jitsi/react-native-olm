require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|

  MAJOR = 0
  MINOR = 0
  PATCH = 1

  s.name         = "react-native-olm"
  s.version      = "#{MAJOR}.#{MINOR}.#{PATCH}"
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "10.0" }
  s.source       = { :git => "https://github.com/jitsi/react-native-olm.git", :tag => "#{s.version}" }

  s.ios.deployment_target = "6.0"
  s.osx.deployment_target = "10.9"

  s.source_files =  
  "include/**/*.{h,hh}", 
  "src/*.{c,cpp}", 
  "lib/crypto-algorithms/sha256.c",  
  "lib/crypto-algorithms/aes.c", 
  "lib/curve25519-donna/curve25519-donna.c", 
  "ios/**/*.{h,m,mm}", 
  "cpp/**/*.{h,cpp,c}",
  "adapter/**/*.{h,cpp}",
  "jsi_host_objects/**/*.{h,cpp}",
  "jsi-adapter/**/*.{h,cpp}"
  
  s.dependency "React-Core"

  # Those files (including .c) are included by ed25519.c. We do not want to compile them twice
  s.preserve_paths = "lib/ed25519/**/*.{h,c}"

  s.library = "c++"


  # Use the same compiler options for C and C++ as olm/Makefile

  s.compiler_flags = "-g -O3 -DOLMLIB_VERSION_MAJOR=#{MAJOR} -DOLMLIB_VERSION_MINOR=#{MINOR} -DOLMLIB_VERSION_PATCH=#{PATCH}"

  # For headers search paths, manage first the normal installation. Then, use paths used
  # when the pod is local
  s.xcconfig = {
    'USER_HEADER_SEARCH_PATHS' =>"${PODS_ROOT}/OLMKit/include ${PODS_ROOT}/OLMKit/lib #{File.join(File.dirname(__FILE__), 'include')} #{File.join(File.dirname(__FILE__), 'lib')}"
  }
end