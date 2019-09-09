# Makefile

# 変更すべき箇所を1箇所に集約するため，マクロを使用する
#
# マクロ (変数) 名 = 値 (数字，文字列...)で定義し，
# $(変数名) で値を参照する

# "#"で始まる行はコメント行として扱われる

#----------------------------------------------------------------------
# Cコンパイラの登録
CC = gcc
CXX = g++

#----------------------------------------------------------------------
# AutoGL(おてがる)のファイルの場所の設定
# このMakefileの置かれたディレクトリからの相対パス
AUTOGL_HEADER_DIR = lib/autogl
AUTOGL_LIBRARY_DIR = lib/autogl

# AutoGL(おてがる)のライブラリ名の設定
LIBAUTOGL = $(AUTOGL_LIBRARY_DIR)/libautogl.a

#----------------------------------------------------------------------
# コンパイル時に読み込むファイルの設定
INCLUDES = -I$(AUTOGL_HEADER_DIR)
# コンパイルオプション
CFLAGS = $(INCLUDES) -g -Wall -O03
CXXFLAGS = $(INCLUDES) -g -Wall -O03

# リンクするライブラリの設定
LIBS = -L$(AUTOGL_LIBRARY_DIR) -lautogl -lm
LIBS += `${AUTOGL_LIBRARY_DIR}/advautogl-config`
# リンクオプション
LDFLAGS = $(LIBS)

#----------------------------------------------------------------------
# 作成する実行ファイルの名称
# (無指定でビルドして得られる "a.out" に相当するもの)
PROGRAM = traffic

#----------------------------------------------------------------------
# .cファイルをコンパイルし，.oファイルを作成するための設定
#  (サフィックスルールと呼ばれる)
.c.o:
	$(CC) -c $< $(CFLAGS)

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

# ちなみに.oファイルは"オブジェクトファイル"と呼ばれる．
# .cファイルと実行形式のファイルの間にある"中間ファイル"である．

# 実行ファイルを構成するオブジェクトファイル
# 新たなファイル (.cファイル) を追加した場合はここに追加する
# \ で改行して続ける
OBJ =   main.o \
	Drawer.o \
	Car.o \
	MathAux.o \
	RoadMapBuilder.o \
	Node.o \
	Link.o \
	Visualizer.o

#----------------------------------------------------------------------
# "make"と打って作成されるプログラムの指定
all : $(LIBAUTOGL) $(PROGRAM)

# 以下は次のようなルールの記述
# (厳密にはルールではなく"ターゲット"と呼ばれる)

# ルール名 : ルールの依存対象
# [TAB] ルールの依存対象が更新された時に実行されるコマンド

# なお"$@"は特殊なマクロで，ターゲット名を表す

# 以下は"lib/autogl/libautogl.a"という名前のルールであり，
# "lib/autogl/libautogl.a"というファイルを作成するためのもの
#
# lib/autogl/*.c, lib/autogl/*.hというファイルに依存し，
# それらのファイルに更新があれば，lib/autogl/ディレクトリに移動し (cd)
# そこで改めてmakeを実行する，という意味
$(LIBAUTOGL) : $(AUTOGL_LIBRARY_DIR)/*.[ch]
	cd $(AUTOGL_LIBRARY_DIR); $(MAKE)

# 以下は"traffic"という名前のルールであり
# "traffic"というファイルを作成するためのもの
#
# $(OBJ)で定義されたオブジェクトファイルに依存し，
# それらのファイルに更新があれば，改めてファイルをまとめて (リンクして)
# trafficという実行形式のファイルを作成する，という意味
$(PROGRAM) : $(OBJ) $(LIBAUTOGL)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# これは通常の"make"でなく，"make clean"と打った時に実行されるルール
# 慣習として，"clean"というルールは中間ファイルと実行ファイルを削除する
clean :
	-rm *.o $(PROGRAM) $(PROGRAM).exe

# これはflymake用の設定
check-syntax :
	case $(CHK_SOURCES) in \
	*.c) \
		$(CC) $(CFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES) ; \
		;; \
	*) \
		;; \
	esac
