Program inspired by http://www.eevblog.com/forum/microcontrollers/hdl-problem-first-bit-equal-to-zero-and-set-it-to-one

so, you have a vector of 1024 bit (it might be 4x bigger), they are initially all zero, and sometimes, inside a process, you need to find the lower bit set to zero, and once found you need to set it to one.

It's like find the first bit equal to zero, and set it to one so it will be skipped on future searches.

it's 1024 bit, how could you implement it in a decent way? 
As a giant circuit made on the cascade of "IF" vector(x) branches ?  ￼
