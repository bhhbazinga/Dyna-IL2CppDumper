#include "define.h"

const char* kOutputDir = "D:\\proj\\Dyna-IL2CppDumper\\";
const char* kLogFile = "D:\\proj\\Dyna-IL2CppDumper\\log.txt";

#ifdef WHITE_LIST
const std::unordered_set<std::string> kDumpAssemblies{
    "UnityEngine.dll",
    "UnityEngine.IMGUIModule.dll",
    "UnityEngine.UIElementsModule.dll",
    "UnityEngine.UIModule.dll",
    "UnityEngine.UI.dll",
    "Cute.UI.Assembly.dll",
    "umamusume.dll",
};

// Example: All assemblies in umamusume
// Image 0: mscorlib.dll - 0
// Image 1: System.Configuration.dll - 1522
// Image 2: Mono.Security.dll - 1532
// Image 3: System.Xml.dll - 1537
// Image 4: System.dll - 1540
// Image 5: System.Core.dll - 1818
// Image 6: UnityEngine.SharedInternalsModule.dll - 1867
// Image 7: UnityEngine.CoreModule.dll - 1897
// Image 8: UnityEngine.PhysicsModule.dll - 2496
// Image 9: UnityEngine.SubsystemsModule.dll - 2519
// Image 10: UnityEngine.TextRenderingModule.dll - 2531
// Image 11: UnityEngine.AssetBundleModule.dll - 2546
// Image 12: UnityEngine.AudioModule.dll - 2551
// Image 13: UnityEngine.GridModule.dll - 2566
// Image 14: UnityEngine.IMGUIModule.dll - 2568
// Image 15: UnityEngine.ImageConversionModule.dll - 2608
// Image 16: UnityEngine.InputLegacyModule.dll - 2610
// Image 17: UnityEngine.Physics2DModule.dll - 2619
// Image 18: UnityEngine.TextCoreModule.dll - 2637
// Image 19: UnityEngine.UnityWebRequestModule.dll - 2652
// Image 20: UnityEngine.XRModule.dll - 2665
// Image 21: UnityEngine.AIModule.dll - 2692
// Image 22: UnityEngine.AndroidJNIModule.dll - 2695
// Image 23: UnityEngine.AnimationModule.dll - 2709
// Image 24: UnityEngine.DirectorModule.dll - 2752
// Image 25: UnityEngine.GameCenterModule.dll - 2754
// Image 26: UnityEngine.InputModule.dll - 2774
// Image 27: UnityEngine.JSONSerializeModule.dll - 2779
// Image 28: UnityEngine.ParticleSystemModule.dll - 2781
// Image 29: UnityEngine.ScreenCaptureModule.dll - 2797
// Image 30: UnityEngine.SpriteMaskModule.dll - 2800
// Image 31: UnityEngine.TerrainModule.dll - 2802
// Image 32: UnityEngine.TilemapModule.dll - 2818
// Image 33: UnityEngine.UIElementsModule.dll - 2828
// Image 34: UnityEngine.UIModule.dll - 2983
// Image 35: UnityEngine.UnityAnalyticsModule.dll - 2994
// Image 36: UnityEngine.UnityWebRequestWWWModule.dll - 3005
// Image 37: UnityEngine.VFXModule.dll - 3007
// Image 38: UnityEngine.VRModule.dll - 3018
// Image 39: UnityEngine.VideoModule.dll - 3022
// Image 40: UnityEngine.dll - 3037
// Image 41: DOTween.dll - 3038
// Image 42: System.Runtime.dll - 3217
// Image 43: Unity.Compat.dll - 3218
// Image 44: Unity.Tasks.dll - 3219
// Image 45: UnityEngine.UI.dll - 3220
// Image 46: Cute.Core.Assembly.dll - 3416
// Image 47: DOTweenPro.dll - 3482
// Image 48: Facebook.Unity.Settings.dll - 3498
// Image 49: Firebase.Platform.dll - 3503
// Image 50: LibNative.Runtime.dll - 3540
// Image 51: Unity.Burst.Unsafe.dll - 3577
// Image 52: Unity.TextMeshPro.dll - 3580
// Image 53: Unity.Timeline.dll - 3740
// Image 54: Boo.Lang.dll - 3812
// Image 55: Cinemachine.dll - 3829
// Image 56: Cute.Cri.Assembly.dll - 3995
// Image 57: Cute.Http.Assembly.dll - 4358
// Image 58: Cute.UI.Assembly.dll - 4391
// Image 59: Facebook.Unity.dll - 4485
// Image 60: Firebase.App.dll - 4601
// Image 61: MessagePack.dll - 4623
// Image 62: Plugins.dll - 4896
// Image 63: SignInWithApple.dll - 5611
// Image 64: Unity.Burst.dll - 5624
// Image 65: XNode.dll - 5636
// Image 66: _Cyan.dll - 5663
// Image 67: cute_payment.dll - 5774
// Image 68: Cute.AssetBundle.Assembly.dll - 5795
// Image 69: DebugHook.dll - 5804
// Image 70: Firebase.Crashlytics.dll - 5806
// Image 71: GooglePlayGames.dll - 5832
// Image 72: umamusume.dll - 5877
#endif