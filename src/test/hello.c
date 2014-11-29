char x = 5;
char z = 0;

void main(){
	asm volatile ("int $0");
	for(;;);
}
