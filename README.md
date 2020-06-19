# 「ゲームプログラミングC++」

このリポジトリは、Sanjay Madhav著「ゲームプログラミングC++」のソースコードです。

各章のソースコードは三条項BSDライセンスで公開されています。詳細は`LICENSE`ファイルを参照してください。
このライセンスは`External`ディレクトリのコードには適用されないことに注意してください。`External`
ディレクトリの各プロジェクトはそれぞれにライセンスされています。

# コードのビルド

各章のコードはMicrosoft WindowsとApple macOSでテストされており、両OSで稼働します。

Windows上でコンパイルするには、 Microsoft Visual Studio 2017 Community版(https://www.visualstudio.com/downloads/)をインストールしてください。インストールの過程でワークフロー
"Game Development in C++" を選択してください。各章用のディレクトリに対応する`ChapterXX-windows.sln`がありますので、それを開いてください。

macOSでコンパイルするには、App StoreからXcodeをインストールしてください。各章には対応する`ChapterXX-mac.xcodeproj`ファイルがあります。

7章以降のコードではオーディオ用にFMOD APIを使用します。これは(https://www.fmod.com/download)から
インストールする必要があります。`FMOD Studio API`の`1.09.x`をダウンロード・インストールして
ください（新しい版はテストされていません）。WindowsではFMODをデフォルトディレクトリにインストール
してください。MacではFMODパッケージの内容を`External/FMOD`にコピーしてください。

-------------------------------------

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

## 課題2.2

次のように解釈して実装。

1. 複数のアニメーションの画像を`GetAnimTextures()`で取り込み、使用するアニメーションを`SetAnimation()`で取り出す。
2. ループしないアニメーションは最期のフレームを継続表示とする。

## 課題2.3

`Tiles.png`の画像は`256x768`ピクセルで、1つのタイルが`32x32`ピクセル、一行8タイル、24行であった。
また、レイアウトは一行32タイル、24行であるので、画面サイズは`1024x768`ピクセル。

### 完成した画面

![完成した画面](Exercises/2.3/Assets/screen.png)

## 課題3.2

消去の実装は、衝突時に宇宙船の状態を`EPaused`とし、位置をスクリーン外とした。オリジナルの`MovementComponent`は
ラッピング処理をしてスクリーン外の位置もスクリーン内に変換されて表示されてしまうので、ラッピング処理の有無をフラグとして
持つことにし、宇宙船はラッピング処理をしないようにした。

1. 宇宙船と小惑星の衝突判定を追加し、衝突したら、宇宙船を位置は画面中心、回転角ゼロでリセット
2. 衝突時、宇宙船を1秒消去し、その後、上記の条件で現れる

### `libpng warinig: iCCP: known incorrect sRGB profile`対策

表題でgoogleしてヒットした[ここ](https://www.henjins-toolbox.tech/entry/opencv_error1)の記載通り
以下のコマンドでpngファイルを処理したところ警告がなくなった。

```
convert Ship.png -strip Ship.png
```

## 課題3.3

課題が曖昧で、何が正解なのかわからなかった。調べたところ一人だけ解答を
[公開されていた方](https://github.com/TimFenwick15/game-programming)がいたので参考にさせてもらった。

- 力と初速度、質量をうまく設定しないとそれらしく動かない。
- レーザーを発射元の宇宙船の位置と回転角に合わせるには、作成時にこれらの値を設定するひつようがあった。そのため、`Actor`の定義を変更した。
- レーザーの一撃力はよほど大きな値を設定しないとすぐに減衰していまう。結局、初速の方が重要である。

## 4章のステートマシン部分

完成したゲームがセグメンテーションフォールトで動かない。問題は`AIComponent::ChangeState()`の以下の部分だった。

```c++
void AIComponent::ChangeState(const std::string& name)
{
    // 現在の状態を抜け出す
    if (mCurrentState)
    {
        mCurrentState->OnExit();
    }
```

コンパイラオプションのためと思われるが、何も設定されていない`mCurrentState`が
この`if`文で`true`となり、実際は`nullptr`(?)であるので`OnExit`がないためエラーになって
いるようであった。`AIComponent`の作成時に明示的に`nullptr`を設定し、`if`でチェックするように
した。

```c++
AIComponent::AIComponent(class Actor* owner)
:Component(owner)
,mCurrentState(nullptr)
{ }

void AIComponent::ChangeState(const std::string& name)
{
    // 現在の状態を抜け出す
    if (mCurrentState != nullptr)
    {
        mCurrentState->OnExit();
    }
```

### ゲーム結果

```
2020-06-19 15:10:57.331 a.out[23290:118809] INFO: AIPatrol: entered.
2020-06-19 15:10:57.714 a.out[23290:118809] INFO: AIPatrol: exited.
2020-06-19 15:10:57.714 a.out[23290:118809] INFO: AIAttack: entered.
2020-06-19 15:10:57.730 a.out[23290:118809] INFO: AIAttack: nothig occured.
2020-06-19 15:10:57.746 a.out[23290:118809] INFO: AIAttack: exited.
2020-06-19 15:10:57.746 a.out[23290:118809] INFO: AIDeath: entered.
2020-06-19 15:10:57.762 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.778 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.794 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.810 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.826 a.out[23290:118809] INFO: AIDeath: really dead.
2020-06-19 15:10:57.826 a.out[23290:118809] INFO: AIDeath: exited.
2020-06-19 15:10:57.826 a.out[23290:118809] INFO: Game Over!
```