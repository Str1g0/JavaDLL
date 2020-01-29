#include "JniFunction.hpp"

namespace java
{
    template<>
    std::string TypeToJniStr<jvoid_t>(jvoid_t value) {
        return "V";
    }

    template<>
    std::string TypeToJniStr<jint>(jint value) {
        return "I";
    }

    template<>
    std::string TypeToJniStr<jlong>(jlong value) {
        return "J";
    }

    template<>
    std::string TypeToJniStr<jshort>(jshort value) {
        return "S";
    }

    template<>
    std::string TypeToJniStr<jchar>(jchar value) {
        return "C";
    }

    template<>
    std::string TypeToJniStr<jbyte>(jbyte value) {
        return "B";
    }

    template<>
    std::string TypeToJniStr<jfloat>(jfloat value) {
        return "F";
    }

    template<>
    std::string TypeToJniStr<jdouble>(jdouble value) {
        return "D";
    }

    template<>
    std::string TypeToJniStr<JavaObject>(JavaObject value) {
        return 'L' + value.signature + ';';
    }

    template<>
    std::string TypeToJniStr<std::string>(std::string value) {
        return "L/java/lang/String;";
    }

    template<>
    std::string TypeToJniStr<cstring>(cstring value) {
        return "L/java/lang/String;";
    }
}