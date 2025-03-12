#include "DB.h"


TABLE *openDB(const char *fileName) {
    PAGER *pager = openPager(fileName);
    uint32_t numRows = pager->fileLen / ROW_SIZE;
    TABLE *table = (TABLE *)malloc(sizeof(TABLE));
    table->pager = pager;
    table->numRows = numRows;

    return table;
}

void closeDB(TABLE *table) {
    PAGER *pager = table->pager;
    uint32_t numOfFullPages = table->numRows / ROWS_PER_PAGE;

    for(uint32_t i = 0; i < numOfFullPages; i++) {
        if(pager->pages[i] == NULL) {
            continue;
        }
        flushPager(pager, i, PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    uint32_t numOfAdditionalRows = table->numRows % ROWS_PER_PAGE;
    if(numOfAdditionalRows > 0) {
        uint32_t pageNum = numOfFullPages;
        if(pager->pages[pageNum] != NULL) {
            flushPager(pager, pageNum, numOfAdditionalRows * ROW_SIZE);
            free(pager->pages[pageNum]);
            pager->pages[pageNum] = NULL;
        }
    }

    int result = close(pager->fileDesc);
    if(result == -1) {
        printf("Error closing DB file.\n");
        exit(EXIT_FAILURE);
    }

    for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        void *page = pager->pages[i];
        if(page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);
    free(table);
}