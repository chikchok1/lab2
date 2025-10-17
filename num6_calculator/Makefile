# 📘 Makefile for Arithmetic Library Test

CC = gcc
CFLAGS = -Wall -I./lib
LIBDIR = lib

# 라이브러리 파일 이름
STATIC_LIB = $(LIBDIR)/libarith.a
SHARED_LIB = $(LIBDIR)/libarith.so

# 객체 파일
OBJS = $(LIBDIR)/arith.o

# 실행 파일
TARGET_STATIC = main_static
TARGET_SHARED = main_shared
TARGET_DYNAMIC = main_dynamic

# 기본 명령
all: $(TARGET_STATIC) $(TARGET_SHARED) $(TARGET_DYNAMIC)

# ---------- 정적 라이브러리 ----------
$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^

$(TARGET_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L$(LIBDIR) -larith -o $@

# ---------- 공유 라이브러리 ----------
$(SHARED_LIB): $(OBJS)
	$(CC) -shared -o $@ $^

$(TARGET_SHARED): main.c $(SHARED_LIB)
	$(CC) $(CFLAGS) main.c -L$(LIBDIR) -larith -o $@

# ---------- 동적 로드 실행 ----------
$(TARGET_DYNAMIC): main_dynamic.c $(SHARED_LIB)
	$(CC) $(CFLAGS) main_dynamic.c -ldl -o $@

# ---------- 객체 파일 ----------
$(LIBDIR)/arith.o: $(LIBDIR)/arith.c $(LIBDIR)/arith.h
	$(CC) $(CFLAGS) -fPIC -c $(LIBDIR)/arith.c -o $@

# ---------- 실행 ----------
run_static:
	./$(TARGET_STATIC)

run_shared:
	export LD_LIBRARY_PATH=$(LIBDIR):$$LD_LIBRARY_PATH; ./$(TARGET_SHARED)

run_dynamic:
	export LD_LIBRARY_PATH=$(LIBDIR):$$LD_LIBRARY_PATH; ./$(TARGET_DYNAMIC)

# ---------- 정리 ----------
clean:
	rm -f $(LIBDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so $(TARGET_STATIC) $(TARGET_SHARED) $(TARGET_DYNAMIC)
