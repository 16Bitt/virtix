#ifndef CPU_H
#define CPU_H

//ONLY CALL THESE ONCE PAGING IS ACTIVATED
//Otherwise, there isn't any fault protection if CPUID isn't available

#define CPU_FEATURE_FPU	1

char* cpu_name();	//Return the CPU name from CPUID
uint cpu_features();	//Get the EDX feature string from CPUID
void cpu_check_fpu();	//Panic if there isn't an FPU

#endif
