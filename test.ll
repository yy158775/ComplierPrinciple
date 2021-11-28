; ModuleID = 'test'
source_filename = "test"

declare i32 @putchar(i32)

declare i32 @getchar()

define i32 @main() {
entry:
  %b = alloca i32
  %a = alloca i32
  %c = alloca i32
  %d = alloca i32
  store i32 0, i32* %d
  store i32 0, i32* %b
  store i32 2, i32* %c
  %0 = load i32, i32* %b
  %1 = load i32, i32* %c
  %2 = add i32 %0, %1
  store i32 %2, i32* %a
  br label %ret

ret:                                              ; preds = %entry
  %3 = load i32, i32* %a
  %4 = call i32 @putchar(i32 %3)
  ret i32 %3
}
