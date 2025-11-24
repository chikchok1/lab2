#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("사용법: %s <원본파일> <복사파일>\n", argv[0]);
        exit(1);
    }

    const char *srcFile = argv[1];
    const char *dstFile = argv[2];

    // 1. 원본 파일 정보 가져오기 (크기 등)
    struct stat st;
    if (stat(srcFile, &st) == -1) {
        perror("stat error");
        exit(1);
    }
    size_t fileSize = st.st_size;

    // 2. 원본 파일 열기
    int srcFd = open(srcFile, O_RDONLY);
    if (srcFd == -1) {
        perror("open srcFile");
        exit(1);
    }

    // 3. 원본 파일을 mmap으로 메모리에 매핑
    void *srcMap = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, srcFd, 0);
    if (srcMap == MAP_FAILED) {
        perror("mmap error");
        exit(1);
    }

    // 4. 공유 메모리 생성
    int shmid = shmget(IPC_PRIVATE, fileSize, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget error");
        exit(1);
    }

    // 5. 공유 메모리 연결
    void *shmPtr = shmat(shmid, NULL, 0);
    if (shmPtr == (void *)-1) {
        perror("shmat error");
        exit(1);
    }

    // 6. mmap된 원본 파일 데이터를 공유 메모리에 복사
    memcpy(shmPtr, srcMap, fileSize);

    // 7. 복사될 파일 생성
    int dstFd = open(dstFile, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (dstFd == -1) {
        perror("open dstFile");
        exit(1);
    }

    // 8. 복사 파일 크기 지정
    if (ftruncate(dstFd, fileSize) == -1) {
        perror("ftruncate error");
        exit(1);
    }

    // 9. 복사 파일 mmap
    void *dstMap = mmap(NULL, fileSize, PROT_WRITE, MAP_SHARED, dstFd, 0);
    if (dstMap == MAP_FAILED) {
        perror("mmap error");
        exit(1);
    }

    // 10. 공유 메모리의 데이터를 복사 파일 mmap으로 복사
    memcpy(dstMap, shmPtr, fileSize);

    // 11. 자원 해제
    munmap(srcMap, fileSize);
    munmap(dstMap, fileSize);
    close(srcFd);
    close(dstFd);
    shmdt(shmPtr);
    shmctl(shmid, IPC_RMID, NULL);

    printf("파일 복사 완료: %s → %s\n", srcFile, dstFile);
    return 0;
}
