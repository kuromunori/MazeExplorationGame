# 迷路探査ゲーム


## 概要
理系高校生向けのイベントを企画した際に作成したプログラムです。

企画のルール考案は私が中心となって数人で行い、プログラム本体は私1人で書きました。

本Github上のデータは、パワーポイントやPDFを含め基本的に私1人による著作物です。(DXライブラリのみ例外)

## 動作確認環境
・DXライブラリ ver3.22c + Windows 10 Home

・Visual Studio 2022

## プログラム説明
本プログラムは、以下の迷路探査アルゴリズム作成ゲームの提出動作確認及び、模範解答を見せるためのものです。

__お題：10×10マスの迷路内に存在する、5つの☆マスのうち3つ以上を通過するアルゴリズムをフローチャート形式で作れ。ただし使用されたコマンドがより少なくなるように工夫すること__

<p align="center">
 <img src="https://user-images.githubusercontent.com/18481351/206546977-d9fbcb81-4830-48b4-a492-06c6815a2bc8.png" width=320px>
</p>
ゲームの詳細なルールは、「スライド等/迷路探査ゲーム ルール説明.pptx」をご覧ください。

### 1.模範解答を見せるプログラム
「実行ファイル/Explanation.exe」を実行してください。5コマンドですべての☆マスを通るアルゴリズムのアニメーションが描画されます。
#### 操作方法
上矢印キー:アニメーションのスピードアップ

下矢印キー:アニメーションのスピードダウン

スペースキー：アニメーションのスピードリセット

エンターキー：初期化

<p align="center">
 <img src="https://user-images.githubusercontent.com/18481351/206564607-a9c7d6cf-4170-419b-8f07-5429dc72a004.png">
</p>

### 2.提出動作確認用のプログラム
「実行ファイル/LoadScript.exe」を実行してください。同階層のcode.txt内に書かれた自作スクリプト言語を読み、その動きを実行します。2種類のアルゴリズムcode.txtとcode2.txtが添付されているので、実行したい方のファイル名をcode.txtに書き換えてからexeを実行してください。
#### 操作方法
上矢印キー:アニメーションのスピードアップ

下矢印キー:アニメーションのスピードダウン

スペースキー：アニメーションのスピードリセット

エンターキー：特別賞(410コマンド実行した時点で何マス踏破できたか)を計測するための隠しコマンド
<p align="center">
 <img src="https://user-images.githubusercontent.com/18481351/206565176-c11bb29b-65c5-48b8-90e3-152ca709c03c.png">
</p>


## コンパイル方法
### 1. Visual Studio 2022をインストール
### 2. モードを指定してコンパイル

#### 方法0. Tsubasa.slnをVisual Studio 2022で開く
![説明1](https://user-images.githubusercontent.com/18481351/206540092-7dbd6595-e1dc-4c4e-9164-5b0fc016c31e.png)

#### 方法1. main.cppを開き、9行目のmodeをexplanation_modeとload_script_modeから選択
それぞれ、「実行ファイル/Explanation.exe」と「実行ファイル/LoadScript.exe」に対応するプログラムが「Release/Tsubasa.exe」として生成されます。
#### 方法2. Release,x86になっていることを確認し、実行ボタンを押す


## ファイル構成
主要なファイルのみ紹介します
<pre>
リポジトリTOP
│
├ Tsubasa.sln .. Visual Studio 用のSolutionファイル
│
├ DxLib .. コンパイル用に再配布したDXライブラリ
│
├ Tsubasa ..　作業ディレクトリ
│　├ CommandGraph.h  .. 自作スクリプト言語を読み込む用のプログラム【自作】
│　├ HanchoRobotCommand.h .. ロボットおよび盤面を更新するためのプログラム【自作】
│　├ main.cpp .. 大元の呼び出しプログラム【自作】
│　└ Input.h, Input.cpp .. キー入力受付用のプログラム 【テンプレを流用】
│　 
│
├ 実行ファイル .. コンパイル済みの実行ファイル
│　├ Explanation.exe　.. 模範解答のアニメーション描画用のプログラム
│　├ LoadScript.exe .. 自作スクリプト言語を読み込み動作確認するためのプログラム
│　└ code.txt, code2.txt .. 自作スクリプト言語の例
│
├ スライド等 ..　解説用のパワーポイントやスライド置き場
│　├ 迷路探査ゲーム　ルール説明.pptx .. ゲームの詳細なルールやコマンドの説明
│　├ コマンド一覧.pdf .. コマンドの一覧
│　├ 自作スクリプト言語の書き方.pptx .. フローチャート→スクリプト言語への変換方法
│　├ アルゴリズム紹介.pptx .. 実行ファイル/code.txt および code2.txtのアルゴリズムの紹介
│　└ 解説.pptx .. 模範解答に至るまでの解説
</pre>
## 著作権
本プログラムはDXライブラリを使用して作られました。
また、このレポジトリ単独でコンパイルできるよう、規約に則りながらDXライブラリを含んでの再配布となっております。

DX Library Copyright (C) 2001-2022 Takumi Yamada.
