# Introduction to MPI

## MPI คืออะไร ?
* MPI ย่อมาจาก Message Passing Interface
* เป็น __**Library**__ สำหรับเขียนโปรแกรมแบบ Message-Passing
  * ลง Library และใช้ได้ทันที
* เป็น Standard ไม่ใช่ Implementations
  * เวอร์ชันล่าสุด 3.1 (ปี 2015)
  * [https://www.mpi-forum.org/docs](https://www.mpi-forum.org/docs)
  * มีหลาย Implementations
*  C, C++, Fortran, Java, Python, R เป็นต้น
* ใช้ได้กับสถาปัตยกรรมหลายแบบ 
  * Multiprocessor, Multi-core Processor - หลาย CPU หรือหลาย Core ในเครื่องเดียวกัน
  * Computer Cluster
* จัดการเรื่อง Network ให้เอง โปรแกรมเมอร์ไม่จำเป็นต้องเขียนในระดับ Socket

## MPI Implementations
* MPICH - [https://www.mpich.org](https://www.mpich.org)
* Open MPI - [https://www.open-mpi.org](https://www.open-mpi.org)
* MVAPICH - [http://mvapich.cse.ohio-state.edu](http://mvapich.cse.ohio-state.edu)
* Intel MPI - [https://software.intel.com/en-us/intel-mpi-library](https://software.intel.com/en-us/intel-mpi-librar)
* และอื่นๆ ไงล่ะ

## Installation (Open MPI)
* Ubuntu  `apt-get install libopenmpi-dev`
* CentOS  `yum install openmpi-devel`
* Windows - [Cygwin + Open MPI](https://www.open-mpi.org/software/ompi/v1.6/ms-windows.php)
* OS X - [Build from source](https://github.com/firemodels/fds/wiki/Installing-Open-MPI-on-OS-X)

## Simplest MPI Program

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  printf("Hello World\n");
  MPI_Finalize();
  return 0;
}
```

Save ด้วยชื่อ `mpi-hello.c` และ Compile ด้วยคำสั่ง

```
gcc mpi-hello.c -o hello -lmpi
```

Run โปรแกรมด้วยคำสั่ง

```
mpirun -np 4 ./hello
```

## Size, Rank and MPI_COMM_WORLD

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Hello World from Rank %d/%d\n", rank, size);
  
  MPI_Finalize();
  return 0;
}
```

```C
MPI_Comm_size(
    MPI_Comm communicator,
    int* size)
```

```C
MPI_Comm_rank(
    MPI_Comm communicator,
    int* rank)
```

* **size** เท่ากับจำนวนโปรเซสทั้งหมดใน MPI_Comm (MPI_COM_WORLD) ถูกกำหนดตอนรันด้วย `-np`
* **rank** หมายเลขโปรเซส มีค่าตั้งแต่่ 0 ถึง size-1

ทดลองรันโปรแกรมต่อไปนี้หลายๆ ครั้ง และสังเกต Output ที่ออกมาว่าเหมือนกันหรือไม่? เพราะเหตุใด?

## MPI Programming

* ทุกโปรเซสรันโค้ดชุดเดียวกัน และทำงานเหมือนกัน ความต่างของแต่ละโปรเซสคือหมายเลขของ rank ที่ได้
* ลำดับการทำงานขึ้นกับการจัดลำดับของ Operating System ซึ่งไม่สามารถคาดเดาได้แน่นอน ดังนั้นการ Debug จะทำได้ยาก
* **แบบฝึกหัด** ทดลองเขียนโปรแกรม MPI ให้รันด้วยโปรเซสจำนวน 8 โปรเซส โดยที่
  * โปรเซสที่ 0, 2, 4, 6 พิมพ์คำว่า `Hi! from Rank <rank>` เมื่อ `<rank>` คือหมายเลข rank ของโปรเซส
  * โปรเซสที่ 1, 3, 5, 7 พิมพ์คำว่า `Hello from Rank <rank>` เมื่อ `<rank>` คือหมายเลข rank ของโปรเซส

## Communication

จากโปรแกรมก่อนหน้าที่รัน 4 โปรเซส โปรเซสทั้งหมดทำงานเป็นอิสระต่อกัน โดยไม่มีการสื่อสาร แต่ในโปรแกรมจริงการสื่อสารเป็นสิ่งที่ขาดไม่ได้สำหรับการแก้ปัญหาแบบขนาน

คำสั่งของ MPI ที่ใช้ในการสื่อสารระหว่างโปรเซส แบ่งเป็น 2 ชนิด ได้แก่

1. Point-to-Point Communication - เป็นการสื่อสารแบบรับส่งข้อมูลระหว่าง 2 โปรเซส มีโปรเซสหนึ่งเป็นผู้ส่ง และอีกโปรเซสหนึ่งเป็นผู้รับ
2. Collective Communication - เป็นการสื่อสารที่ถูกเรียกโดยโปรเซสทั้งหมดใน Communicator 

## MPI_Send and MPI_Recv

* MPI_Send และ MPI_Recv เป็นฟังก์ชันสำหรับการสื่อสารแบบ Point-to-Point 
* MPI_Send เป็นฟังก์ชันสำหรับส่งข้อมูลจากโปรเซสหนึ่งไปยังอีกโปรเซสหนึ่ง
* MPI_Recv เป็นฟังก์ชันสำหรับรับข้อมูลที่ถูกส่งมา

```C
MPI_Send(
    void* data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator)
```

```C
MPI_Recv(
    void* data,
    int count,
    MPI_Datatype datatype,
    int source,
    int tag,
    MPI_Comm communicator,
    MPI_Status* status)
```

* MPI_Datatype คือชนิดของข้อมูลของ MPI ที่ใช้ส่งและรับ เช่น MPI_CHAR, MPI_INT, MPI_FLOAT เป็นต้น ชนิดข้อมูลอื่นๆ สามารถอ้างอิงที่จาก[ที่นี่](https://msdn.microsoft.com/en-us/library/dn473290%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396)

โปรแกรมต่อไปนี้เป็นการส่งข้อมูลจากโปรเซส 0 ไปยังโปรเซส 1

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank;
  int a;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    a = 123;
    MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (rank == 1) {
    a = 456;
    MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  printf("Process %d : a = %d\n", rank, a);
  
  MPI_Finalize();
  return 0;
}
```

```
gcc send-recv.c -o send-recv -lmpi
mpirun -np 2 ./send-recv
```

โปรแกรมต่อไปนี้เป็นการส่งข้อมูลจากโปรเซส 0 ไปยังโปรเซสอื่นทั้งหมด

```C
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, count;
  int root = 0;
  char s[20];
  count = 20;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == root) {
    strcpy(s, "STRING IN PROCESS 0");
    for (i = 1; i < size; ++i) {
      MPI_Send(s, count, MPI_CHAR, i, 100, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(s, count, MPI_CHAR, root, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Received message in process (%d) : %s\n", rank, s);
  }
  
  MPI_Finalize();
  return 0;
}
```

```
gcc send-recv2.c -o send-recv2 -lmpi
mpirun -np 8 ./send-recv2
```

## MPI_Bcast

* MPI_Bcast เป็นฟังก์ชันสำหรับกระจายข้อมูลจากโปรเซสหนึ่งไปยังโปรเซสที่เหลือทั้งหมด เป็น Collective Communication ชนิดหนึ่ง
* ต้องกำหนด root ที่เป็นโปรเซสเริ่มต้นที่จะส่งข้อมูลไปยังโปรเซสอื่น

```C
MPI_Bcast(
    void* data,
    int count,
    MPI_Datatype datatype,
    int root,
    MPI_Comm communicator)
```

โปรแกรมต่อไปนี้ทำงานเหมือนกับ `send-recv2` แต่ใช้ฟังก์ชัน MPI_Bcast ในการประจายข้อมูล

```C
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, count;
  int root = 0;
  char s[20];
  count = 20;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == root)
    strcpy(s, "STRING IN PROCESS 0");

  MPI_Bcast(s, 20, MPI_CHAR, root, MPI_COMM_WORLD);

  if (rank != root)
    printf("Received message in process (%d) : %s\n", rank, s);
  
  MPI_Finalize();
  return 0;
}
```

```
gcc bcast.c -o bcast -lmpi
mpirun -np 4 ./bcast
```

## MPI_Reduce

* MPI_Reduce เป็นฟังก์ชันสำหรับรวมข้อมูลของทุกโปรเซสมายังโปรเซส root เป็น Collective Communication ชนิดหนึ่งเช่นกัน

```C
MPI_Reduce(
    void* send_data,
    void* recv_data,
    int count,
    MPI_Datatype datatype,
    MPI_Op op,
    int root,
    MPI_Comm communicator)
```

* MPI_Op คือ Operator สำหรับการ Reduce ข้อมูล MPI_Op ที่เป็นไปได้ เช่น MPI_SUM, MPI_MAX, MPI_LAND ชนิด Operator อื่นๆ สามารถอ้างอิงที่จาก [ที่นี่](https://msdn.microsoft.com/en-us/library/dn473436%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396)

โปรแกรมต่อไปนี้ทำการหาค่าผลรวมของค่าในตัวแปร a ที่อยู่ในแต่ละโปรเซส และนำมาเก็บค่าไว้ในตัวแปร a ของโปรเซส 0

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, a;
  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  a = rank * 10;
  MPI_Reduce(&a, &a, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  printf("Value [a] of rank (%d) : %d\n", rank, a);
  
  MPI_Finalize();
  return 0;
}
```

```
gcc reduce.c -o reduce -lmpi
mpirun -np 4 ./reduce
```

## MPI_Allreduce

* MPI_Allreduce ทำงานเหมือนกับ MPI_Reduce แต่ละไม่มี root สำหรับ Reduce ข้อมูล เมื่อทำงานเสร็จ ทุกโปรเซสจะได้ค่าที่ Reduce แล้วเหมือนกันทั้งหมด
* สามารถมองได้เหมือนกับเป็นการใช้ฟังก์ชัน MPI_Reduce และตามด้วย MPI_Bcast

```C
MPI_Allreduce(
    void* send_data,
    void* recv_data,
    int count,
    MPI_Datatype datatype,
    MPI_Op op,
    MPI_Comm communicator)
```

โปรแกรมต่อไปนี้ทำงานเหมือนกับโปรแกรม reduce แต่ทุกโปรเซสจะได้ค่าผลรวมที่หาได้

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, a;
  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  a = rank * 10;
  MPI_Allreduce(&a, &a, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("Value [a] of rank (%d) : %d\n", rank, a);
  
  MPI_Finalize();
  return 0;
}
```

```
gcc reduce2.c -o reduce2 -lmpi
mpirun -np 4 ./reduce2
```

## MPI_Scatter

* MPI_Scatter ทำงานโดยการกระจายข้อมูลจากโปรเซส root ไปยังโปรเซสทั้งหมด แต่ไม่เหมือนกับ MPI_Bcast ตรงที่ MPI_Bcast จะส่งข้อมูลทั้งหมดไปให้กับทุกโปรเซส แต่ MPI_Scatter จะแบ่งข้อมูลเป็นส่วนๆ และส่งแต่จะส่วนให้กับแต่ละโปรเซส

```C
MPI_Scatter(
    void* send_data,
    int send_count,
    MPI_Datatype send_datatype,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_datatype,
    int root,
    MPI_Comm communicator)
```

โปรแกรมต่อไปนี้จะทำการหาผลรวมของ 1 ถึง 200 (สมมติให้จำนวนโปรเซสหาร 200 ลงตัว)

```C
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, numbers[200];
  int num_per_proc, *sub_nums;
  int root = 0, sub_sum = 0, sum = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  num_per_proc = 200 / size;

  if (rank == root) {
    for (i = 0; i < 200; ++i) 
      numbers[i] = i + 1;
  }

  sub_nums = (int*) malloc(num_per_proc * sizeof(int));
  MPI_Scatter(numbers, num_per_proc, MPI_INT, sub_nums, num_per_proc, MPI_INT, root, MPI_COMM_WORLD);

  for (i = 0; i < num_per_proc; i++)
    sub_sum += sub_nums[i];

  MPI_Reduce(&sub_sum, &sum, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

  if (rank == root) {
    printf("Summation of 1 to 200 is %d\n", sum);
  }

  free(sub_nums);

  MPI_Finalize();
  return 0;
}
```

```
gcc sum.c -o sum -lmpi
mpirun -np 4 ./sum
```

## MPI_Gather

* MPI_Gather ทำงานตรงกันข้ามกับ MPI_Scatter คือเป็นการรวบรวมข้อมูลทั้งหมดมาไว้ที่โปรเซส root ซึ่งต่างจาก MPI_Reduce จะมีการ Reduce ข้อมูล แต่ MPI_Gather จะนำข้อมูลแต่ละส่วนมาต่อกันเป็นชิ้นเดียว

```C
MPI_Gather(
    void* send_data,
    int send_count,
    MPI_Datatype send_datatype,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_datatype,
    int root,
    MPI_Comm communicator)
```

ตัวอย่างโปรแกรมต่อไปนี้จะหาค่า 2 เท่าของทุกสมาชิกในอาร์เรย์ ซึ่งจะทำงานโดยการแบ่งข้อมูลในอาร์เรย์ออกเป็นส่วนๆ แและกระจายไปยังแต่ละโปรเซสด้วยฟังก์ชัน MPI_Scatter จากนั้นแต่ละโปรเซสจะนำส่วนของข้อมูลที่ตนเองถืออยู่ไปหาค่าใหม่ (ด้วยการคูณ 2) จากนั้นทุกโปรเซสจะส่งข้อมูลที่คำนวนเสร็จแล้วกลับไปยังโปรเซส root (หมายเลข 0)

```C
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, numbers[200];
  int num_per_proc, *sub_nums;
  int root = 0, sub_sum = 0, sum = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  num_per_proc = 200 / size;

  if (rank == root) {
    for (i = 0; i < 200; ++i) 
      numbers[i] = i + 1;
  }

  sub_nums = (int*) malloc(num_per_proc * sizeof(int));
  MPI_Scatter(numbers, num_per_proc, MPI_INT, sub_nums, num_per_proc, MPI_INT, root, MPI_COMM_WORLD);

  for (i = 0; i < num_per_proc; i++)
    sub_nums[i] *= 2;

  MPI_Gather(sub_nums, num_per_proc, MPI_INT, numbers, num_per_proc, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (i = 0; i < 200; i++)
      printf("%5d");
    printf("\n");
  }

  free(sub_nums);

  MPI_Finalize();
  return 0;
}
```

```
gcc double.c -o double -lmpi
mpirun -np 4 ./double
```

## MPI_Allgather

* คล้ายกับ MPI_Reduce ที่มี MPI_Allreduce ฟังก์ชัน MPI_Gather ก็มีเวอร์ชันที่ข้อมูลหลังจากที่รวบรวมทั้งหมดถูกส่งไปยังทุกโปรเซส ซึ่งก็คือฟังก์ชัน MPI_Allgather
* MPI_Allgather มีลักษณะการทำงานเหมือนกับการเรียก MPI_Gather และตามด้วย MPI_Bcast
* ไม่จำเป็นต้องกำหนดโปรเซส root

```C
MPI_Allgather(
    void* send_data,
    int send_count,
    MPI_Datatype send_datatype,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_datatype,
    MPI_Comm communicator)
```

## MPI_Scatterv and MPI_Gatherv

* MPI_Scatter จะถือว่าทุกโปรเซสจะต้องรับข้อมูลในขนาดเท่ากันเสมอ (ตัวแปร send_count) แต่ในโปรแกรมจริงไม่จำเป็นที่เราจะต้องส่งข้อมูลในเท่ากันทุกโปรเซส
* เช่น ในตัวอย่างของ MPI_Scatter ถ้าหากขนาดของอาร์เรย์หารด้วยจำนวนโปรเซสไม่ลงตัว แต่ละโปรเซสก็ควรจะได้รับจำนวนข้อมูลไม่เท่ากัน ซึ่งเราอาจจะแก้ปัญหาด้วยการ
  1. เพิ่มขนาดอาร์เรย์ให้เป็นจำนวนเท่าของโปรเซส แล้วข้ามการทำงานในส่วนที่เกินจาก้อมูลจริงออกมา
  2. ใช้ MPI_Scatterv ที่สามารถกำหนดขนาดที่แต่ละโปรเซสจะได้รับได้ (send_counts)
* ตัวแปรอาร์เรย์ displs ที่เพิ่มขึ้นมา เป็นตัวบอกตำแหน่งเริ่มต้นของข้อมูลใน send_data ที่จะส่งให้แต่ละโปรเซส

```C
int MPI_Scatterv(
    void *send_data, 
    int *send_counts, 
    int *displs,
    MPI_Datatype send_datatype,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_datatype,
    int root,
    MPI_Comm communicator)
```

* เช่นเดียวกันกับ MPI_Gether ที่ข้อมูลที่ส่งมาจากแต่ละโปรเซสไม่จำเป็นที่จะต้องมีขนาดเท่ากันเสมอไป ซึ่งการใช้ฟังก์ชัน MPI_Gatherv แทนจะช่วยให้เราสามารถกำหนดขนาดข้อมูลที่จะรวบรวมมาของแต่ละโปรเซสได้

```C
int MPI_Gatherv(
    void* send_data, 
    int send_count, 
    MPI_Datatype send_datatype,
    void *recv_data, 
    const int *recv_counts, 
    const int *displs,
    MPI_Datatype recv_datatype, 
    int root, 
    MPI_Comm communicator)
```

## Miscellaneous

### **Communicator**

* โปรเซสสามารถจับกลุ่มเพื่อทำงานร่วมกันได้ เรียกว่าเป็น Communicator
* ในแต่ละ Communicator โปรเซสจะมี Rank เป็นของตัวเอง
* MPI_COMM_WORLD คือ Communicator ที่มีโปรเซสทั้งหมดเป็นสมาชิก
* สามารถสร้าง Communicator ใหม่ได้เอง ด้วยฟังก์ชัน MPI_Comm_split

### **Tag**

* ในการส่ง Message หากัน สามารถกำหนดเลข Tag เพื่อช่วยแยกแยะคู่ของการรับส่งข้อมูลแต่ละครั้ง
* ในฝั่งผู้รับจะรับเฉพาะ Message ที่มี Tag ที่ตรงกับที่ต้องการเท่านั้น
* สามารถใช้ MPI_ANY_TAG เพื่อรับ Message ที่มี Tag ใดๆ ก็ได้