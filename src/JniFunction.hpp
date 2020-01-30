#pragma once

#include "Stdlib.hpp"
#include <jni.h>

namespace java
{
    struct JavaObject  {
        std::string classpath;
    };

    static enum jvoid_t {} jvoid;

    template<typename T>
    std::string TypeToJniStr(T value);

    template<>
    std::string TypeToJniStr<jvoid_t>(jvoid_t value);

    template<>
    std::string TypeToJniStr<jint>(jint value);

    template<>
    std::string TypeToJniStr<jvoid_t>(jvoid_t value);

    template<>
    std::string TypeToJniStr<jint>(jint value);

    template<>
    std::string TypeToJniStr<jlong>(jlong value);

    template<>
    std::string TypeToJniStr<jshort>(jshort value);

    template<>
    std::string TypeToJniStr<jchar>(jchar value);

    template<>
    std::string TypeToJniStr<jbyte>(jbyte value);

    template<>
    std::string TypeToJniStr<jfloat>(jfloat value);

    template<>
    std::string TypeToJniStr<jdouble>(jdouble value);

    template<>
    std::string TypeToJniStr<JavaObject>(JavaObject value);

    template<>
    std::string TypeToJniStr<jstring>(jstring value);

    template<>
    std::string TypeToJniStr<std::string>(std::string value);

    template<>
    std::string TypeToJniStr<cstring>(cstring value);

    template<typename T>
    std::string TypeToJniStr(std::vector<T> const& value);

    template<typename T, typename ...args_t>
    std::string MakeMethodArgsJniString(T current, args_t... args)
    {
        std::string methodArgs{""};
        return MakeMethodArgs(methodArgs, current, args...);
    }

    template<typename T, typename ...args_t>
    std::string MakeMethodArgs(std::string& toAdd, T current, args_t... args)
    {
        toAdd += TypeToJniStr(current);
        return MakeMethodArgs(args...);
    }

    template<typename T, typename ...args_t>
    std::string MakeMethodArgs(std::string& toAdd, T last)
    {
        toAdd += TypeToJniStr(last);
        return toAdd;
    }

    template<typename T>
    std::string TypeToJniStr(std::vector<T> const& value)
    {
        static_assert("Unsupported value");
        return "";
    }

    template<typename T>
    std::string TypeToJniStr(T value)
    {
        static_assert("Unsupported value");
        return "";
    }
}