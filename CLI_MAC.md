# Macでmakeを使って開発

## 第2章のコンパイルで以下のようなエラーが大量発生

```bash
In file included from BGSpriteComponent.cpp:9:
In file included from ./BGSpriteComponent.h:12:
In file included from ./math.h:11:
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/cmath:318:9: error:
      no member named 'signbit' in the global namespace
using ::signbit;
      ~~^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/cmath:319:9: error:
      no member named 'fpclassify' in the global namespace
using ::fpclassify;
...
```

現象は[Stack Overflowのこの問題](https://stackoverflow.com/questions/58628377/catalina-c-using-cmath-headers-yield-error-no-member-named-signbit-in-th)
と同じであり、紹介されている方法のうち`Gralex`氏の方法で解決した。
