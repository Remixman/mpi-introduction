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
gcc mpi-hello.c -o hello
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
2. Collective Communication - เป็นการ

## MPI_Send and MPI_Recv

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

MPI_Datatype คือชนิดของข้อมูลของ MPI ที่ใช้ส่งและรับ เช่น MPI_CHAR, MPI_INT, MPI_FLOAT เป็นต้น ชนิดข้อมูลอื่นๆ สามารถอ้างอิงที่จาก[ที่นี่](https://msdn.microsoft.com/en-us/library/dn473290%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396)

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
mpirun -np 2 ./send-recv
```

ส่งจาก 0 ไปทุกตัว

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

## MPI_Bcast

```C
MPI_Bcast(
    void* data,
    int count,
    MPI_Datatype datatype,
    int root,
    MPI_Comm communicator)
```

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

## MPI_Reduce

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

MPI_Op ที่เป็นไปได้ เช่น MPI_SUM, MPI_MAX, MPI_LAND [ที่นี่](https://msdn.microsoft.com/en-us/library/dn473436%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396)

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

## MPI_Allreduce
```C
MPI_Allreduce(
    void* send_data,
    void* recv_data,
    int count,
    MPI_Datatype datatype,
    MPI_Op op,
    MPI_Comm communicator)
```

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

## MPI_Scatter

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

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, array[];
  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == root) {
    for (i = 0; i < ; ++i) {
      array[]
    }
  }

  a = rank * 10;
  MPI_Allreduce(&a, &a, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("Value [a] of rank (%d) : %d\n", rank, a);
  
  MPI_Finalize();
  return 0;
}
```

## MPI_Gather

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

```C

```

### **Communicator**

* โปรเซสสามารถจับกลุ่มเพื่อทำงานร่วมกันได้ เรียกว่าเป็น Communicator
* ในแต่ละ Communicator โปรเซสจะมี Rank เป็นของตัวเอง
* MPI_COMM_WORLD คือ Communicator ที่มีโปรเซสทั้งหมดเป็นสมาชิก
* สามารถสร้าง Communicator ใหม่ได้เอง ด้วยฟังก์ชัน MPI_Comm_split

### **Tag**

* ในการส่ง Message หากัน สามารถกำหนดเลข Tag เพื่อช่วยแยกแยะคู่ของการรับส่งข้อมูลแต่ละครั้ง
* ในฝั่งผู้รับจะรับเฉพาะ Message ที่มี Tag ที่ตรงกับที่ต้องการเท่านั้น
* สามารถใช้ MPI_ANY_TAG เพื่อรับ Message ที่มี Tag ใดๆ ก็ได้