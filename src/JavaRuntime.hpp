#pragma once

#include <jni.h>
#include "Stdlib.hpp"

#include "JniFunction.hpp"
#include "Macros.hpp"

namespace java
{
    struct JavaError:
        public std::runtime_error
    {
        JavaError(std::string const& what);
    };

    class JavaRuntime
    {
        static constexpr cstring OPT_CLASS_PATH = "-Djava.class.path=";
        
        struct JavaVMDtor
        {
            void operator()(JavaVM* vm)
            {
                vm->DestroyJavaVM();
            }
        };

        typedef std::vector<JavaVMOption> JavaOptions;

        jint m_javaVersion;

        JavaOptions     m_options   {0};
        JavaVMInitArgs  m_args      {0};
         
        JavaVM* m_virtualMachine{ nullptr };
        JNIEnv*  m_environment   { nullptr };

        std::string m_classOption;
        std::vector<std::string> m_jarsToLoad;

        void SetupArgs();

        public:

            JavaRuntime(jint version, std::vector<std::string> jarPaths);

            void SpitJniError();
            bool CreateJVM();

            template<typename return_t, typename ...args_t>
            bool CallJavaMethod(return_t& result, ClassPath javaClass, std::string const& methodName, jobject object, args_t... args);
            template<typename return_t>
            bool CallJavaMethod(return_t& result, ClassPath javaClass, std::string const& methodName, jobject object);

            template<typename ...args_t>
            bool CallJavaVoidMethod(ClassPath javaClass, std::string const& methodName, jobject object, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallClassPathMethod(return_t& result, ClassPath javaClass, std::string const& methodName, jobject object, args_t... args);

            template<typename ...args_t>
            bool CallJavaStringMethod(std::string& result, ClassPath javaClass, std::string const& methodName, jobject object,args_t... args);

            CALL_JAVA_METHOD(Int);
            CALL_JAVA_METHOD(Short);
            CALL_JAVA_METHOD(Long);
            CALL_JAVA_METHOD(Char);
            CALL_JAVA_METHOD(Byte);
            CALL_JAVA_METHOD(Float);
            CALL_JAVA_METHOD(Double);

            template<typename ...args_t>
            bool CallJavaStaticVoidMethod(ClassPath javaClass, std::string const& methodName, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallJavaStaticObjectMethod(return_t& result, ClassPath javaClass, std::string const& methodName, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallJavaStaticStringMethod(return_t& result, ClassPath javaClass, std::string const& methodName, args_t... args);

            CALL_STATIC_JAVA_METHOD(Int);
            CALL_STATIC_JAVA_METHOD(Short);
            CALL_STATIC_JAVA_METHOD(Long);
            CALL_STATIC_JAVA_METHOD(Char);
            CALL_STATIC_JAVA_METHOD(Byte);
            CALL_STATIC_JAVA_METHOD(Float);
            CALL_STATIC_JAVA_METHOD(Double);

            jobject CreateNewObject(ClassPath obj);
    };

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaMethod(return_t& result, ClassPath javaClass, std::string const& methodName, jobject object, args_t ...args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);
        std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        return true;
    }

    template<typename return_t>
    inline bool JavaRuntime::CallJavaMethod(return_t& result, ClassPath javaClass, std::string const& methodName, jobject object)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string signature =  + TypeToJniStr(result);
        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        m_environment->CallVoidMethod(object, methodId);

        return true;
    }
    
    template<typename ...args_t>
    inline bool JavaRuntime::CallJavaVoidMethod(ClassPath javaClass, std::string const& methodName, jobject object, args_t... args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(args...);
        std::string signature = '(' + methodArgs + ')' + 'V';

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        m_environment->CallVoidMethod(object, methodId);
        SpitJniError();

        return true;
    }

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallClassPathMethod(return_t& result, ClassPath javaClass, std::string const& methodName, jobject object, args_t ...args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(args...);
        std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        result = m_environment->CallObjectMethod(object, methodId, args...);
        SpitJniError();

        return true;
    }

    template<typename ...args_t>
    inline bool JavaRuntime::CallJavaStringMethod(std::string& result, ClassPath javaClass, std::string const& methodName, jobject object, args_t... args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(args...);
        std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);
        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        jstring javaString = reinterpret_cast<jstring>(m_environment->CallObjectMethod(object, methodId, args...));
        SpitJniError();

        jboolean isCopy;
        cstring jstringContent = m_environment->GetStringUTFChars(javaString, &isCopy);
        SpitJniError();

        result = jstringContent;
        if (!isCopy)
        {
            m_environment->ReleaseStringUTFChars(javaString, jstringContent);
            SpitJniError();
        }

        return true;
    }

    template<typename ...args_t>
    inline bool JavaRuntime::CallJavaStaticVoidMethod(ClassPath javaClass, std::string const& methodName, args_t... args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);
        std::string signature = '(' + methodArgs + ')' + 'V';

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        m_environment->CallStaticVoidMethod(_class, methodId);
        SpitJniError();

        return true;
    }

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaStaticObjectMethod(return_t& result, ClassPath javaClass, std::string const& methodName, args_t ...args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);
        std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }
          
        result = m_environment->CallJavaStaticObjectMethod(_class, methodId);
        SpitJniError();

        return true;
    }

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaStaticStringMethod(return_t& result, ClassPath javaClass, std::string const& methodName, args_t ...args)
    {
        jclass _class = m_environment->FindClass(javaClass.asString.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);
        std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.asString.c_str(), methodName.c_str());
            return false;
        }

        jstring javaString = m_environment->CallObjectMethod(_class, methodId, args...);
        SpitJniError();

        jboolean isCopy;
        cstring jstringContent = m_environment->GetStringChars(javaString, &isCopy);
        SpitJniError();

        result = jstringContent;

        if (!isCopy)
        {
            m_environment->ReleaseStringChars(javaString, jstringContent);
            SpitJniError();
        }

        return true;
    }
}