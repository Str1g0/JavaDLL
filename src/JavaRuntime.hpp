#pragma once

#include <jni.h>
#include "Stdlib.hpp"

#include "JniFunction.hpp"

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
            bool CallJavaMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object, args_t... args)
            {
                jclass _class = m_environment->FindClass(javaClass.signature.c_str());

                if (!_class)
                {
                    SpitJniError();

                    std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.signature.c_str());
                    return false;
                }

                std::string methodArgs;
                methodArgs = MakeMethodArgs(methodArgs, args...);

                std::string signature = '(' + methodArgs + ')' + TypeToJniStr(result);

                jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

                if (!methodId)
                {
                    SpitJniError();
                    std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.signature.c_str(), methodName.c_str());
                    return false;
                }

                return true;
            }

            template<typename return_t>
            bool CallJavaMethod(return_t& result, JavaObject javaClass, std::string const& methodName, jobject object)
            {
                jclass _class = m_environment->FindClass(javaClass.signature.c_str());

                if (!_class)
                {
                    SpitJniError();

                    std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.signature.c_str());
                    return false;
                }

                std::string signature = "()" + TypeToJniStr(result);

                jmethodID methodId = m_environment->GetMethodID(_class, methodName.c_str(), signature.c_str());

                if (!methodId)
                {
                    SpitJniError();
                    std::fprintf(stderr, "Failed to find java method: %s.%s!\n", javaClass.signature.c_str(), methodName.c_str());
                    return false;
                }

                m_environment->CallVoidMethod(object, methodId);

                return true;
            }

            jobject GetObjectMethod()
            {
                jclass _class = m_environment->FindClass("jar/lib/TestBeacon");

                if (!_class)
                {
                    SpitJniError();

                    std::fprintf(stderr, "Failed to find java class: %s!\n", "jar/lib/TestBeacon");
                    return nullptr;
                }

                std::string signature = "(I)Ljar/lib/TestBeacon;";

                jmethodID methodId = m_environment->GetStaticMethodID(_class, "NewBeacon", signature.c_str());

                if (!methodId)
                {
                    SpitJniError();
                    std::fprintf(stderr, "Failed to find java method: BEACON!\n");
                    return nullptr;
                }

                jobject beacon = m_environment->CallStaticObjectMethod(_class, methodId, jint(10));
                m_environment->NewGlobalRef(beacon);

                if (!beacon)
                {
                    SpitJniError();
                    std::fprintf(stderr, "Failed to find java method: BEACON!\n");
                    return nullptr;
                }

                return beacon;
            }
    };
}