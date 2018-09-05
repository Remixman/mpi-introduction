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

```Bash
gcc mpi-hello.c -o hello
```

