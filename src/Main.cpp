#include "JavaRuntime.hpp"
#include <Windows.h>

int main(int argc, char** argv)
{
    using namespace std::string_literals;

    java::JavaRuntime runtime(JNI_VERSION_1_8, {"../jar/beacon.jar"});

    java::ClassPath cls{ "jar/lib/TestBeacon" };

    std::string out;
    jobject beacon = runtime.CreateNewObject(cls);
    //runtime.CallJavaStringMethod(out, cls, "StringTestFunc", beacon, 10l, 20.f);
    //runtime.CallJavaVoidMethod(obj, "PrintStuff", beacon);

    std::cout << out;

    //jint arg = 10;
    //auto beacon = runtime.GetObjectMethod();
    //runtime.CallJavaMethod(java::jvoid, obj, "PrintStuff"s, beacon);

     std::cin.get();
}  