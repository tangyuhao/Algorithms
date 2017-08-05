#include <iostream>

struct linkedList {
    int num;
    linkedList *next;

    linkedList(int v) : num(v), next(NULL) {}
};

linkedList *add(linkedList *pHead1, linkedList *pHead2) {
    linkedList *pSum = new linkedList(-1);
    linkedList *pTail = pSum;
    linkedList *p1 = pHead1->next;
    linkedList *p2 = pHead2->next;
    linkedList *pCur;
    int carry = 0;
    int value = 0;
    // common elements
    while (p1 && p2) {
        value = p1->num + p2->num + carry;
        carry = value > 9 ? 1 : 0;
        value = value > 9 ? value % 10 : value;
        pCur = new linkedList(value);
        pTail->next = pCur;
        pTail = pTail->next;

        p1 = p1->next;
        p2 = p2->next;
    }

    // longer list
    linkedList *p = p1 ? p1 : p2;
    while (p) {
        value = p->num + carry;
        carry = value > 9 ? 1 : 0;
        value = value > 9 ? value % 10 : value;
        pCur = new linkedList(value);
        pTail->next = pCur;
        pTail = pTail->next;
        p = p->next;
    }

    // deal with possible carry
    if (carry > 0) {
        pTail->next = new linkedList(carry);
    }
    return pSum;
}

void testAdd() {
    int inputNum;
    linkedList *pHead1 = new linkedList(-1);
    linkedList *pCur1 = pHead1;
    linkedList *pHead2 = new linkedList(-1);
    linkedList *pCur2 = pHead2;

    std::cout << "Please input the first linked list:" << std::endl;
    while (std::cin >> inputNum && 0 <= inputNum && inputNum < 10) {
        pCur1->next = new linkedList(inputNum);
        pCur1 = pCur1->next;
    }

    std::cin.clear();
    std::cin.ignore();

    std::cout << "Please input the second linked list:" << std::endl;
    while (std::cin >> inputNum && 0 <= inputNum && inputNum < 10) {
        pCur2->next = new linkedList(inputNum);
        pCur2 = pCur2->next;
    }

    linkedList *pSum = add(pHead1, pHead2);
    std::cout << "The output is:" << std::endl;
    linkedList *pCur = pSum;
    while (pCur->next) {
        pCur = pCur->next;
        std::cout << pCur->num;
    }
}

linkedList *prodOneWithMulti(int oneDigit, linkedList *pMul) {
    linkedList *pCur = pMul->next;
    linkedList *pProd = new linkedList(-1);
    linkedList *pProdCur = pProd;
    int carry = 0;
    int value = 0;
    while (pCur) {
        value = carry + oneDigit * pCur->num;
        carry = value / 10;
        value = value % 10;
        pProdCur->next = new linkedList(value);
        pProdCur = pProdCur->next;
        pCur = pCur->next;
    }
    if (carry > 0) {
        pProdCur->next = new linkedList(carry);
    }
    return pProd;
}

void testProdOneWithMulti() {
    int inputNum;
    int oneDigit;
    linkedList *pHead = new linkedList(-1);
    linkedList *pCur = pHead;

    std::cout << "Please input the one digit number:" << std::endl;
    std::cin >> oneDigit;

    std::cout << "Please input the linked list:" << std::endl;
    while (std::cin >> inputNum && inputNum >= 0 && inputNum < 10) {
        pCur->next = new linkedList(inputNum);
        pCur = pCur->next;
    }
    linkedList *pProd = prodOneWithMulti(oneDigit, pHead);
    linkedList *pCurr = pProd;
    while (pCurr->next) {
        pCurr = pCurr->next;
        std::cout << pCurr->num;
    }
}

linkedList *longIntProd(linkedList *mul1, linkedList *mul2) {
    linkedList *pCur1 = mul1;
    linkedList *pCur2 = mul2;
    linkedList *pProd = new linkedList(-1);
    pProd->next = new linkedList(0);
    int cnt = 0;
    while (pCur2->next) {
        int curDigit = pCur2->next->num;
        linkedList *pTmpSum = prodOneWithMulti(curDigit, pCur1);
        // add 0s
        for (int i = cnt; i > 0; i--) {
            linkedList *pTmp = pTmpSum->next;
            pTmpSum->next = new linkedList(0);
            pTmpSum->next->next = pTmp;
        }
        pProd = add(pProd, pTmpSum);

        pCur2 = pCur2->next;
        cnt++;
    }
    return pProd;
}

void testLongIntProd() {
    int inputNum;
    linkedList *pHead1 = new linkedList(-1);
    linkedList *pCur1 = pHead1;
    linkedList *pHead2 = new linkedList(-1);
    linkedList *pCur2 = pHead2;

    std::cout << "Please input the first linked list:" << std::endl;
    while (std::cin >> inputNum && 0 <= inputNum && inputNum < 10) {
        pCur1->next = new linkedList(inputNum);
        pCur1 = pCur1->next;
    }

    std::cin.clear();
    std::cin.ignore();

    std::cout << "Please input the second linked list:" << std::endl;
    while (std::cin >> inputNum && 0 <= inputNum && inputNum < 10) {
        pCur2->next = new linkedList(inputNum);
        pCur2 = pCur2->next;
    }

    linkedList *pSum = longIntProd(pHead1, pHead2);
    std::cout << "The production is:" << std::endl;
    linkedList *pCur = pSum;
    while (pCur->next) {
        pCur = pCur->next;
        std::cout << pCur->num;
    }
}

/*
 * start and end count from 1, not 0
 */
linkedList *reverseList(linkedList *pHead, int start, int end) {
    linkedList *reverseHead = pHead;
    for (int i = 1; i < start; i++) {
        reverseHead = reverseHead->next;
    }
    linkedList *pCur = reverseHead->next;
    for (int i = 0; i < end - start; i++) {
        linkedList *pTmp = reverseHead->next;
        linkedList *pTmp2 = pCur->next->next;
        reverseHead->next = pCur->next;
        reverseHead->next->next = pTmp;
        pCur->next = pTmp2;
    }
    return pHead;
}

void testReverseList() {
    int inputNum, start, end;
    linkedList *pHead = new linkedList(-1);
    linkedList *pCur = pHead;

    std::cout << "Please input the start and the end index, the first one's index is 1:" << std::endl;
    std::cin >> start >> end;

    std::cout << "Please input the linked list:" << std::endl;
    while (std::cin >> inputNum) {
        pCur->next = new linkedList(inputNum);
        pCur = pCur->next;
    }
    pHead = reverseList(pHead, start, end);
    linkedList *p = pHead;
    while (p->next) {
        p = p->next;
        std::cout << p->num << ' ';
    }
}

linkedList *removeAllDuplicatedElements(linkedList *pHead) {
    linkedList *pPre = pHead;
    linkedList *pCur = pPre->next;
    linkedList *pNext;
    bool bDup;
    while (pCur) {
        pNext = pCur->next;
        bDup = false;
        while (pNext && (pCur->num == pNext->num)) {
            pPre->next = pNext;
            delete(pCur);
            pCur = pNext;
            pNext = pCur->next;
            bDup = true;
        }
        if (bDup) {
            pPre->next = pNext;
            delete(pCur);
        }
        else {
            pPre = pCur;
        }
        pCur = pNext;
    }
    return pHead;
}

void testRemoveAllDuplicatedElements() {
    int inputNum;
    linkedList *pHead = new linkedList(-1);
    linkedList *pCur = pHead;
    std::cout << "Please input the linked list:" << std::endl;
    while (std::cin >> inputNum) {
        pCur->next = new linkedList(inputNum);
        pCur = pCur->next;
    }
    linkedList *pResult = removeAllDuplicatedElements(pHead);
    linkedList *p = pResult;
    while (p->next) {
        p = p->next;
        std::cout << p->num << ' ';
    }
}

int main() {
    testRemoveAllDuplicatedElements();
    return 0;
}