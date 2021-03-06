#include "JniFunction.hpp"

namespace java
{
    template<>
    jobject java::MakeJavaType(JavaObject param)
    {
        return param.object;
    }

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
    std::string TypeToJniStr<ClassPath>(ClassPath value) {
        return 'L' + value.asString + ';';
    }

    template<>
    std::string TypeToJniStr<jobject>(jobject value) {
        return "Ljava/lang/Object;";
    }

    template<>
    std::string TypeToJniStr(JavaObject value)
    {
        return TypeToJniStr(value.path);
    }

    template<>
    std::string TypeToJniStr<jstring>(jstring value) {
        return "Ljava/lang/String;";
    }

    template<>
    std::string TypeToJniStr<std::string>(std::string value) {
        return "Ljava/lang/String;";
    }

    template<>
    std::string TypeToJniStr<cstring>(cstring value) {
        return "Ljava/lang/String;";
    }

    std::string MakeMethodArgsJniString()
    {
        return "";
    }
}