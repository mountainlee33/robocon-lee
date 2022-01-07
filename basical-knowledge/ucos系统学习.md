任务延时函数(10ms一次时钟节拍中断)：OSTimeDly()，最多延时65535个节拍
建立任務，OSTaskCreate(void(*task)(void *pd),void *pdata,OS_STK *ptos,int prio),task是指向任務代碼的指針，pdata是傳遞給任務參數的指針，ptos是分配給任務的堆棧的棧頂指針，prio是分配給任務的優先級
優先級必須各不相同，堆棧空間必須聲明為OS_STK類型

launch = OSSemCreate(0)建立一個信號量，可以作爲計數量，bool型變量
OSSemDel()刪除一個信號量，刪除信號量前必須刪除操作它的所有任務
OSSemPend()等待一個信號量，信號量非0，計數值遞減；0，則任務挂起
OSSemPost()發出一個信號量，查找是否有任務等待信號量，若沒有任務等待信號量，計數值加1