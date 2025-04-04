#include "DB.h"


TABLE *openDB(const char *fileName) {
    PAGER *pager = openPager(fileName);

    TABLE *table = (TABLE *)malloc(sizeof(TABLE));
    table->pager = pager;
    table->rootPageNum = 0;

    if(pager->numPages == 0) {
        void *rootNode = getPage(pager, 0);
        initializeLeafNode(rootNode);
        setNodeRoot(rootNode, true);
    }

    return table;
}

void closeDB(TABLE *table) {
    PAGER *pager = table->pager;

    for(uint32_t i = 0; i < pager->numPages; i++) {
        if(pager->pages[i] == NULL) {
            continue;
        }
        flushPager(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
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