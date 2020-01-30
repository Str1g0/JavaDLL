#include "JavaRuntime.hpp"


void java::JavaRuntime::SetupArgs()
{
    m_classOption = OPT_CLASS_PATH;
    for (auto jar = m_jarsToLoad.begin();
        jar != m_jarsToLoad.end(); ++jar)
    {
        m_classOption += *jar;

        if(jar + 1 != m_jarsToLoad.end())
            m_classOption += ':';
    }

    JavaVMOption classPath;
    JavaVMOption memMin;
    JavaVMOption memMax;
    JavaVMOption verbose;

    classPath.optionString = const_cast<char*>(m_classOption.c_str());

    memMin.optionString = (char*)"-Xms1m";  //1mb
    memMax.optionString = (char*)"-Xms1g";  //1gb
    verbose.optionString = (char*)"-verbose:jni";

    m_options.push_back(classPath);
    m_options.push_back(memMin);
    m_options.push_back(memMax);

    m_args.version  = m_javaVersion;
    m_args.nOptions = m_options.size();
    m_args.options  = m_options.data();

    m_args.ignoreUnrecognized = JNI_FALSE;
}

void java::JavaRuntime::SpitJniError()
{
    if (m_environment && (m_environment)->ExceptionOccurred()) {
        (m_environment)->ExceptionDescribe();
    }
}

bool java::JavaRuntime::CreateJVM()
{
    SetupArgs();

    jint result = JNI_CreateJavaVM(&m_virtualMachine, reinterpret_cast<void**>(&m_environment), (void*)&m_args);
    if (result != JNI_OK)
    {
        SpitJniError();

        std::fprintf(stdout, "Failed to create the java virtual machine: %i!", result);
        return false;
    }

    return true;
}

jobject java::JavaRuntime::GetObjectMethod()
{
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
}

java::JavaRuntime::JavaRuntime(jint version, std::vector<std::string> jarPaths):
    m_javaVersion(version),
    m_jarsToLoad(jarPaths)
{
    CreateJVM();
}
