rogue-clone: Version III for Linux/Windows

	Copyright (c) 1988 The Regents of the University of 
	California.  All rights reserved.  

	This code is derived from software contributed to Berkeley 
	by Timothy C. Stoehr.  

	Revision by Y.Oz (Y.Oz Vox)  [yozvox at yahoo dot co dot jp]



I revised it about the following items.  

1. I ported this software to Windows console (Cygwin).  Of course you 
  can run this software on Linux.  

2p. You can use the "color" option in environment variable 
  "ROGUEOPTS".  This adds a color to a map of the dungeon.  I took a 
  color of IBM-PC version Rogue into account.  You should use the 
  "bright" option if you expect the color of NEC PC-9801/8801 version.  

3. On a scoreboard, one player can print ranking in only one.  I 
  changed a program so that one player could have plural ranking.  

4. I changed a Rogue program to be able to set a user name with a 
  command-line option.  From a command-line, please input a user name 
  after "-n" option (or, "-u") as follows, and run.  You can use "?" 
  for name character string to input a name after starting software.  

	rogue -n username
	rogue -n ?

5. You can use the "name=?" option in environment variable 
  "ROGUEOPTS".  When you begin a game, Rogue will hear your name.  

6p. You can use "-11" or "-16" option with a command-line option.  It 
  is the same as "ROGUEOPTS=passgo,color,bright,name=?" ("bright" is 
  the case of "-16").  You can use "-11 username" or "-16 username", 
  too.  

7p. When you make a "lan.cfg" file of contents with "/home/username/" 
  or "C:\", Rogue comes to make a score file there.  The path name must 
  be over in "/" or "\".  When you set shared folders such as the 
  office LAN in a "lan.cfg" file, you can compete with an in-house 
  member for score of Rogue.  

8p. When you press the "-" key (toggle), keys for movement ("hjklyubn") 
  will be treated with "[Ctrl]+" those keys.  

9. For a person except the English zone, I bundled batch file to run a 
  command-line with English mode.  Please double-click "cmd_us.bat".  

note) "p" are only for "p" version.  

---*---*---*---

以下の点について修正を行いました。

1. これはrogue-clone: Version IIIをWindowsのコンソールプログラム
  （Cygwin）として移植したものです。もちろん、Linux上で動作させることも
  可能です。

2p. 環境変数「ROGUEOPTS」に「color」オプションが使えます。これはマップを
  カラー表示にするためのものです。配色は、IBM-PC版Rogueを参考にしていま
  す。「bright」にすると、NEC PC-9801/8801版の配色を参考にしたものになり
  ます。

3. スコアボード上で、１人のプレイヤーは１つのランキングしか残せません。
  １人のプレイヤーが複数のランキングを残せるように変更しました。

4. ユーザの名前をコマンドラインから指定できるようにプログラムを変更して
  あります。コマンドラインから、「-n」（もしくは「-u」）に続けて名前を指
  定して実行してください。ゲーム開始後に名前の文字列を入力するようにする
  には、名前に「?」を指定してください。

	rogue -n username
	rogue -n ?

5. 環境変数「ROGUEOPTS」に「name=?」を用いることもできます。ゲームを始め
  ると、Rogueは名前を聞いてくるでしょう。

6p. コマンドラインから、「-11」、「-16」オプションが使えます。
  "ROGUEOPTS=passgo,color,bright,name=?"（"bright"は「-16」の場合のみ）
  で起動したのと同じ状態になります。「-11 username」や「-16 username」も
  使えます。

7p. あなたが、「/home/username/」とか「C:\」とかの内容の「lan.cfg」ファ
  イルを作成しておくと、Rogueはそこにスコアファイルを作成するようになり
  ます。パス名は、「/」か「\」で終わっていなければなりません。あなたが、
  社内LAN等の共有フォルダを、「lan.cfg」ファイルに設定しておくと、あなた
  は社内のメンバーとRogueのスコアを競うことができます。

8p. 「-」（トグル）を押すと、移動キー（「hjklyubn」）が「[Ctrl]+」で扱わ
  れます。

9. 英語圏以外の人のために、英語モードのコマンドラインを立ち上げるバッチ
  ファイルを添付しました。「cmd_us.bat」をダブルクリックしてください。

注）「p」は「p」バージョン専用です。

---*---*---*---
