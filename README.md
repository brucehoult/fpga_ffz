What is it?
=====

This program is a quick and dirty hack meant to solve the problem posed at http://www.eevblog.com/forum/microcontrollers/hdl-problem-first-bit-equal-to-zero-and-set-it-to-one/

---
so, you have a vector of 1024 bit (it might be 4x bigger), they are initially all zero, and sometimes, inside a process, you need to find the lower bit set to zero, and once found you need to set it to one.

It's like find the first bit equal to zero, and set it to one so it will be skipped on future searches.

it's 1024 bit, how could you implement it in a decent way? 
As a giant circuit made on the cascade of "IF" vector(x) branches ?  ￼
---

Instructions
=====

Running "make" will will generate a bunch of LUTs as C code, then include them in another C program and simulate a million random test cases.

Examine the gemerated luts.h to see the FPGA "bitstream", sim.cpp for what interprets the FPGA, and make_luts.cpp for what generates the FPGA contents (by far the most complex part).

I get 1247 LUTs, of which 224 are 6 input LUTs used as 4:1 muxes. The other 1023 only require four inputs, with groups of three LUTs sharing the same four inputs. If you can use your 6-input LUTs as two 5-input LUTs (as I know Xilinx can) or even into four 4-input LUTs (anyone?) then the number of LUTs can be reduced to 906 or 565.

In theory you could cover 1024 inputs with 4 layers of 6-input LUTs, but presumably you want the output of the location of the first zero in binary, not some wacky form. It's only 5 layers with 4-input LUTs.

The output of whether there is a zero of not is available in 5 LUT delays and the location of the first zero in 6 LUT delays.
