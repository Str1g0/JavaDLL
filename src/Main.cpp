#include "JavaRuntime.hpp"

int testmain(int argc, char** argv)
{
    using namespace std::string_literals;

    java::JavaRuntime runtime(JNI_VERSION_1_8, {"../jar/BattlefieldExplorer.jar", 
        "../jar/lib/swing-bits-1.0.0-SNAPSHOT.jar", "../jar/TestBeacon"  });

    java::ClassPath cls{ "battlefieldexplorer/BattlefieldExplorer" };
    java::ClassPath guiCls{ "battlefieldexplorer/gui/Gui" };
    java::ClassPath bfInfoClass{ "battlefieldexplorer/search/BattleFieldInfo" };
    java::ClassPath bfClass{ "battlefieldexplorer/generator/Battlefield" };
    java::ClassPath terrainClass{ "battlefieldexplorer/generator/Terrain" };

    runtime.CallMain(cls);

    jobject gui;
    runtime.CallJavaStaticObjectMethod(gui, guiCls, "getGuiInstance", guiCls);
    runtime.CallJavaVoidMethod(guiCls, "Test", gui);

    //jobject terrain = runtime.GetStaticObjectField(terrainClass, "SNOW");
    //java::JavaObject terrainObj{ terrainClass, terrain };

    //jobject bfInfo;
    //runtime.CallJavaStaticObjectMethod(bfInfo, bfInfoClass, "load", bfInfoClass);

    //jobject battlefield;

    jint x = 10, y = 20;
    jint terrainID = 4;
    runtime.CallJavaVoidMethod(guiCls, "onNewBattlefield", gui, x, y, terrainID);

    /*java::JavaObject battlefieldObj {
        bfClass,
        battlefield
    };

    runtime.CallJavaVoidMethod(guiCls, "setControlState", gui, battlefieldObj);*/
    
    std::string out;
    //jobject beacon = runtime.CreateNewObject(cls);
    //runtime.CallJavaStringMethod(out, cls, "StringTestFunc", beacon, 10l, 20.f);
    //runtime.CallJavaVoidMethod(obj, "PrintStuff", beacon);

    std::cout << out;

    //jint arg = 10;
    //auto beacon = runtime.GetObjectMethod();
    //runtime.CallJavaMethod(java::jvoid, obj, "PrintStuff"s, beacon);

    std::cin.get();

    return 0;
}  