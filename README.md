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

1. Point-to-Point Communication - 
2. Collective Communication - 

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

## MPI_Bcast

```C
MPI_Bcast(
    void* data,
    int count,
    MPI_Datatype datatype,
    int root,
    MPI_Comm communicator)
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

##
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