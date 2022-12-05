; ModuleID = 'mainprogram'
source_filename = "mainprogram"

@string.const = private global [5 x i8] c"dick\00"
@string.const.1 = private global [6 x i8] c"hello\00"
@string.const.2 = private global [7 x i8] c"hello!\00"

define void @main() {
entry:
  %env = alloca i8, i64 12
  br i1 true, label %iftrue, label %endif

iftrue:                                           ; preds = %entry
  br label %endif

endif:                                            ; preds = %iftrue, %entry
  %rawBytePtr = getelementptr i8, i8* %env, i64 8
  %x.ptr = bitcast i8* %rawBytePtr to i16*
  %x = load i16, i16* %x.ptr
  %rawBytePtr1 = getelementptr i8, i8* %env, i64 11
  call void @prime_int_char_char_ptr(i8* %env, i16 42, i8* %rawBytePtr1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @string.const.1, i32 0, i32 0))
  ret void
}

define void @prime_int_char_char_ptr(i8* %prevEnv, i16 %n, i8* %c, i8* %str) {
entry:
  %env = alloca i8, i64 126
  %env.0 = getelementptr i8, i8* %env, i64 0
  %env.0.casted = bitcast i8* %env.0 to i8**
  store i8* %prevEnv, i8** %env.0.casted
  %env.01 = getelementptr i8, i8* %env, i64 8
  %env.0.casted2 = bitcast i8* %env.01 to i16*
  store i16 %n, i16* %env.0.casted2
  %env.03 = getelementptr i8, i8* %env, i64 10
  %env.0.casted4 = bitcast i8* %env.03 to i8**
  store i8* %c, i8** %env.0.casted4
  %env.05 = getelementptr i8, i8* %env, i64 18
  %env.0.casted6 = bitcast i8* %env.05 to i8**
  store i8* %str, i8** %env.0.casted6
  %bitcastedEnv = bitcast i8* %env to i8**
  %prevEnv7 = load i8*, i8** %bitcastedEnv
  %rawBytePtr = getelementptr i8, i8* %prevEnv7, i64 10
  %z = load i8, i8* %rawBytePtr
  %cond = icmp ne i8 %z, 0
  br i1 %cond, label %iftrue, label %endif

iftrue:                                           ; preds = %entry
  br label %endif

endif:                                            ; preds = %iftrue, %entry
  %bitcastedEnv8 = bitcast i8* %env to i8**
  %prevEnv9 = load i8*, i8** %bitcastedEnv8
  %rawBytePtr10 = getelementptr i8, i8* %prevEnv9, i64 8
  %x.ptr = bitcast i8* %rawBytePtr10 to i16*
  %x = load i16, i16* %x.ptr
  %bitcastedEnv11 = bitcast i8* %env to i8**
  %prevEnv12 = load i8*, i8** %bitcastedEnv11
  %rawBytePtr13 = getelementptr i8, i8* %env, i64 10
  %c.ptr = bitcast i8* %rawBytePtr13 to i8**
  %c.byref.ptr = load i8*, i8** %c.ptr
  call void @prime_int_char_char_ptr(i8* %prevEnv12, i16 13, i8* %c.byref.ptr, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @string.const, i32 0, i32 0))
  call void @main()
  ret void
}