#include "Pager.h"

PAGER *openPager(const char *fileName) {
    int fd = open(
        fileName,
        O_RDWR |                // Read and write mode
        O_CREAT,                // Creates file if there is none
        S_IWUSR |               // Grants permission to user to write
        S_IRUSR                 // Grants permission to user to read
    );

    if(fd == -1) {
        printf("Unable to open file.\n");
        exit(EXIT_FAILURE);
    }

    off_t fileLen = lseek(fd, 0, SEEK_END);

    PAGER *pager = (PAGER *)malloc(sizeof(PAGER));
    pager->fileDesc = fd;
    pager->fileLen = fileLen;
    pager->numPages = (fileLen / PAGE_SIZE);

    if(fileLen % PAGE_SIZE != 0) {
        printf("DB file is not a whole number of pages. Corrupt file!\n");
        exit(EXIT_FAILURE);
    }

    for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

void *getPage(PAGER *pager, uint32_t pageNum) {
    if(pageNum > TABLE_MAX_PAGES) {
        printf("Tried to fetch page num out of bounds. %d > %d\n", pageNum, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if(pager->pages[pageNum] == NULL) {
        void *page = malloc(PAGE_SIZE);
        uint32_t numOfPages = pager->fileLen / PAGE_SIZE;

        if(pager->fileLen % PAGE_SIZE) {
            numOfPages += 1;
        }

        if(pageNum <= numOfPages) {
            lseek(pager->fileDesc, pageNum * PAGE_SIZE, SEEK_SET);
            ssize_t bytesToRead = read(pager->fileDesc, page, PAGE_SIZE);

            if(bytesToRead == -1) {
                printf("Error reading file: %d\n", errno);
            }
        }

        pager->pages[pageNum] = page;

        if(pageNum >= pager->numPages) {
            pager->numPages = pageNum + 1;
        }
    }

    return pager->pages[pageNum];
}

void flushPager(PAGER *pager, uint32_t pageNum) {
    if(pager->pages[pageNum] == NULL) {
        printf("Tried to flush NULL page.\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->fileDesc, pageNum * PAGE_SIZE, SEEK_SET);

    if(offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten = write(pager->fileDesc, pager->pages[pageNum], PAGE_SIZE);

    if(bytesWritten == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}