#pragma once


#define CALL_JAVA_METHOD(returnType)                                                                                                                    \
template<typename return_t, typename ...args_t>                                                                                                         \
inline bool CallJava ## returnType ## Method (return_t& result, JavaObject javaClass, std::string const& methodName, jobject object, args_t ...args)    \
{                                                                                                                                                       \
    jclass _class = m_environment->FindClass(javaClass.classpath.c_str());                                                                              \
                                                                                                                                                        \
    if (!_class)                                                                                                                                        \
    {                                                                                                                                                   \
        SpitJniError();                                                                                                                                 \
                                                                                                                                                        \
        std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());                                                          \
        return false;                                                                                                                                   \
    }                                                                                                                                                   \
                                                                                                                                                        \
    std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);                                                                              \
    std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);                                                                              \
                                                                                                                                                        \
    jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());                                                     \
                                                                                                                                                        \
    if (!methodId)                                                                                                                                      \
    {                                                                                                                                                   \
        SpitJniError();                                                                                                                                 \
        std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());                                  \
        return false;                                                                                                                                   \
    }                                                                                                                                                   \
                                                                                                                                                        \
                                                                                                                                                        \
    result = m_environment->Call ## returnType ## Method(object, methodId, args...);                                                                    \
    SpitJniError();                                                                                                                                     \
    return true;                                                                                                                                        \
}


#define CALL_STATIC_JAVA_METHOD(returnType)                                                                                                             \
template<typename return_t, typename ...args_t>                                                                                                         \
inline bool CallJavaStatic ## returnType ## Method   (return_t& result, JavaObject javaClass, std::string const& methodName, args_t ...args)            \
{                                                                                                                                                       \
    jclass _class = m_environment->FindClass(javaClass.classpath.c_str());                                                                              \
                                                                                                                                                        \
    if (!_class)                                                                                                                                        \
    {                                                                                                                                                   \
        SpitJniError();                                                                                                                                 \
                                                                                                                                                        \
        std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());                                                          \
        return false;                                                                                                                                   \
    }                                                                                                                                                   \
                                                                                                                                                        \
    std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);                                                                              \
    std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);                                                                              \
                                                                                                                                                        \
    jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());                                                     \
                                                                                                                                                        \
    if (!methodId)                                                                                                                                      \
    {                                                                                                                                                   \
        SpitJniError();                                                                                                                                 \
        std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());                                  \
        return false;                                                                                                                                   \
    }                                                                                                                                                   \
                                                                                                                                                        \
                                                                                                                                                        \
    result = m_environment->CallStatic ## returnType ## Method(_class, methodId, args...);                                                              \
    SpitJniError();                                                                                                                                     \
    return true;                                                                                                                                        \
}
