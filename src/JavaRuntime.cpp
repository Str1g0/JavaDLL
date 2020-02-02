#include "JavaRuntime.hpp"


void java::JavaRuntime::SetupArgs()
{
    m_classOption = OPT_CLASS_PATH;
    for (auto jar = m_jarsToLoad.begin();
        jar != m_jarsToLoad.end(); ++jar)
    {
        m_classOption += *jar;

        if(jar + 1 != m_jarsToLoad.end())
            m_classOption += ';';
    }

    JavaVMOption classPath;
    JavaVMOption memMin;
    JavaVMOption memMax;
    JavaVMOption verbose;

    classPath.optionString = const_cast<char*>(m_classOption.c_str());

    memMin.optionString     = (char*)"-Xms512m";  //512mb
    memMax.optionString     = (char*)"-Xms1g";  //1gb
    verbose.optionString    = (char*)"-verbose:jni";

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
    if (m_environment && (m_environment)->ExceptionOccurred()) 
    {
        m_environment->ExceptionDescribe();
        m_environment->ExceptionClear();
        //throw JavaError("A java exception has occured!");
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

jstring java::JavaRuntime::MakeJavaString(std::string const& in)
{
    if (m_environment)
        return m_environment->NewStringUTF(in.c_str());

    return nullptr;
}

bool java::JavaRuntime::CallMain(ClassPath mainClass)
{
    jclass _class = m_environment->FindClass(mainClass.asString.c_str());

    if (!_class)
    {
        SpitJniError();

        std::fprintf(stderr, "Failed to find java class: %s!\n", mainClass.asString.c_str());
        return false;
    }

    jmethodID methodId = m_environment->GetStaticMethodID(_class, "main", "([Ljava/lang/String;)V");
    if (!methodId)
    {
        SpitJniError();

        std::fprintf(stderr, "Failed to find java method: %s.%s!\n", mainClass.asString.c_str(), "main");
        return false;
    }

    jobjectArray args = (jobjectArray)m_environment->NewObjectArray(0,
        m_environment->FindClass("java/lang/String"),
        m_environment->NewStringUTF(""));

    m_environment->CallStaticVoidMethod(_class, methodId, args);
    SpitJniError();

    return true;
}

bool java::JavaRuntime::CheckReference(jobject ref)
{
    return m_environment->GetObjectRefType(ref) != JNIInvalidRefType;
}

jobject java::JavaRuntime::GetStaticObjectField(ClassPath javaClass, std::string const& fieldName)
{
    jclass _class = m_environment->FindClass(javaClass.asString.c_str());

    if (!_class)
    {
        SpitJniError();
        std::fprintf(stderr, "Failed to find java class: %s!\n", javaClass.asString.c_str());
        return false;
    }

    jfieldID id = m_environment->GetStaticFieldID(_class, fieldName.c_str(), TypeToJniStr(javaClass).c_str());
    if (!id)
    {
        SpitJniError();
        std::fprintf(stderr, "Failed to find static field: %s.%s!\n", javaClass.asString.c_str(), fieldName.c_str());
        return nullptr;
    }

    jobject localRef = m_environment->GetStaticObjectField(_class, id);
    if (!localRef)
    {
        SpitJniError();
        std::fprintf(stderr, "Failed to find static field: %s.%s!\n", javaClass.asString.c_str(), fieldName.c_str());
        return nullptr;
    }

    jobject result = m_environment->NewGlobalRef(localRef);
    SpitJniError();

    m_environment->DeleteLocalRef(localRef);
    SpitJniError();

    return result;
}


java::JavaRuntime::JavaRuntime(jint version, std::vector<std::string> jarPaths):
    m_javaVersion(version),
    m_jarsToLoad(jarPaths)
{
    CreateJVM();
}

java::JavaError::JavaError(std::string const& what):
    std::runtime_error(what)
{
}
