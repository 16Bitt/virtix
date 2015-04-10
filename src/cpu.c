#include "common.h"
#include "cpu.h"
#include "monitor.h"
#include "virtix_page.h"

char* cpuid_string = "RiseRiseRise";

char* cpu_name(){
	int eax, ebx, ecx, edx;
	
	asm volatile ("movl $0, %eax");
	asm volatile ("cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx));
	
	int* strtoint = (int*) cpuid_string;
	strtoint[0] = ebx;
	strtoint[1] = edx;
	strtoint[2] = ecx;

	return cpuid_string;
}

uint cpu_features(){
	int eax, ebx, ecx, edx;
	
	asm volatile ("movl $1, %eax");
	asm volatile ("cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx));
	return edx;
}

void cpu_check_fpu(){
	vga_fmt("CPU has features: %X\n", cpu_features());
	ASSERT(cpu_features() & CPU_FEATURE_FPU);
}
