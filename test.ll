; ModuleID = 'test'
source_filename = "test"

declare i32 @putchar(i32)

declare i32 @getchar()

define i32 @read() {
entry:
  %a = alloca i32
  store i32 0, i32* %a
  %b = alloca i32
  %0 = load i32, i32* %b
  %1 = call i32 @getchar()
  store i32 %1, i32* %b
  br label %whcond

whcond:                                           ; preds = %whrun, %entry
  %2 = load i32, i32* %b
  %3 = icmp ne i32 %2, 10
  br i1 %3, label %whrun, label %whafter

whrun:                                            ; preds = %whcond
  %4 = load i32, i32* %a
  %5 = load i32, i32* %a
  %6 = mul i32 %5, 10
  %7 = load i32, i32* %b
  %8 = add i32 %6, %7
  %9 = sub i32 %8, 48
  store i32 %9, i32* %a
  %10 = load i32, i32* %b
  %11 = call i32 @getchar()
  store i32 %11, i32* %b
  br label %whcond

whafter:                                          ; preds = %whcond
  br label %ret

ret:                                              ; preds = %whafter
  %12 = load i32, i32* %a
  ret i32 %12
}

define i32 @write(i32 %a) {
entry:
  %a1 = alloca i32
  store i32 %a, i32* %a1
  %b = alloca i32
  store i32 1, i32* %b
  %c = alloca i32
  br label %whcond

whcond:                                           ; preds = %whrun, %entry
  %0 = load i32, i32* %b
  %1 = load i32, i32* %a1
  %2 = icmp ule i32 %0, %1
  br i1 %2, label %whrun, label %whafter

whrun:                                            ; preds = %whcond
  %3 = load i32, i32* %b
  %4 = load i32, i32* %b
  %5 = mul i32 %4, 10
  store i32 %5, i32* %b
  br label %whcond

whafter:                                          ; preds = %whcond
  %6 = load i32, i32* %b
  %7 = load i32, i32* %b
  %8 = sdiv i32 %7, 10
  store i32 %8, i32* %b
  br label %whcond2

whcond2:                                          ; preds = %whrun3, %whafter
  %9 = load i32, i32* %a1
  %10 = icmp sge i32 %9, 10
  br i1 %10, label %whrun3, label %whafter4

whrun3:                                           ; preds = %whcond2
  %11 = load i32, i32* %c
  %12 = load i32, i32* %a1
  %13 = load i32, i32* %b
  %14 = sdiv i32 %12, %13
  store i32 %14, i32* %c
  %15 = load i32, i32* %a1
  %16 = load i32, i32* %a1
  %17 = load i32, i32* %c
  %18 = load i32, i32* %b
  %19 = mul i32 %17, %18
  %20 = sub i32 %16, %19
  store i32 %20, i32* %a1
  %21 = load i32, i32* %b
  %22 = load i32, i32* %b
  %23 = sdiv i32 %22, 10
  store i32 %23, i32* %b
  %24 = load i32, i32* %c
  %25 = load i32, i32* %c
  %26 = add i32 %25, 48
  store i32 %26, i32* %c
  %27 = load i32, i32* %c
  %28 = call i32 @putchar(i32 %27)
  br label %whcond2

whafter4:                                         ; preds = %whcond2
  %29 = load i32, i32* %a1
  %30 = add i32 %29, 48
  %31 = call i32 @putchar(i32 %30)
  %32 = call i32 @putchar(i32 10)
  br label %ret

ret:                                              ; preds = %whafter4
  %33 = load i32, i32* %a1
  ret i32 %33
}

define i32 @fibo(i32 %a) {
entry:
  %a1 = alloca i32
  store i32 %a, i32* %a1
  %0 = load i32, i32* %a1
  %1 = icmp eq i32 %0, 1
  %2 = load i32, i32* %a1
  %3 = icmp eq i32 %2, 2
  %4 = or i1 %1, %3
  br i1 %4, label %ifthen, label %ifnot

ifthen:                                           ; preds = %entry
  br label %ret

ret:                                              ; preds = %ifthen
  ret i32 1

ifnot:                                            ; preds = %ret, %entry
  br label %ret2

ret2:                                             ; preds = %ifnot
  %5 = load i32, i32* %a1
  %6 = sub i32 %5, 1
  %7 = call i32 @fibo(i32 %6)
  %8 = load i32, i32* %a1
  %9 = sub i32 %8, 2
  %10 = call i32 @fibo(i32 %9)
  %11 = add i32 %7, %10
  ret i32 %11
}

define i32 @main() {
entry:
  %m = alloca i32
  %n = alloca i32
  %i = alloca i32
  store i32 1, i32* %i
  %0 = load i32, i32* %m
  %1 = call i32 @read()
  store i32 %1, i32* %m
  br label %whcond

whcond:                                           ; preds = %whrun, %entry
  %2 = load i32, i32* %i
  %3 = load i32, i32* %m
  %4 = icmp ule i32 %2, %3
  br i1 %4, label %whrun, label %whafter

whrun:                                            ; preds = %whcond
  %5 = load i32, i32* %n
  %6 = load i32, i32* %i
  %7 = call i32 @fibo(i32 %6)
  store i32 %7, i32* %n
  %8 = load i32, i32* %n
  %9 = call i32 @write(i32 %8)
  %10 = load i32, i32* %i
  %11 = load i32, i32* %i
  %12 = add i32 %11, 1
  store i32 %12, i32* %i
  br label %whcond

whafter:                                          ; preds = %whcond
  br label %ret

ret:                                              ; preds = %whafter
  ret i32 0
}
