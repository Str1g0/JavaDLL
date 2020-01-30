#pragma once

#include <jni.h>
#include "Stdlib.hpp"

#include "JniFunction.hpp"
#include "Macros.hpp"

namespace java
{
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
        JNIEnv* m_environment   { nullptr };

        std::string m_classOption;
        std::vector<std::string> m_jarsToLoad;

        void SetupArgs();

        public:

            JavaRuntime(jint version, std::vector<std::string> jarPaths);

            void SpitJniError();
            bool CreateJVM();

            template<typename return_t, typename ...args_t>
            bool CallJavaMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object, args_t... args);
            template<typename return_t>
            bool CallJavaMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object);

            template<typename return_t, typename ...args_t>
            bool CallJavaVoidMethod(JavaObject javaClass, std::string const& methodName, jobject object, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallJavaObjectMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallJavaStringMethod(std::string& result, JavaObject javaClass, std::string const& methodName, jobject object,args_t... args);

            CALL_JAVA_METHOD(Int);
            CALL_JAVA_METHOD(Short);
            CALL_JAVA_METHOD(Long);
            CALL_JAVA_METHOD(Char);
            CALL_JAVA_METHOD(Byte);
            CALL_JAVA_METHOD(Float);
            CALL_JAVA_METHOD(Double);

            template<typename return_t, typename ...args_t>
            bool CallJavaStaticVoidMethod(JavaObject javaClass, std::string const& methodName, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallJavaStaticObjectMethod(return_t& result, JavaObject javaClass, std::string const& methodName, args_t... args);

            template<typename return_t, typename ...args_t>
            bool CallJavaStaticStringMethod(return_t& result, JavaObject javaClass, std::string const& methodName, args_t... args);

            CALL_STATIC_JAVA_METHOD(Int);
            CALL_STATIC_JAVA_METHOD(Short);
            CALL_STATIC_JAVA_METHOD(Long);
            CALL_STATIC_JAVA_METHOD(Char);
            CALL_STATIC_JAVA_METHOD(Byte);
            CALL_STATIC_JAVA_METHOD(Float);
            CALL_STATIC_JAVA_METHOD(Double);

            jobject GetObjectMethod();
           
    };

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object, args_t ...args)
    {
        jclass _class = m_environment->FindClass(javaClass.classpath.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());
            return false;
        }

        std::string methodArgs = MakeMethodArgsJniString(methodArgs, args...);
        std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);

        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());
            return false;
        }

        return true;
    }

    template<typename return_t>
    inline bool JavaRuntime::CallJavaMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object)
    {
        jclass _class = m_environment->FindClass(javaClass.classpath.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());
            return false;
        }

        std::string signature = "()" + TypeToJniStr(result);
        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());
            return false;
        }

        m_environment->CallVoidMethod(object, methodId);

        return true;
    }
    
    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaVoidMethod(JavaObject javaClass, std::string const& methodName, jobject object, args_t... args)
    {
        jclass _class = m_environment->FindClass(javaClass.classpath.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());
            return false;
        }

        std::string signature = "()" + TypeToJniStr(result);
        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());
            return false;
        }

        m_environment->CallVoidMethod(object, methodId);
        SpitJniError();

        return true;
    }

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaStringMethod(std::string& result, JavaObject javaClass, std::string const& methodName, jobject object, args_t... args)
    {
        jclass _class = m_environment->FindClass(javaClass.classpath.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());
            return false;
        }

        std::string signature = "()" + TypeToJniStr(result);
        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());
            return false;
        }

        jstring javaString = m_environment->CallObjectMethod(object, methodId, args...);
        SpitJniError();

        m_environment;

        return true;
    }

    template<typename return_t, typename ...args_t>
    inline bool JavaRuntime::CallJavaStaticVoidMethod(JavaObject javaClass, std::string const& methodName, args_t... args)
    {
        jclass _class = m_environment->FindClass(javaClass.classpath.c_str());

        if (!_class)
        {
            SpitJniError();

            std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.classpath.c_str());
            return false;
        }

        std::string signature = "()" + TypeToJniStr(result);
        jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

        if (!methodId)
        {
            SpitJniError();
            std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.classpath.c_str(), methodName.c_str());
            return false;
        }

        m_environment->CallStaticVoidMethod(_class, methodId);
        SpitJniError();

        return true;
    }
}