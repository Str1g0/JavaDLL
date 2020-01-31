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

    memMin.optionString     = (char*)"-Xms1m";  //1mb
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
        (m_environment)->ExceptionDescribe();
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

jobject java::JavaRuntime::CreateNewObject(ClassPath obj)
{
    jclass _class = m_environment->FindClass(obj.asString.c_str());

    if (!_class)
    {
        SpitJniError();

        std::fprintf(stderr, "Failed to find java class: %s!\n", obj.asString.c_str());
        return nullptr;
    }

    jobject newObject = m_environment->AllocObject(_class);
    SpitJniError();

    return newObject;
}
