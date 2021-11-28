int read(){
	int a=0;
	a = getchar();
	return a - 48;
}
int main(){
	int m,n;
	int i=48; 
	m = read();
	n = read();
    if(m == n ){
        int i = 1;
        i = i + 48;  //49
        putchar(i);  //49  1
    }else{
        i = i + 2;  //50  2
        putchar(i);
    }
    i = i + 1;  //51  1
    putchar(i);
	return 0;
}