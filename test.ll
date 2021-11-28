; ModuleID = 'test'
source_filename = "test"

declare i32 @putchar(i32)

declare i32 @getchar()

define i32 @read() {
entry:
  %a = alloca i32
  store i32 0, i32* %a
  %0 = load i32, i32* %a
  %1 = call i32 @getchar()
  store i32 %1, i32* %a
  br label %ret

ret:                                              ; preds = %entry
  %2 = load i32, i32* %a
  %3 = sub i32 %2, 48
  ret i32 %3
}

define i32 @main() {
entry:
  %m = alloca i32
  %n = alloca i32
  %i = alloca i32
  store i32 48, i32* %i
  %0 = load i32, i32* %m
  %1 = call i32 @read()
  store i32 %1, i32* %m
  %2 = load i32, i32* %n
  %3 = call i32 @read()
  store i32 %3, i32* %n
  %4 = load i32, i32* %m
  %5 = load i32, i32* %n
  %6 = icmp eq i32 %4, %5
  br i1 %6, label %ifthen, label %ifnot

ifthen:                                           ; preds = %entry
  %i1 = alloca i32
  store i32 1, i32* %i1
  %7 = load i32, i32* %i1
  %8 = load i32, i32* %i1
  %9 = add i32 %8, 48
  store i32 %9, i32* %i1
  %10 = load i32, i32* %i1
  %11 = call i32 @putchar(i32 %10)
  br label %ifafter

ifnot:                                            ; preds = %entry
  %12 = load i32, i32* %i
  %13 = load i32, i32* %i
  %14 = add i32 %13, 2
  store i32 %14, i32* %i
  %15 = load i32, i32* %i
  %16 = call i32 @putchar(i32 %15)
  br label %ifafter

ifafter:                                          ; preds = %ifnot, %ifthen
  %17 = load i32, i32* %i
  %18 = load i32, i32* %i
  %19 = add i32 %18, 1
  store i32 %19, i32* %i
  %20 = load i32, i32* %i
  %21 = call i32 @putchar(i32 %20)
  br label %ret

ret:                                              ; preds = %ifafter
  ret i32 0
}
