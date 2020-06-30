# 「ゲームプログラミングC++」

このリポジトリは、Sanjay Madhav著「ゲームプログラミングC++」のソースコードです。

各章のソースコードは三条項BSDライセンスで公開されています。詳細は`LICENSE`ファイルを参照してください。
このライセンスは`External`ディレクトリのコードには適用されないことに注意してください。`External`
ディレクトリの各プロジェクトはそれぞれにライセンスされています。

# コードのビルド

各章のコードはMicrosoft WindowsとApple macOSでテストされており、両OSで稼働します。

Windows上でコンパイルするには、[Microsoft Visual Studio 2017 Community版](https://www.visualstudio.com/downloads/)をインストールしてください。インストールの過程でワークフロー
"Game Development in C++" を選択してください。各章用のディレクトリに対応する`ChapterXX-windows.sln`がありますので、それを開いてください。

macOSでコンパイルするには、App StoreからXcodeをインストールしてください。各章には対応する`ChapterXX-mac.xcodeproj`ファイルがあります。

7章以降のコードではオーディオ用にFMOD APIを使用します。これは[ここ](https://www.fmod.com/download)から
インストールする必要があります。`FMOD Studio API`の`1.09.x`をダウンロード・インストールして
ください（新しい版はテストされていません）。WindowsではFMODをデフォルトディレクトリにインストール
してください。MacではFMODパッケージの内容を`External/FMOD`にコピーしてください。
