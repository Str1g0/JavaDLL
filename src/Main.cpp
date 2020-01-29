#include "JavaRuntime.hpp"
#include <Windows.h>

int main(int argc, char** argv)
{
    using namespace std::string_literals;

    java::JavaRuntime runtime(JNI_VERSION_1_8, {"D:/Workspace/Projects/JavaDLL/jar/beacon.jar"});

    java::JavaObject obj{ "jar/lib/TestBeacon" };

    jint arg = 10;
    auto beacon = runtime.GetObjectMethod();
    runtime.CallJavaMethod(java::jvoid, obj, "PrintStuff"s, beacon);

    std::cin.get();
}