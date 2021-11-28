; ModuleID = 'test'
source_filename = "test"

declare i32 @putchar(i32)

declare i32 @getchar()

define i32 @read() {
entry:
  %a1 = alloca i32
  store i32 0, i32* %a1
  %0 = load i32, i32* %a1
  %1 = call i32 @getchar()
  store i32 %1, i32* %a1
  br label %ret

ret:                                              ; preds = %entry
  %2 = load i32, i32* %a1
  %3 = sub i32 %2, 48
  ret i32 %3
}

define i32 @main() {
entry:
  %m1 = alloca i32
  %n1 = alloca i32
  %i1 = alloca i32
  store i32 48, i32* %i1
  %0 = load i32, i32* %m1
  %1 = call i32 @read()
  store i32 %1, i32* %m1
  %2 = load i32, i32* %n1
  %3 = call i32 @read()
  store i32 %3, i32* %n1
  %4 = load i32, i32* %m1
  %5 = load i32, i32* %n1
  %6 = icmp eq i32 %4, %5
  br i1 %6, label %ifthen, label %ifnot

ifthen:                                           ; preds = %entry
  %i2 = alloca i32
  store i32 1, i32* %i2
  %7 = load i32, i32* %i2
  %8 = load i32, i32* %i2
  %9 = add i32 %8, 48
  store i32 %9, i32* %i2
  %10 = load i32, i32* %i2
  %11 = call i32 @putchar(i32 %10)
  br label %ifafter

ifnot:                                            ; preds = %entry
  %12 = load i32, i32* %i2
  %13 = load i32, i32* %i2
  %14 = add i32 %13, 2
  store i32 %14, i32* %i2
  %15 = load i32, i32* %i2
  %16 = call i32 @putchar(i32 %15)
  br label %ifafter

ifafter:                                          ; preds = %ifnot, %ifthen
  %17 = load i32, i32* %i1
  %18 = load i32, i32* %i1
  %19 = add i32 %18, 1
  store i32 %19, i32* %i1
  %20 = load i32, i32* %i1
  %21 = call i32 @putchar(i32 %20)
  br label %ret

ret:                                              ; preds = %ifafter
  ret i32 0
}
